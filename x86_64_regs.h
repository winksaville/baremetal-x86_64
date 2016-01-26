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

#ifndef X86_64_REGS_H
#define X86_64_REGS_H

#include "inttypes.h"

/** Write data segment */
static __inline__ void write_x86_ds(u16 ds) {
  __asm__ volatile("movw %0, %%ds;" :: "r" (ds));
}

/** Read data segment */
static __inline__ u16 read_x86_ds(void) {
  u16 ds;
  __asm__ volatile("movw %%ds, %0;" : "=r" (ds));
  return ds;
}

/** Write stack segment */
static __inline__ void write_x86_ss(u16 ss) {
  __asm__ volatile("movw %0, %%ss;" :: "r" (ss));
}

/** Read stack segment */
static __inline__ u16 read_x86_ss(void) {
  u16 ss;
  __asm__ volatile("movw %%ss, %0;" : "=r" (ss));
  return ss;
}

/** Write extra segment */
static __inline__ void write_x86_es(u16 es) {
  __asm__ volatile("movw %0, %%es;" :: "r" (es));
}

/** Read extra segment */
static __inline__ u16 read_x86_es(void) {
  u16 es;
  __asm__ volatile("movw %%es, %0;" : "=r" (es));
  return es;
}


#endif
