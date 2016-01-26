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
#include "descriptors_x86_64_print.h"

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
