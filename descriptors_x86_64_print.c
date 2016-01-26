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
  print_int_nl(" gate->ist: ", gate->ist);
  print_int_nl(" gate->unused_1: ", gate->unused_1);
  print_int_nl(" gate->type: ", gate->type);
  print_int_nl(" gate->unused_2: ", gate->unused_2);
  print_int_nl(" gate->dpl: ", gate->dpl);
  print_int_nl(" gate->p: ", gate->p);
  print_u64_nl(" gate->offset_hi: ", gate->offset_hi);
  print_u32_nl(" gate->unused_3: ", gate->unused_3);
  print_u64_nl(" gate->offset: ", GET_GATE_OFFSET(*gate));
}

void print_tss_ldt_desc(char *str, tss_ldt_desc* desc) {
  print_str_nl(str);
  print_u16_nl(" desc->seg_limit_lo: ", desc->seg_limit_lo);
  print_u32_nl(" desc->base_addr_lo: ", desc->base_addr_lo);
  print_int_nl(" desc->type: ", desc->type);
  print_int_nl(" desc->unused_1: ", desc->unused_1);
  print_int_nl(" desc->dpl: ", desc->dpl);
  print_int_nl(" desc->p: ", desc->p);
  print_u8_nl(" desc->seg_limit_hi: ", desc->seg_limit_hi);
  print_int_nl(" desc->avl: ", desc->avl);
  print_int_nl(" desc->unused_2: ", desc->unused_2);
  print_int_nl(" desc->g: ", desc->g);
  print_u64_nl(" desc->base_addr_hi: ", desc->base_addr_hi);
  print_u32_nl(" desc->unused_3: ", desc->unused_3);
  print_u32_nl(" desc->seg_limit: ", GET_TSS_LDT_DESC_SEG_LIMIT(*desc));
  print_u64_nl(" desc->base_addr: ", GET_TSS_LDT_DESC_BASE_ADDR(*desc));
}

void print_seg_desc(char *str, seg_desc* desc) {
  print_str_nl(str);
  print_u16_nl(" desc->seg_limit_lo: ", desc->seg_limit_lo);
  print_u32_nl(" desc->base_addr_lo: ", desc->base_addr_lo);
  print_int_nl(" desc->type: ", desc->type);
  print_int_nl(" desc->s: ", desc->s);
  print_int_nl(" desc->dpl: ", desc->dpl);
  print_int_nl(" desc->p: ", desc->p);
  print_u8_nl(" desc->seg_limit_hi: ", desc->seg_limit_hi);
  print_int_nl(" desc->avl: ", desc->avl);
  print_int_nl(" desc->l: ", desc->l);
  print_int_nl(" desc->d_b: ", desc->d_b);
  print_int_nl(" desc->g: ", desc->g);
  print_u8_nl(" desc->base_addr_hi: ", desc->base_addr_hi);
  print_u32_nl(" desc->seg_limit: ", GET_SEG_DESC_SEG_LIMIT(*desc));
  print_u64_nl(" desc->base_addr: ", GET_SEG_DESC_BASE_ADDR(*desc));
}
