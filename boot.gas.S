/*
 * Copyrigh;t 2015 Philipp Oppermann
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

  .global _start
  .extern kmain

  .section .text
  .code32
_start:
    movl $stack_top, %esp
    // Move Multiboot info pointer to edi to pass it to the kernel. We must not
    // modify the `edi` register until the kernel it called.
    movl %ebx, %edi

    call check_multiboot
    call check_cpuid
    call check_long_mode

    call set_up_page_tables
    call enable_paging
    call set_up_SSE

    // load the 64-bit GDT
    lgdt (gdt64_pointer_entry)

    // update selectors
    movw $gdt64_data_seg, %ax
    movw %ax, %ss
    movw %ax, %ds
    movw %ax, %es

    // edi = multiboot info
    //ljmp gdt64_code_seg, kmain
    .byte 0xea
    .long kmain
    .word 0x08

set_up_page_tables:
    // recursive map P4
    mov $p4_table, %eax
    orl $0b11, %eax       // present + writable
    movl %eax, (p4_table + 511 * 8)

    // map first P4 entry to P3 table
    movl $p3_table, %eax
    orl $0b11, %eax       // present + writable
    movl %eax, (p4_table)

    // map first P3 entry to P2 table
    movl $p2_table, %eax
    orl $0b11, %eax       // present + writable
    mov %eax, (p3_table)

    // map each P2 entry to a huge 2MiB page
    movl $0, %ecx         // counter variable
map_p2_table:
    // map ecx-th P2 entry to a huge page that starts at address (2MiB * ecx)
    movl $0x200000, %eax  // 2MiB
    mul %ecx              // start address of ecx-th page
    orl $0b10000011, %eax // present + writable + huge
    movl %eax, p2_table(,%ecx,8) // map ecx-th entry

    inc %ecx              // increase counter
    cmp $512, %ecx        // if counter == 512, the whole P2 table is mapped
    jne map_p2_table      // else map the next entry

    ret

enable_paging:
    // load P4 to cr3 register (cpu uses this to access the P4 table)
    movl $p4_table, %eax
    movl %eax, %cr3

    // enable PAE-flag in cr4 (Physical Address Extension)
    movl %cr4, %eax
    orl $1 << 5, %eax
    mov %eax, %cr4

    // set the long mode bit in the EFER MSR (model specific register)
    mov $0xC0000080, %ecx
    rdmsr
    orl $1 << 8, %eax
    wrmsr

    // enable paging in the cr0 register
    movl %cr0, %eax
    orl $1 << 31, %eax
    mov %eax, %cr0

    ret

// Prints `ERR: ` and the given error code to screen and hangs.
// parameter: error code (in ascii) in al
error:
    movl $0x4f524f45, (0xb8000)
    movl $0x4f3a4f52, (0xb0004)
    movl $0x4f204f20, (0xb8008)
    movb %al, (0xb800a)
    hlt

// Throw error 0 if eax doesn't contain the Multiboot 2 magic value (0x36d76289).
check_multiboot:
    cmpl $0x36d76289, %eax
    jne no_multiboot
    ret
no_multiboot:
    movb $'0', %al
    jmp error

// Throw error 1 if the CPU doesn't support the CPUID command.
check_cpuid:
    pushf                // Store the FLAGS-register.
    pop %eax             // Restore the A-register.
    mov %eax, %ecx       // Set the C-register to the A-register.
    xor $1 << 21, %eax   // Flip the ID-bit, which is bit 21.
    push %eax            // Store the A-register.
    popf                 // Restore the FLAGS-register.
    pushf                // Store the FLAGS-register.
    pop %eax             // Restore the A-register.
    push %ecx            // Store the C-register.
    popf                 // Restore the FLAGS-register.
    xor %ecx, %eax       // Do a XOR-operation on the A-register and the C-register.
    jz no_cpuid          // The zero flag is set, no CPUID.
    ret                  // CPUID is available for use.
no_cpuid:
    mov $'1', %al
    jmp error

// Throw error 2 if the CPU doesn't support Long Mode.
check_long_mode:
    movl $0x80000000, %eax  // Set the A-register to 0x80000000.
    cpuid                   // CPU identification.
    cmp $0x80000001, %eax   // Compare the A-register with 0x80000001.
    jb no_long_mode         // It is less, there is no long mode.
    movl $0x80000001, %eax  // Set the A-register to 0x80000001.
    cpuid                   // CPU identification.
    test $1 << 29, %edx     // Test if the LM-bit, which is bit 29, is set in the D-register.
    jz no_long_mode         // They aren't, there is no long mode.
    ret
no_long_mode:
    mov $'2', %al
    jmp error

// Check for SSE and enable it. If it's not supported throw error "a".
set_up_SSE:
    // check for SSE
    movl $0x1, %eax
    cpuid
    testl $1<<25, %edx
    jz no_SSE

    // enable SSE
    movl %cr0, %eax
    andw  $0xFFFB, %ax      // clear coprocessor emulation CR0.EM
    orw $0x2, %ax          // set coprocessor monitoring  CR0.MP
    movl %eax, %cr0
    movl %cr4, %eax
    orw $3 << 9, %ax       // set CR4.OSFXSR and CR4.OSXMMEXCPT at the same time
    movl %eax, %cr4

    ret
no_SSE:
    movb $'a', %al
    jmp error

  .section .bss
  .align 4096
p4_table:
    .skip 4096
p3_table:
    .skip 4096
p2_table:
    .skip 4096
stack_bottom:
    .skip 4096 * 2
stack_top:

  .section .rodata
gdt64_zero_entry:
  .quad 0 // zero entry

gdt64_code_entry:
  .set gdt64_code_seg, gdt64_code_entry - gdt64_zero_entry
  .quad (1<<44) | (1<<47) | (1<<41) | (1<<43) | (1<<53) + kmain // code segment

gdt64_data_entry:
  .set gdt64_data_seg, gdt64_data_entry - gdt64_zero_entry
  .quad (1<<44) | (1<<47) | (1<<41) // data segment

gdt64_pointer_entry:
  .set gdt64_pointer_seg, gdt64_data_entry - gdt64_pointer_entry - 1
  .word gdt64_pointer_seg
  .quad gdt64_zero_entry
