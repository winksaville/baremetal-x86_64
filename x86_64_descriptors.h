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

typedef struct descriptor_ptr descriptor_ptr;

#endif
