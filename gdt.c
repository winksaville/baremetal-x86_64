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
#include "descriptors_x86_64_print.h"

/* Global Descriptor Table */
struct global_descriptor_table {
  seg_desc zero;
  seg_desc code;
  seg_desc data;
  //tss_ldt_desc tss;
} __attribute__((__packed__, __aligned__(8)));

typedef struct global_descriptor_table global_descriptor_table;

_Static_assert(sizeof(global_descriptor_table) == 24,
    L"struct global_descriptor_table != 24");

static global_descriptor_table gdt;

/**
 * Initialize GDT for a flat memory model.
 *
 * See "Intel 64 and IA-32 Architectures Software Developer's Manual"
 * Volume 3 chapter 9.8.1 "Protected-Mode System Data Structures"
 */
void initialize_gdt() {
  // Print the current values
  descriptor_ptr desc_ptr;
  print_uptr_nl("&desc_ptr=", &desc_ptr);
  print_uptr_nl("&desc_ptr.limit=", (void*)&desc_ptr.limit);
  print_uptr_nl("&desc_ptr.address=", (void*)&desc_ptr.address);

  // Dump current gdt
  store_gdtr(&desc_ptr);
  print_u16_nl("cur gdt limit=", desc_ptr.limit);
  print_u64_nl("cur gdt address=", desc_ptr.address);
  print_seg_desc("gdt[0] zero:", &desc_ptr.sd[0]);
  print_seg_desc("gdt[1] code seg:", &desc_ptr.sd[1]);
  print_seg_desc("gdt[2] data seg:", &desc_ptr.sd[2]);

  /*
   * Initialize new gdt
   */
  print_uptr_nl("&gdt=", &gdt);
  print_int_nl("sizeof(global_descriptor_table)=",
      sizeof(global_descriptor_table));

  // Zero the first entry.
  set_seg_desc(&gdt.zero, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  print_seg_desc("gdt.zero:", &gdt.zero);
  if (cmp_seg_desc(&gdt.zero, &desc_ptr.sd[0]) != 0) {
    print_str_nl("gdt.zero != desc_ptr.sd[0]");
    abort();
  }

  // Initialize the code segment
  set_code_seg_desc(&gdt.code, 0, 1, 0, 0, 1, 0, 1, 0, 0);
  print_seg_desc("gdt.code:", &gdt.code);
  if (cmp_seg_desc(&gdt.code, &desc_ptr.sd[1]) != 0) {
    print_str_nl("gdt.code != desc_ptr.sd[1]");
    abort();
  }

  // Initialize the data segment
  set_data_seg_desc(&gdt.data, 0, 1, 0, 0, 1, 0, 0, 0, 0);
  print_seg_desc("gdt.data:", &gdt.data);
  if (cmp_seg_desc(&gdt.data, &desc_ptr.sd[2]) == 0) {
    print_str_nl(
        "ERROR: gdt.data == desc_ptr.sd[2], accessed should be different");
    abort();
  }

  // Initialize the tss segment
  //set_tss_ldt_desc(&gdt.tss, 0, 0, 0, 0, 0, 0, 0);
  //print_tss_ldt_desc("gdt.tss:", &gdt.tss);

  // Update to new gdt
  desc_ptr.limit = sizeof(gdt) - 1;
  desc_ptr.sd = &gdt.zero;
  load_gdtr(&desc_ptr);
  store_gdtr(&desc_ptr);
  print_u16_nl("new gdt limit=", desc_ptr.limit);
  print_u64_nl("new gdt address=", desc_ptr.address);

  if (cmp_seg_desc(&gdt.data, &desc_ptr.sd[2]) != 0) {
    print_str_nl("ERROR: gdt.code != desc_ptr.sd[1]");
    print_seg_desc("gdt.data:", &gdt.data);
    abort();
  }
}
