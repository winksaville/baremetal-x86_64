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
#include "descriptors_x86_64.h"
#include "descriptors_x86_64_print.h"

void set_intr_gate(intr_trap_gate* gate, intr_handler* ih) {
  intr_trap_gate g = INITIALIZER_INTR_TRAP_GATE;
  *gate = g;

  // Set other fields
  gate->offset_lo = GATE_OFFSET_LO(ih);
  gate->offset_hi = GATE_OFFSET_HI(ih);
  gate->type = DT_64_INTR_GATE;
}

void set_expt_gate(intr_trap_gate* gate, expt_handler* eh) {
  intr_trap_gate g = INITIALIZER_INTR_TRAP_GATE;
  *gate = g;

  // Set other fields
  gate->offset_lo = GATE_OFFSET_LO(eh);
  gate->offset_hi = GATE_OFFSET_HI(eh);
  gate->type = DT_64_INTR_GATE;
}

void set_idtr(intr_trap_gate idt[], u32 count) {
  descriptor_ptr dp;
  dp.limit = (u16)(((uptr)&idt[count] - (uptr)&idt[0] - 1)
      & 0xFFFF);
  dp.itg = &idt[0];
  load_idtr(&dp);
}

void set_seg_desc(seg_desc* sd, u32 seg_limit, u64 base_addr, u8 type,
    u8 s, u8 dpl, u8 p, u8 avl, u8 l, u8 d_b, u8 g) {

  sd->seg_limit_lo = SEG_DESC_SEG_LIMIT_LO(seg_limit);
  sd->seg_limit_hi = SEG_DESC_SEG_LIMIT_HI(seg_limit);
  sd->base_addr_lo = SEG_DESC_BASE_ADDR_LO(base_addr);
  sd->base_addr_hi = SEG_DESC_BASE_ADDR_HI(base_addr);
  sd->type = type;
  sd->s = s;
  sd->dpl = dpl;
  sd->p = p;
  sd->avl = avl;
  sd->l = l;
  sd->d_b = d_b;
  sd->g = g;
}
