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
#include "x86_64_descriptors.h"

// From [here](//http://www.lammertbies.nl/comm/info/RS-232_io.html)
#define COM1_BASE 0x3F8
#define COM2_BASE 0x2F8
#define COM3_BASE 0x3E8
#define COM4_BASE 0x2E8

#define COM1_IRQ 4
#define COM2_IRQ 3
#define COM3_IRQ 4
#define COM4_IRQ 3

// and [here](http://www.lammertbies.nl/comm/info/serial-uart.html)
#define RX_FIFO 0
#define TX_FIFO 0

#define COM_TX_FIFO ((short int)(COM1_BASE + TX_FIFO))

static u8* u4_2_ascii = (u8*)"0123456789ABCDEF";

static inline void outb(u16 port, u8 val) {
    __asm volatile ( "outb %0, %1" : : "a"(val), "d"(port) );
}

void putchar(char ch) {
  outb(COM_TX_FIFO, ch);
}

void print_nl() {
  putchar('\n');
}

void print_str(char* s) {
  u8 ch;
  while((ch = *s++) != 0) {
    putchar(ch);
  }
}

void print_int(long long v) {
  if (v == 0) {
    putchar('0');
  } else {
    // A buffer large enough to hold a 2^64 integer
    // converted to decaimal without a trailing 0
    // calculated using  roundup(log(2^64))
    char buff[20];
    unsigned int i = 0;
    int negative = 0;

    if (v < 0) {
      negative = 1;
      v = -v;
    }
    while (i < sizeof(buff) && (v > 0)) {
      buff[i++] = u4_2_ascii[v % 10];
      v /= 10;
    }
    if (negative) {
      putchar('-');
    }
    while (i > 0) {
      putchar(buff[--i]);
    }
  }
}

void print_u8(u8 v) {
  for (unsigned int i = 0; i < (sizeof(v) * 2); i++) {
    putchar(u4_2_ascii[(v & 0xF0) >> 4]);
    v <<= 4;
  }
}

void print_u16(u16 v) {
  for (unsigned int i = 0; i < (sizeof(v) * 2); i++) {
    putchar(u4_2_ascii[(v & 0xF000) >> 12]);
    v <<= 4;
  }
}

void print_u32(u32 v) {
  for (unsigned int i = 0; i < (sizeof(v) * 2); i++) {
    putchar(u4_2_ascii[(v & 0xF0000000) >> 28]);
    v <<= 4;
  }
}

void print_u64(u64 v) {
  print_u32((v >> 32) & 0xFFFFFFFF);
  print_u32(v & 0xFFFFFFFF);
}

void print_uptr(void* p) {
  print_u64((u64)p);
}

void print_str_nl(char *str) {
  print_str(str);
  print_nl();
}

void print_str_str_nl(char *s, char *str) {
  print_str(s);
  print_str(str);
  print_nl();
}


void print_int_nl(char *s, long long v) {
  print_str(s);
  print_int(v);
  print_nl();
}

void print_u8_nl(char *s, u8 v) {
  print_str(s);
  print_u8(v);
  print_nl();
}

void print_u16_nl(char *s, u16 v) {
  print_str(s);
  print_u16(v);
  print_nl();
}

void print_u32_nl(char *s, u32 v) {
  print_str(s);
  print_u32(v);
  print_nl();
}

void print_u64_nl(char *s, u64 v) {
  print_str(s);
  print_u64(v);
  print_nl();
}

void print_uptr_nl(char *s, void* v) {
  print_str(s);
  print_uptr(v);
  print_nl();
}

void print_u8s(u8* s, u32 len) {
  print_uptr(s);
  print_str(": ");
  while(len-- > 0) {
    print_u8(*s++);
    putchar(' ');
  }
  print_nl();
}

#pragma pack(push,1)
typedef struct multiboot_header_tag {
  u32 type;
  u32 size;
} multiboot_header_tag;
#pragma pack(pop)

multiboot_header_tag* print_tag(multiboot_header_tag *tag) {
  print_str("tag=");
  print_uptr(tag);
  print_str(" type=");
  print_u32(tag->type);
  print_str(" size=");
  print_int(tag->size);
  print_nl();

  void* next = (void*)((((uptr)tag + tag->size) + 7) & 0xfffffffffffffff8);
  if (next == tag) {
    print_str("Something is wrong next == tag returning NULL");
    print_nl();
    next = NULL;
  }
  return (multiboot_header_tag*)next;
}

__attribute__ ((__noreturn__))
void abort() {
  // Abort by executing the x86 undefined instruction
  while (1) {
    __asm__ ("ud2");
  }
}


void print_intr_frame(char* str, intr_frame* frame) {
  print_str_nl(str);
  print_u64_nl(" frame->ip: ", frame->ip);
  print_u64_nl(" frame->cs: ", frame->cs);
  print_u64_nl(" frame->flags: ", frame->flags);
  print_u64_nl(" frame->sp: ", frame->sp);
  print_u64_nl(" frame->ss: ", frame->ss);
}

void print_gate(char *str, intr_trap_gate* gate) {
  print_str_nl(str);
  print_u16_nl(" gate->offset_lo: ", gate->offset_lo);
  print_u16_nl(" gate->segment: ", gate->segment);
  print_u16_nl(" gate->ist: ", gate->ist);
  print_u16_nl(" gate->unused_1: ", gate->unused_1);
  print_u16_nl(" gate->type: ", gate->type);
  print_u16_nl(" gate->unused_2: ", gate->unused_2);
  print_u16_nl(" gate->dpl: ", gate->dpl);
  print_u16_nl(" gate->p: ", gate->p);
  print_u64_nl(" gate->offset_hi: ", gate->offset_hi);
  print_u64_nl(" gate->offset:    ", GET_GATE_OFFSET(*gate));
  print_u32_nl(" gate->unused_3:    ", gate->unused_3);
}

__attribute__ ((__interrupt__, __used__))
void intr_unused(struct intr_frame *frame) {
  print_intr_frame("intr_unused:", frame);
}

__attribute__ ((__interrupt__, __used__))
void expt_unused(struct intr_frame *frame, u64 error_code) {
  print_intr_frame("expt_unused:", frame);
  print_u64_nl(" error_code=", error_code);
}

__attribute__ ((__interrupt__, __used__))
void expt_invalid_opcode(struct intr_frame *frame, u64 error_code) {
  print_intr_frame("expt_invalid_opcode:", frame);
  print_u64_nl(" error_code=", error_code);
}

/* Interrupt Descriptor Table */
intr_trap_gate idt[256];

#define ARRAY_COUNT(__array) (sizeof(__array)/sizeof(__typeof__(__array[0])))

void setidt_intr(u64 idx, intr_handler* ih) {
  intr_trap_gate g = INITIALIZER_INTR_TRAP_GATE;
  idt[idx] = g;
  idt[idx].offset_lo = GATE_OFFSET_LO(ih);
  idt[idx].offset_hi = GATE_OFFSET_HI(ih);
  idt[idx].type = DT_64_INTR_GATE;
}

void setidt_expt(u64 idx, expt_handler* eh) {
  intr_trap_gate g = INITIALIZER_INTR_TRAP_GATE;
  idt[idx] = g;
  idt[idx].offset_lo = GATE_OFFSET_LO(eh);
  idt[idx].offset_hi = GATE_OFFSET_HI(eh);
}

void setidtr() {
  descriptor_ptr dp;
  dp.limit = (u16)(((uptr)&idt[ARRAY_COUNT(idt)] - (uptr)&idt[0] - 1) & 0xFFFF);
  dp.address = (uptr)idt;
  x86_lidt(&dp);
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
// Previously he used 1GB pages there they are only supported in newer
// cpu's so he's nowusing the 2MB pages.

__attribute__ ((__noreturn__))
void kmain(void* mb_info) {
  u32 total_size = *(u32*)mb_info;
  u32 reserved = *(u32*)(mb_info + 4);
  multiboot_header_tag* mb_end = (multiboot_header_tag*)(mb_info + total_size);
  multiboot_header_tag* tag = (multiboot_header_tag*)(mb_info + 8);

  print_uptr_nl("mb_info:    ", mb_info);
  print_int_nl( "total_size: ", total_size);
  print_int_nl( "reserved:   ", reserved);
  print_uptr_nl("mb_end:     ", mb_end);

  while ((tag < mb_end) && (tag->type != 0) && (tag->size != 8)) {
    multiboot_header_tag* next = print_tag(tag);
    if (next == NULL) break;

    if (tag->type == 1) {
      char* cmdline = (char*)tag + 8;
      print_str_str_nl("cmd_line=", cmdline);
    }
    tag = next;
  }

  descriptor_ptr desc_ptr;
  desc_ptr.limit = 0x1234;
  desc_ptr.address = 0x1234567812345678;

  print_int_nl("0:                ", 0);
  print_int_nl("1:                ", 1);
  print_int_nl("7F..FF:           ", 0x7FFFFFFFFFFFFFFFLL);
  print_int_nl("FF..FF:           ", 0xFFFFFFFFFFFFFFFFLL);
  print_int_nl("-1:               ", -1);
  print_int_nl("sizeof(desc_ptr): ", sizeof(desc_ptr));
  print_u8_nl("sizeof(desc_ptr): ", (u8)sizeof(desc_ptr));
  print_u16_nl("desc_ptr.limit:   ", desc_ptr.limit);
  print_u64_nl("desc_ptr.address: ", desc_ptr.address);

  x86_lidt(&desc_ptr);

  descriptor_ptr idtr;
  x86_sidt(&idtr);
  print_u16_nl("idtr.limit:   ", idtr.limit);
  print_u64_nl("idtr.address: ", idtr.address);

  if (idtr.limit != desc_ptr.limit) {
    print_str_nl("ERROR desc_ptr.limit != idtr.limit");
  }
  if (idtr.address != desc_ptr.address) {
    print_str_nl("ERROR desc_ptr.address != idtr.address");
  }

  descriptor_ptr gdtr;
  x86_sgdt(&gdtr);
  print_u16_nl("gdtr.limit:   ", gdtr.limit);
  print_u64_nl("gdtr.address: ", gdtr.address);

  uptr	offset = 0x1234567812345678;
  intr_trap_gate gate = INITIALIZER_INTR_TRAP_GATE;
  print_int_nl("sizeof(gate):   ", sizeof(gate));
  print_gate("gate default initialization:", &gate);

  gate.offset_lo = GATE_OFFSET_LO(offset);
  gate.segment = 0x1234;
  gate.ist = 0x7;
  gate.type = 0xF;
  gate.dpl = 0x3;
  gate.p = 0x1;
  gate.offset_hi = GATE_OFFSET_HI(offset);
  print_gate("gate specific initialization:", &gate);

  for (u64 idx = 0; idx < ARRAY_COUNT(idt); idx++) {
    setidt_intr(idx, intr_unused);
  }
  setidt_expt(6, expt_invalid_opcode);
  print_gate("invalid opcode gate:", &idt[6]);

  setidtr();
  x86_sidt(&idtr);
  print_u64_nl("sizeof(idt):         ", sizeof(idt));
  print_u64_nl("sizeof(typeof(idt)): ", sizeof(__typeof__(idt[0])));
  print_u64_nl("ARRAY_COUNT(idt):    ", ARRAY_COUNT(idt));
  print_uptr_nl("idt[0]:              ", &idt[0]);
  print_uptr_nl("idt[last]:           ", &idt[ARRAY_COUNT(idt)]);
  print_u16_nl("idtr.limit:          ", idtr.limit);
  print_u64_nl("idtr.address:        ", idtr.address);

  abort();
}
