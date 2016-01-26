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

#ifndef DESCRIPTORS_X86_64_PRINT_H
#define DESCRIPTORS_X86_64_PRINT_H

#include "inttypes.h"
#include "descriptors_x86_64.h"

void print_intr_frame(char* str, intr_frame* frame);

void print_gate(char *str, intr_trap_gate* gate);

void print_tss_ldt_desc(char *str, tss_ldt_desc* desc);

void print_seg_desc(char *str, seg_desc* desc);

#endif
