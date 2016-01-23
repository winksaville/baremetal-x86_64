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

_Static_assert(sizeof(struct descriptor_ptr) == 10,
    L"struct descriptor_ptr != 10");


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

/**
 * Interrupt or Trap Gate.
 *
 * See "Intel 64 and IA-32 Architectures Software Developer's Manual"
 * Volume 3A 6-16 6.14.1 "64-Bit Mode IDT"
 */
struct intr_trap_gate {
  u64 offset_lo:16;
  u64 segment:16;
  u64 ist:3;
  u64 unused_1:5;
  u64 type:4;
  u64 unused_2:1;
  u64 dpl:2;
  u64 p:1;
  u64 offset_hi:48;
  u64 unused_3:32;
} __attribute__((__packed__));

_Static_assert(sizeof(struct intr_trap_gate) == 16,
    L"struct intr_trap_gate is not 16 bytes");

typedef struct intr_trap_gate intr_trap_gate;

#define INITIALIZER_INTR_TRAP_GATE { \
   .offset_lo = 0, \
   .segment = 0, \
   .ist = 0, \
   .unused_1 = 0, \
   .type = 0, \
   .unused_2 = 0, \
   .dpl = 0, \
   .p = 0, \
   .offset_hi = 0, \
   .unused_3 = 0, \
}

/** Passed an integer i return the lower 16 bits */
#define GATE_OFFSET_LO(__i) (((__i) >> 0) & 0xFFFF)

/** Passed an integer i return the upper 48 bits */
#define GATE_OFFSET_HI(__i) (((__i) >> 16) & 0xFFFFFFFFFFFF)

/** Passed a gate return the offset as a uptr */
#define GET_GATE_OFFSET(__gate) \
  ((uptr)((((uptr)(__gate).offset_hi) << 16) | (uptr)((__gate).offset_lo)))

#endif
