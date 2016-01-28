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
#include "regs_x86_64.h"
#include "interrupts.h"
#include "descriptors_x86_64.h"
#include "descriptors_x86_64_print.h"

/* Interrupt Descriptor Table */
static intr_trap_gate idt[256];

__attribute__ ((__interrupt__, __used__))
static void intr_undefined(struct intr_frame *frame) {
  (void)frame;
  putchar('I');
 //print_intr_frame("intr_undefined:", frame);
}

__attribute__ ((__interrupt__, __used__))
static void expt_undefined(struct intr_frame *frame, u64 error_code) {
  (void)frame;
  (void) error_code;
  putchar('E');
  //print_intr_frame("expt_undefined:", frame);
  //print_u64_nl(" error_code=", error_code);
}

__attribute__ ((__interrupt__, __used__))
static void expt_invalid_opcode(struct intr_frame *frame, u64 error_code) {
  (void)frame;
  (void) error_code;
  putchar('O');
  //print_intr_frame("expt_invalid_opcode:", frame);
  //print_u64_nl(" error_code=", error_code);
}

static void set_intr_gate(intr_trap_gate* gate, intr_handler* ih) {
  static intr_trap_gate g = INITIALIZER_INTR_TRAP_GATE;
  *gate = g;

  // Set other fields
  gate->offset_lo = GATE_OFFSET_LO(ih);
  gate->offset_hi = GATE_OFFSET_HI(ih);
  gate->segment = 8; // Code Segment TI=0, RPL=0
                     // FIXME: How to know where the Code Segement is
  gate->ist = 0; // Modified legacy stack switching mode
  gate->type = DT_64_INTR_GATE;
  gate->dpl = 0;
  gate->p = 1;
}

static void set_expt_gate(intr_trap_gate* gate, expt_handler* eh) {
  static intr_trap_gate g = INITIALIZER_INTR_TRAP_GATE;
  *gate = g;

  // Set other fields
  gate->offset_lo = GATE_OFFSET_LO(eh);
  gate->offset_hi = GATE_OFFSET_HI(eh);
  gate->segment = 8; // Code Segment TI=0, RPL=0
                     // FIXME: How to know where the Code Segement is
  gate->ist = 0; // Modified legacy stack switching mode
  gate->type = DT_64_INTR_GATE;
  gate->dpl = 0;
  gate->p = 1;
}

static void set_idtr(intr_trap_gate idt[], u32 count) {
  descriptor_ptr dp;
  dp.limit = (u16)(((uptr)&idt[count] - (uptr)&idt[0] - 1)
      & 0xFFFF);
  dp.itg = &idt[0];
  load_idtr(&dp);
}

void set_intr_handler(u32 intr_num, intr_handler ih) {
  set_intr_gate(&idt[intr_num], ih);
}

void set_expt_handler(u32 intr_num, expt_handler eh) {
  set_expt_gate(&idt[intr_num], eh);
}

void initialize_intr_trap_table() {
  // Initialize all of the entires to intr_undefined
  for (u64 idx = 0; idx < ARRAY_COUNT(idt); idx++) {
    set_intr_gate(&idt[idx], intr_undefined);
  }

  // Initialize some exceptions
  set_expt_gate(&idt[6], expt_invalid_opcode);

  // set the idtr
  set_idtr(idt, ARRAY_COUNT(idt));
}
