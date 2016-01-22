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

#ifndef X86_64_DESCRIPTORS_h
#define X86_64_DESCRIPTORS_h

#include "inttypes.h"

/* Pointer to Global Descriptor Table  Interrupt Descriptor Table */
struct descriptor_ptr {
  u16 limit;
  u64 address;
} __attribute__((__packed__));


/** Descriptor Pointer typedef */
typedef struct descriptor_ptr descriptor_ptr;

/** Load the IDT register from desc_ptr */
static __inline__ void x86_lidt(descriptor_ptr *desc_ptr) {
  __asm__ volatile("lidt %0" :: "m" (*desc_ptr));
}

/** Store the IDT register to desc_ptr */
static __inline__ void x86_sidt(descriptor_ptr *desc_ptr) {
  __asm__ volatile("sidt %0" : "=m" (*desc_ptr));
}

/** Load the GDT register from desc_ptr */
static __inline__ void x86_lgdt(descriptor_ptr *desc_ptr) {
  __asm__ volatile("lgdt %0" :: "m" (*desc_ptr));
}

/** Store the GDT register to desc_ptr */
static __inline__ void x86_sgdt(descriptor_ptr *desc_ptr) {
  __asm__ volatile("sgdt %0" : "=m" (*desc_ptr));
}


#endif
