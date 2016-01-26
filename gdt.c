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

/* Global Descriptor Table */
struct global_descriptor_table {
  seg_desc unused;
  seg_desc code;
  seg_desc data;
  tss_ldt_desc tss;
} __attribute__((__packed__, __aligned__(8)));

typedef struct global_descriptor_table global_descriptor_table;

_Static_assert(sizeof(global_descriptor_table) == 40,
    L"struct global_descriptor_table != 40");

static global_descriptor_table gdt;

void initialize_gdt() {
  print_uptr_nl("&gdt=", &gdt);
  print_int_nl("sizeof(global_descriptor_table)=",
      sizeof(global_descriptor_table));

  set_seg_desc(&gdt.unused, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
}
