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
#include "abort.h"
#include "print.h"
#include "regs_x86_64.h"
#include "descriptors_x86_64.h"
#include "interrupts.h"
#include "descriptors_x86_64_print.h"

static u64 intr_79_counter;

__attribute__ ((__interrupt__))
static void intr_79(struct intr_frame *frame) {
  (void)frame;
  intr_79_counter += 1;
  print_intr_frame("intr 79:", frame);
  print_u64_nl(" rsp: ", get_rsp());
  print_int_nl(" intr_79_counter: ", intr_79_counter);
}

void test_interrupts() {
  // Displat the current idtr
  descriptor_ptr idtr;
  print_str_nl("idtr:");
  store_idtr(&idtr);
  print_u16_nl(" idtr.limit: ", idtr.limit);
  print_uptr_nl(" idtr.itg: ", idtr.itg);

  // Test we can set an interrupt handler and invoke it
  set_intr_handler(79, intr_79);
  print_gate("idt[79]:", get_intr_trap_gate(79));
  print_str_nl("invoke intr(79)");
  intr_79_counter = 0;
  intr(79);
  print_str_nl("done   intr(79)");
}
