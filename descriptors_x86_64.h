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

#ifndef DESCRIPTORS_X86_64_h
#define DESCRIPTORS_X86_64_h

#include "inttypes.h"

/* Pointer to Global Descriptor Table  Interrupt Descriptor Table */
struct descriptor_ptr {
  u16 limit;
  uptr address;
} __attribute__((__packed__));

_Static_assert(sizeof(struct descriptor_ptr) == 10,
    L"struct descriptor_ptr != 10");

/** Descriptor Pointer typedef */
typedef struct descriptor_ptr descriptor_ptr;

/** Load the IDT register from desc_ptr */
static __inline__ void load_idtr(descriptor_ptr* desc_ptr) {
  __asm__ volatile("lidt %0" :: "m" (*desc_ptr));
}

/** Store the IDT register to desc_ptr */
static __inline__ void store_idtr(descriptor_ptr* desc_ptr) {
  __asm__ volatile("sidt %0" : "=m" (*desc_ptr));
}

/** Load the GDT register from desc_ptr */
static __inline__ void load_gdtr(descriptor_ptr* desc_ptr) {
  __asm__ volatile("lgdt %0" :: "m" (*desc_ptr));
}

/** Store the GDT register to desc_ptr */
static __inline__ void store_gdtr(descriptor_ptr* desc_ptr) {
  __asm__ volatile("sgdt %0" : "=m" (*desc_ptr));
}

/**
 * Interrupt or Trap Gate.
 *
 * See "Intel 64 and IA-32 Architectures Software Developer's Manual"
 * Volume 3 chapter 6.14.1 "64-Bit Mode IDT"
 * Figure 6-7. 64-Bit IDT Gate Descriptors
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

/** Return the bits for intr_trap_gate.offset_lo */
#define GATE_OFFSET_LO(addr) ({ \
  u64 r = ((u64)(addr) >> 0) & 0xFFFF; \
  r; \
})

/** Return the bits for intr_trap_gate.offset_hi */
#define GATE_OFFSET_HI(addr) ({ \
  u64 r = ((u64)(addr) >> 16) & 0xFFFFFFFFFFFFLL; \
  r; \
})

/** Return the intr_trap_gate.offset as a uptr */
#define GET_GATE_OFFSET(gate) ({ \
  uptr r = (uptr)((((uptr)(gate).offset_hi) << 16) | (uptr)((gate).offset_lo)); \
  r; \
})


/**
 * Desctriptor Types for 64 bit IA32e
 *
 * See "Intel 64 and IA-32 Architectures Software Developer's Manual"
 * Volume 3 chapter 3.5 "Segment Descriptor Types"
 * Table 3-2. System-Segment and Gate-Descriptor Types
 */
enum {
  DT_64_UPPER =      0,
  DT_64_RESV_1 =     1,
  DT_64_LDT =        2,
  DT_64_RESV_3 =     3,
  DT_64_RESV_4 =     4,
  DT_64_RESV_5 =     5,
  DT_64_RESV_6 =     6,
  DT_64_RESV_7 =     7,
  DT_64_RESV_8 =     8,
  DT_64_TSS_AVAIL =  9,
  DT_64_RESV_10 =   10,
  DT_64_TSS64_BUSY= 11,
  DT_64_CALL_GATE = 12,
  DT_64_RESV_13 =   13,
  DT_64_INTR_GATE = 14,
  DT_64_TRAP_GATE = 15,
};

/**
 * Segment Descriptor
 *
 * See "Intel 64 and IA-32 Architectures Software Developer's Manual"
 * Volume 3 chapter 3.4.5 "Segment Descriptors"
 * Figure 3-8. Segment Descriptor
 */
struct seg_desc {
  u64 seg_limit_lo:16;
  u64 base_addr_lo:24;
  u64 type:4;
  u64 s:1;
  u64 dpl:2;
  u64 p:1;
  u64 seg_limit_hi:4;
  u64 avl:1;
  u64 l:1;
  u64 d_b:1;
  u64 g:1;
  u64 base_addr_hi:8;
} __attribute__((__packed__));

_Static_assert(sizeof(struct seg_desc) == 8,
    L"segment_descriptor not 8 bytes");

typedef struct seg_desc seg_desc;

#define INITIALIZER_SEG_DESC { \
  .seg_limit_lo = 0, \
  .base_addr_lo = 0, \
  .type = 0, \
  .s = 0, \
  .dpl = 0, \
  .p = 0, \
  .seg_limit_hi = 0, \
  .avl = 0, \
  .l = 0, \
  .d_b = 0, \
  .g = 0, \
  .base_addr_hi = 0, \
}

/** Return the bits for seg_desc.seg_limit_lo */
#define SEG_DESC_SEG_LIMIT_LO(i) ({ \
  u64 r = (((u64)(i) >> 0) & 0xFFFF); \
  r; \
})

/** Return the bits for seg_desc.seg_limit_hi */
#define SEG_DESC_SEG_LIMIT_OFFSET_HI(addr) ({ \
  u64 r = ((u64)(addr) >> 16) & 0xFFFFFFFFFFFFLL; \
  r; \
})

/** Return seg_desc.seg_limit as a u64 */
#define GET_SEG_DESC_SEG_LIMIT(sd) ({ \
  u64 r = (u64)((((uptr)(sd).seg_limit_hi) << 16) | (u64)((sd).seg_limit_lo)); \
  r; \
})

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

void setidt_intr(u64 idx, intr_handler ih);

void setidt_expt(u64 idx, expt_handler eh);

#endif