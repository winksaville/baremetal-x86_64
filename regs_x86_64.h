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

#ifndef REGS_X86_64_H
#define REGS_X86_64_H

#include "inttypes.h"
#include "descriptors_x86_64.h"

/** Load the GDT register from desc_ptr */
static __inline__ void load_gdtr(descriptor_ptr* desc_ptr) {
  u16* p = (u16*)&desc_ptr->limit;
  __asm__ volatile("lgdt %0" :: "m" (*p));
}

/** Store the GDT register to desc_ptr */
static __inline__ void store_gdtr(descriptor_ptr* desc_ptr) {
  u16* p = (u16*)&desc_ptr->limit;
  __asm__ volatile("sgdt %0" : "=m" (*p));
}

/** Load the LDT register from desc_ptr */
static __inline__ void load_ldtr(descriptor_ptr* desc_ptr) {
  u16* p = (u16*)&desc_ptr->limit;
  __asm__ volatile("lldt %0" :: "m" (*p));
}

/** Store the LDT register to desc_ptr */
static __inline__ void store_ldtr(descriptor_ptr* desc_ptr) {
  u16* p = (u16*)&desc_ptr->limit;
  __asm__ volatile("sldt %0" : "=m" (*p));
}

/** Load the IDT register from desc_ptr */
static __inline__ void load_idtr(descriptor_ptr* desc_ptr) {
  u16* p = (u16*)&desc_ptr->limit;
  __asm__ volatile("lidt %0" :: "m" (*p));
}

/** Store the IDT register to desc_ptr */
static __inline__ void store_idtr(descriptor_ptr* desc_ptr) {
  u16* p = (u16*)&desc_ptr->limit;
  __asm__ volatile("sidt %0" : "=m" (*p));
}

/** Write Task Register which is a 16 bit selector into the GDT */
static __inline__ void write_tr(u16 selector) {
  __asm__ volatile("ltr %0;" :: "r" (selector));
}

/** read Task Register which is a 16 bit selector into the GDT */
static __inline__ u16 read_tr(void) {
  u16 tr;
  __asm__ volatile("str %0;" : "=r" (tr));
  return tr;
}

/** Write data segment */
static __inline__ void write_ds(u16 ds) {
  __asm__ volatile("movw %0, %%ds;" :: "r" (ds));
}

/** Read data segment */
static __inline__ u16 read_ds(void) {
  u16 ds;
  __asm__ volatile("movw %%ds, %0;" : "=r" (ds));
  return ds;
}

/** Write stack segment */
static __inline__ void write_ss(u16 ss) {
  __asm__ volatile("movw %0, %%ss;" :: "r" (ss));
}

/** Read stack segment */
static __inline__ u16 read_ss(void) {
  u16 ss;
  __asm__ volatile("movw %%ss, %0;" : "=r" (ss));
  return ss;
}

/** Write extra segment */
static __inline__ void write_es(u16 es) {
  __asm__ volatile("movw %0, %%es;" :: "r" (es));
}

/** Read extra segment */
static __inline__ u16 read_es(void) {
  u16 es;
  __asm__ volatile("movw %%es, %0;" : "=r" (es));
  return es;
}

/** get stack pointer */
static __inline__ u64 get_rsp(void) {
  u64 sp;
  __asm__ volatile("movq %%rsp, %0;" : "=r" (sp));
  return sp;
}

/** get msr */
static __inline__ u64 get_msr(u32 msr) {
  u32 lo;
  u32 hi;
  __asm__ volatile("movl  %0, %%ecx\n" :: "g"(msr));
  __asm__ volatile("rdmsr\n" : "=d"(hi), "=a"(lo));
  return (u64)hi << 32 | (u64)lo;
}

/** int instruction */
static __inline__ void intr(u8 num) {
  __asm__ volatile("int %0;" :: "i"(num));
}

/** int instruction */
static __inline__ void iretq(void) {
  __asm__ volatile("iretq");
}

/** hlt, halt instruction */
static __inline void hlt(void) {
  __asm__ volatile("hlt");
}

/** cli, disable interrupts */
static __inline void cli(void) {
  __asm__ volatile("cli");
}

/** sti, enable interrupts */
static __inline void sti(void) {
  __asm__ volatile("sti");
}

#endif
