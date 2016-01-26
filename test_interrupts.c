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
#include "interrupts.h"
#include "descriptors_x86_64_print.h"

void test_interrupts() {
  descriptor_ptr desc_ptr = {
    .limit = 0x1234,
    .address = 0x1234567812345678,
  };

  print_int_nl("0: ", 0);
  print_int_nl("1: ", 1);
  print_int_nl("7F..FF: ", 0x7FFFFFFFFFFFFFFFLL);
  print_int_nl("FF..FF: ", 0xFFFFFFFFFFFFFFFFLL);
  print_int_nl("-1: ", -1);

  print_int_nl("sizeof(desc_ptr): ", sizeof(desc_ptr));
  print_u16_nl("desc_ptr.limit: ", desc_ptr.limit);
  print_u64_nl("desc_ptr.address: ", desc_ptr.address);
  load_idtr(&desc_ptr);

  descriptor_ptr idtr;
  store_idtr(&idtr);
  print_u16_nl("idtr.limit: ", idtr.limit);
  print_u64_nl("idtr.address: ", idtr.address);

  if (idtr.limit != desc_ptr.limit) {
    print_str_nl("ERROR desc_ptr.limit != idtr.limit");
  }
  if (idtr.address != desc_ptr.address) {
    print_str_nl("ERROR desc_ptr.address != idtr.address");
  }

  descriptor_ptr gdtr;
  store_gdtr(&gdtr);
  print_u16_nl("gdtr.limit: ", gdtr.limit);
  print_u64_nl("gdtr.address: ", gdtr.address);

  uptr	offset = 0x1234567812345678;
  intr_trap_gate gate = INITIALIZER_INTR_TRAP_GATE;
  print_int_nl("sizeof(gate): ", sizeof(gate));
  print_gate("gate default initialization:", &gate);

  gate.offset_lo = GATE_OFFSET_LO(offset);
  gate.segment = 0x1234;
  gate.ist = 0x7;
  gate.type = 0xF;
  gate.dpl = 0x3;
  gate.p = 0x1;
  gate.offset_hi = GATE_OFFSET_HI(offset);
  print_gate("gate specific initialization:", &gate);

  initialize_intr_trap_table();
  print_str_nl("idtr after initialization");
  store_idtr(&idtr);
  print_u16_nl(" idtr.limit: ", idtr.limit);
  print_uptr_nl(" idtr.itg: ", idtr.itg);
}
