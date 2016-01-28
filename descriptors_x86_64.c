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

void set_expt_gate(intr_trap_gate* gate, expt_handler* eh) {
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

void set_idtr(intr_trap_gate idt[], u32 count) {
  descriptor_ptr dp;
  dp.limit = (u16)(((uptr)&idt[count] - (uptr)&idt[0] - 1)
      & 0xFFFF);
  dp.itg = &idt[0];
  load_idtr(&dp);
}

void set_seg_desc(seg_desc* sd, u32 seg_limit, u64 base_addr, u8 type,
    u8 s, u8 dpl, u8 p, u8 avl, u8 l, u8 d_b, u8 g) {
  seg_desc default_desc = ZERO_SEG_DESC;

  *sd = default_desc;
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

void set_code_seg_desc(seg_desc* sd, u8 accessed, u8 readable, u8 conforming,
    u8 dpl, u8 p, u8 avl, u8 l, u8 d, u8 g) {
    union seg_type_u type = {
      .code = {
        .a = accessed, .r = readable, .c = conforming, .one = 1
      },
  };

  set_seg_desc(sd, 0, 0, type.byte, 1, dpl, p, avl, l, d, g);
}

void set_data_seg_desc(seg_desc* sd, u8 accessed, u8 write_enable, u8 expand_dir,
    u8 dpl, u8 p, u8 avl, u8 l, u8 d, u8 g) {
  union seg_type_u type = {
      .data = {
        .a = accessed, .w = write_enable, .e = expand_dir, .zero = 0
      },
  };

  set_seg_desc(sd, 0, 0, type.byte, 1, dpl, p, avl, l, d, g);
}

/**
 * Non overlapping memory compare
 *
 * return: 0 if identical, < 0 if *ptr1 < *ptr2
 *         and > 0 if *ptr1 > ptr2
 */
i32 cmp_bytes(void* ptr1, void* ptr2, u32 len) {
  u8* p1 = (u8*)ptr1;
  u8* p2 = (u8*)ptr2;
  u8* pend = p1 + len;

  while (p1 < pend) {
    i32 result = *p1++ - *p2++;
    if (result != 0) {
      return result;
    }
  }
  return 0;
}

i32 cmp_seg_desc(seg_desc* sd1, seg_desc* sd2)  {
  return cmp_bytes(sd1, sd2, sizeof(seg_desc));
}

void set_tss_ldt_desc(tss_ldt_desc* tld, u32 seg_limit, u64 base_addr, u8 type,
    u8 dpl, u8 p, u8 avl, u8 g) {

  tss_ldt_desc default_desc = INITIALIZER_TSS_LDT_DESC;

  *tld = default_desc;
  tld->seg_limit_lo = TSS_LDT_DESC_SEG_LIMIT_LO(seg_limit);
  tld->seg_limit_hi = TSS_LDT_DESC_SEG_LIMIT_HI(seg_limit);
  tld->base_addr_lo = TSS_LDT_DESC_BASE_ADDR_LO(base_addr);
  tld->base_addr_hi = TSS_LDT_DESC_BASE_ADDR_HI(base_addr);
  tld->type = type;
  tld->dpl = dpl;
  tld->p = p;
  tld->avl = avl;
  tld->g = g;
}

i32 cmp_tss_ldt_desc(tss_ldt_desc* sd1, tss_ldt_desc* sd2)  {
  return cmp_bytes(sd1, sd2, sizeof(tss_ldt_desc));
}
