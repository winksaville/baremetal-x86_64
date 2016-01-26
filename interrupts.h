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

#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "inttypes.h"
#include "descriptors_x86_64.h"

/* Interrupt Descriptor Table */
#define IDT_COUNT 256
extern intr_trap_gate idt[IDT_COUNT];

void intr_undefined(struct intr_frame *frame);

void expt_undefined(struct intr_frame *frame, u64 error_code);

void expt_invalid_opcode(struct intr_frame *frame, u64 error_code);

void initialize_intr_trap_table();

#endif
