/*
 * Copyright 2015 Wink Saville
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
#include "gdt.h"
#include "regs_x86_64.h"
#include "print.h"
#include "descriptors_x86_64_print.h"
#include "test_multiboot.h"
#include "test_interrupts.h"

void test_print() {
  print_int_nl("0: ", 0);
  print_int_nl("1: ", 1);
  print_int_nl("7F..FF: ", 0x7FFFFFFFFFFFFFFFLL);
  print_int_nl("FF..FF: ", 0xFFFFFFFFFFFFFFFFLL);
  print_int_nl("-1: ", -1);
}

void test_registers() {
  print_u16_nl("ds=", read_ds());
  print_u16_nl("ss=", read_ss());
  print_u16_nl("es=", read_es());
  print_u16_nl("tr=", read_tr());

  write_ds(0x00);
  write_ss(0x00);
  write_es(0x00);
  write_tr(0x00);

  print_u16_nl("ds=", read_ds());
  print_u16_nl("ss=", read_ss());
  print_u16_nl("es=", read_es());
  print_u16_nl("tr=", read_tr());
}

__attribute__ ((__noreturn__))
void kmain(void* mb_info) {
  test_print();

  test_multiboot(mb_info);

  test_registers();

  initialize_gdt();

  test_interrupts();

  print_nl();
  print_str_nl("SUCCESS");
  abort();
}
