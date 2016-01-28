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

static u64 intr_undefined_counter;
static u64 expt_undefined_counter;
static u64 intr_invalid_opcode_counter;

__attribute__ ((__interrupt__))
static void intr_undefined(struct intr_frame *frame) {
  intr_undefined_counter += 1;
  print_intr_frame("intr_undefined:", frame);
  print_u64_nl(" rsp: ", get_rsp());
  print_int_nl(" intr_undefined_counter: ", intr_undefined_counter);
}

__attribute__ ((__interrupt__))
static void expt_08_undefined(struct intr_frame *frame, u64 error_code) {
  expt_undefined_counter += 1;
  print_intr_frame("expt_08_undefined:", frame);
  print_u64_nl(" rsp: ", get_rsp());
  print_u64_nl(" error_code: ", error_code);
  print_int_nl(" expt_undefined_counter: ", expt_undefined_counter);
}

__attribute__ ((__interrupt__))
static void expt_10_undefined(struct intr_frame *frame, u64 error_code) {
  expt_undefined_counter += 1;
  print_intr_frame("expt_10_undefined:", frame);
  print_u64_nl(" rsp: ", get_rsp());
  print_u64_nl(" error_code: ", error_code);
  print_int_nl(" expt_undefined_counter: ", expt_undefined_counter);
}

__attribute__ ((__interrupt__))
static void expt_11_undefined(struct intr_frame *frame, u64 error_code) {
  expt_undefined_counter += 1;
  print_intr_frame("expt_11_undefined:", frame);
  print_u64_nl(" rsp: ", get_rsp());
  print_u64_nl(" error_code: ", error_code);
  print_int_nl(" expt_undefined_counter: ", expt_undefined_counter);
}

__attribute__ ((__interrupt__))
static void expt_12_undefined(struct intr_frame *frame, u64 error_code) {
  expt_undefined_counter += 1;
  print_intr_frame("expt_12_undefined:", frame);
  print_u64_nl(" rsp: ", get_rsp());
  print_u64_nl(" error_code: ", error_code);
  print_int_nl(" expt_undefined_counter: ", expt_undefined_counter);
}

__attribute__ ((__interrupt__))
static void expt_13_undefined(struct intr_frame *frame, u64 error_code) {
  expt_undefined_counter += 1;
  print_intr_frame("expt_13_undefined:", frame);
  print_u64_nl(" rsp: ", get_rsp());
  print_u64_nl(" error_code: ", error_code);
  print_int_nl(" expt_undefined_counter: ", expt_undefined_counter);
}

__attribute__ ((__interrupt__))
static void expt_14_undefined(struct intr_frame *frame, u64 error_code) {
  expt_undefined_counter += 1;
  print_intr_frame("expt_14_undefined:", frame);
  print_u64_nl(" rsp: ", get_rsp());
  print_u64_nl(" error_code: ", error_code);
  print_int_nl(" expt_undefined_counter: ", expt_undefined_counter);
}

__attribute__ ((__interrupt__))
static void expt_17_undefined(struct intr_frame *frame, u64 error_code) {
  expt_undefined_counter += 1;
  print_intr_frame("expt_17_undefined:", frame);
  print_u64_nl(" rsp: ", get_rsp());
  print_u64_nl(" error_code: ", error_code);
  print_int_nl(" expt_undefined_counter: ", expt_undefined_counter);
}

__attribute__ ((__interrupt__))
static void intr_invalid_opcode(struct intr_frame *frame) {
  intr_invalid_opcode_counter += 1;
  print_intr_frame("intr_invalid_opcode:", frame);
  print_u64_nl(" rsp: ", get_rsp());
  print_int_nl(" intr_invalid_opcode_counter: ", intr_invalid_opcode_counter);
}

static void set_intr_gate(intr_trap_gate* gate, intr_handler* ih) {
  static intr_trap_gate g = INITIALIZER_INTR_TRAP_GATE;
  *gate = g;

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

intr_trap_gate* get_intr_trap_gate(u32 intr_num) {
  return &idt[intr_num];
}

void initialize_intr_trap_table() {
  intr_undefined_counter = 0;
  expt_undefined_counter = 0;
  intr_invalid_opcode_counter = 0;

  // Initialize all of the entires to intr_undefined
  for (u64 intr_num = 0; intr_num < ARRAY_COUNT(idt); intr_num++) {
    set_intr_gate(&idt[intr_num], intr_undefined);
  }
  set_intr_gate(&idt[6], intr_invalid_opcode);

  // Initialize some exceptions
  set_expt_gate(&idt[8], expt_08_undefined);
  set_expt_gate(&idt[10], expt_10_undefined);
  set_expt_gate(&idt[11], expt_11_undefined);
  set_expt_gate(&idt[12], expt_12_undefined);
  set_expt_gate(&idt[13], expt_13_undefined);
  set_expt_gate(&idt[14], expt_14_undefined);
  set_expt_gate(&idt[17], expt_17_undefined);

  // set the idtr
  set_idtr(idt, ARRAY_COUNT(idt));
}
