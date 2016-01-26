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

void setidt_intr(intr_trap_gate idt[], u64 idx, intr_handler* ih) {
  intr_trap_gate g = INITIALIZER_INTR_TRAP_GATE;
  intr_trap_gate* p_entry = &idt[idx];

  // Initialize the entry to known values
  *p_entry = g;

  // Set other fields
  p_entry->offset_lo = GATE_OFFSET_LO(ih);
  p_entry->offset_hi = GATE_OFFSET_HI(ih);
  p_entry->type = DT_64_INTR_GATE;
}

void setidt_expt(intr_trap_gate idt[], u64 idx, expt_handler* eh) {
  intr_trap_gate g = INITIALIZER_INTR_TRAP_GATE;
  intr_trap_gate* p_entry = &idt[idx];

  // Initialize the entry to known values
  *p_entry = g;

  // Set other fields
  p_entry->offset_lo = GATE_OFFSET_LO(eh);
  p_entry->offset_hi = GATE_OFFSET_HI(eh);
  p_entry->type = DT_64_INTR_GATE;
}

void setidtr(intr_trap_gate idt[], u32 count) {
  descriptor_ptr dp;
  dp.limit = (u16)(((uptr)&idt[count] - (uptr)&idt[0] - 1)
      & 0xFFFF);
  dp.address = (uptr)&idt[0];
  load_idtr(&dp);
}
