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

/**
 * Interrupt stack frame
 *
 * This is the signature required for the compiler so
 * intr_handler and expt_handler are can be marked with
 * the __attribute__((__interrupt__))
 */
typedef struct intr_frame {
  u64 ip;
  u64 cs;
  u64 flags;
  u64 sp;
  u64 ss;
} intr_frame;

typedef void (intr_handler)(struct intr_frame* frame);

typedef void (expt_handler) (struct intr_frame* frame, u64 error_code);

void set_intr_handler(u32 intr_num, intr_handler ih);

void set_expt_handler(u32 intr_num, expt_handler eh);

void initialize_intr_trap_table();

#endif
