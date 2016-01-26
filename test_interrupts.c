/*
 * Copyright 2016 Wink Saville
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
#include "print.h"
#include "descriptors_x86_64.h"

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
  load_idtr(&dp);
}

void test_interrupts() {
  descriptor_ptr desc_ptr = {
    .limit = 0x1234,
    .address = 0x1234567812345678,
  };

  print_int_nl("0:                ", 0);
  print_int_nl("1:                ", 1);
  print_int_nl("7F..FF:           ", 0x7FFFFFFFFFFFFFFFLL);
  print_int_nl("FF..FF:           ", 0xFFFFFFFFFFFFFFFFLL);
  print_int_nl("-1:               ", -1);

  print_int_nl("sizeof(desc_ptr): ", sizeof(desc_ptr));
  print_u16_nl("desc_ptr.limit:   ", desc_ptr.limit);
  print_u64_nl("desc_ptr.address: ", desc_ptr.address);
  load_idtr(&desc_ptr);

  descriptor_ptr idtr;
  store_idtr(&idtr);
  print_u16_nl("idtr.limit:   ", idtr.limit);
  print_u64_nl("idtr.address: ", idtr.address);

  if (idtr.limit != desc_ptr.limit) {
    print_str_nl("ERROR desc_ptr.limit != idtr.limit");
  }
  if (idtr.address != desc_ptr.address) {
    print_str_nl("ERROR desc_ptr.address != idtr.address");
  }

  descriptor_ptr gdtr;
  store_gdtr(&gdtr);
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
  store_idtr(&idtr);
  print_u64_nl("sizeof(idt):         ", sizeof(idt));
  print_u64_nl("sizeof(typeof(idt)): ", sizeof(__typeof__(idt[0])));
  print_u64_nl("ARRAY_COUNT(idt):    ", ARRAY_COUNT(idt));
  print_uptr_nl("&idt[0]:             ", &idt[0]);
  print_uptr_nl("&idt[last]:          ", &idt[ARRAY_COUNT(idt) - 1]);
  print_u16_nl("idtr.limit:          ", idtr.limit);
  print_u64_nl("idtr.address:        ", idtr.address);
}
