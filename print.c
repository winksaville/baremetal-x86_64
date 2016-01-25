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

#include "inttypes.h"
#include "print.h"

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

static u8* u4_2_ascii = (u8*)"0123456789ABCDEF";

static inline void outb(u16 port, u8 val) {
    __asm volatile ( "outb %0, %1" : : "a"(val), "d"(port) );
}

void putchar(char ch) {
  outb(COM_TX_FIFO, ch);
}

void print_nl() {
  putchar('\n');
}

void print_str(char* s) {
  u8 ch;
  while((ch = *s++) != 0) {
    putchar(ch);
  }
}

void print_int(long long v) {
  if (v == 0) {
    putchar('0');
  } else {
    // A buffer large enough to hold a 2^64 integer
    // converted to decaimal without a trailing 0
    // calculated using  roundup(log(2^64))
    char buff[20];
    unsigned int i = 0;
    int negative = 0;

    if (v < 0) {
      negative = 1;
      v = -v;
    }
    while (i < sizeof(buff) && (v > 0)) {
      buff[i++] = u4_2_ascii[v % 10];
      v /= 10;
    }
    if (negative) {
      putchar('-');
    }
    while (i > 0) {
      putchar(buff[--i]);
    }
  }
}

void print_u8(u8 v) {
  for (unsigned int i = 0; i < (sizeof(v) * 2); i++) {
    putchar(u4_2_ascii[(v & 0xF0) >> 4]);
    v <<= 4;
  }
}

void print_u16(u16 v) {
  for (unsigned int i = 0; i < (sizeof(v) * 2); i++) {
    putchar(u4_2_ascii[(v & 0xF000) >> 12]);
    v <<= 4;
  }
}

void print_u32(u32 v) {
  for (unsigned int i = 0; i < (sizeof(v) * 2); i++) {
    putchar(u4_2_ascii[(v & 0xF0000000) >> 28]);
    v <<= 4;
  }
}

void print_u64(u64 v) {
  print_u32((v >> 32) & 0xFFFFFFFF);
  print_u32(v & 0xFFFFFFFF);
}

void print_uptr(void* p) {
  print_u64((u64)p);
}

void print_str_nl(char *str) {
  print_str(str);
  print_nl();
}

void print_str_str_nl(char *s, char *str) {
  print_str(s);
  print_str(str);
  print_nl();
}


void print_int_nl(char *s, long long v) {
  print_str(s);
  print_int(v);
  print_nl();
}

void print_u8_nl(char *s, u8 v) {
  print_str(s);
  print_u8(v);
  print_nl();
}

void print_u16_nl(char *s, u16 v) {
  print_str(s);
  print_u16(v);
  print_nl();
}

void print_u32_nl(char *s, u32 v) {
  print_str(s);
  print_u32(v);
  print_nl();
}

void print_u64_nl(char *s, u64 v) {
  print_str(s);
  print_u64(v);
  print_nl();
}

void print_uptr_nl(char *s, void* v) {
  print_str(s);
  print_uptr(v);
  print_nl();
}

void print_u8s(u8* s, u32 len) {
  print_uptr(s);
  print_str(": ");
  while(len-- > 0) {
    print_u8(*s++);
    putchar(' ');
  }
  print_nl();
}
