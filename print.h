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

#ifndef PRINT_H
#define PRINT_H

#include "inttypes.h"

// From [here](//http://www.lammertbies.nl/comm/info/RS-232_io.html)
#define COM1_BASE 0x3F8
#define COM2_BASE 0x2F8
#define COM3_BASE 0x3E8
#define COM4_BASE 0x2E8

#define COM1_IRQ 4
#define COM2_IRQ 3
#define COM3_IRQ 4
#define COM4_IRQ 3

// and [here](http://www.lammertbies.nl/comm/info/serial-uart.html)
#define RX_FIFO 0
#define TX_FIFO 0

#define COM_TX_FIFO ((short int)(COM1_BASE + TX_FIFO))

static inline void outb(u16 port, u8 val) {
    __asm volatile ( "outb %0, %1" : : "a"(val), "d"(port) );
}

static inline void putchar(char ch) {
  outb(COM_TX_FIFO, ch);
}

void putchar(char ch);

void print_nl();

void print_str(char* s);

void print_int(long long v);

void print_u8(u8 v);

void print_u16(u16 v);

void print_u32(u32 v);

void print_u64(u64 v);

void print_uptr(void* p);

void print_str_nl(char *str);

void print_str_str_nl(char *s, char *str);

void print_int_nl(char *s, long long v);

void print_u8_nl(char *s, u8 v);

void print_u16_nl(char *s, u16 v);

void print_u32_nl(char *s, u32 v);

void print_u64_nl(char *s, u64 v);

void print_uptr_nl(char *s, void* v);

void print_u8s(u8* s, u32 len);

#endif
