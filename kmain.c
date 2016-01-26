/*
 * Copyright 2015 Wink Saville
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "inttypes.h"
#include "regs_x86_64.h"
#include "print.h"
#include "test_multiboot.h"
#include "test_interrupts.h"

__attribute__ ((__noreturn__))
void abort() {
  // Abort by executing the x86 undefined instruction
  while (1) {
    __asm__ ("ud2");
  }
}


// [Multiboot 1.6](http://nongnu.askapache.com/grub/phcoder/multiboot.pdf) info.
// [OSDev.org Mulitboot2](http://wiki.osdev.org/Multiboot#Multiboot_2) info.
//
// The other critical information is what is the memory map at this point,
// [see Philipp's](http://os.phil-opp.com/entering-longmode.html) post. He
// initializes an identity mapping (1:1 physical to virtual) with one P4,
// one P3 and one P2. With each P2 entry pointing to 2MB of physical data.
// So the first 1G of physical RAM is mapped 1:1 with virtual memory.
//
// Previously he used 1GB pages but they are only supported in newer
// cpu's so he's nowusing the 2MB pages.

__attribute__ ((__noreturn__))
void kmain(void* mb_info) {
  (void)mb_info;

  print_u16_nl("ds=", read_x86_ds());
  print_u16_nl("ss=", read_x86_ss());
  print_u16_nl("es=", read_x86_es());

  //write_x86_ds(0x00);
  //write_x86_ss(0x00);
  //write_x86_es(0x00);

  //print_u16_nl("ds=", read_x86_ds());
  //print_u16_nl("ss=", read_x86_ss());
  //print_u16_nl("es=", read_x86_es());

  test_multiboot(mb_info);
  test_interrupts();

  abort();
}
