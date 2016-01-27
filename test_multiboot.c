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
#include "abort.h"
#include "print.h"
#include "test_multiboot.h"


#pragma pack(push,1)
typedef struct multiboot_header_tag {
  u32 type;
  u32 size;
} multiboot_header_tag;
#pragma pack(pop)

multiboot_header_tag* print_tag(multiboot_header_tag *tag) {
  print_str("tag=");
  print_uptr(tag);
  print_str(" type=");
  print_u32(tag->type);
  print_str(" size=");
  print_int(tag->size);
  print_nl();

  void* next = (void*)((((uptr)tag + tag->size) + 7) & 0xfffffffffffffff8);
  if (next == tag) {
    print_str("Something is wrong next == tag returning NULL");
    print_nl();
    next = NULL;
    abort();
  }
  return (multiboot_header_tag*)next;
}

// [Multiboot 1.6](http://nongnu.askapache.com/grub/phcoder/multiboot.pdf) info.
// [OSDev.org Mulitboot2](http://wiki.osdev.org/Multiboot#Multiboot_2) info.
//
// The other critical information is what is the memory map at this point,
// [see Philipp's](http://os.phil-opp.com/entering-longmode.html) post. He
// initializes an identity mapping (1:1 physical to virtual) with one P4,
// one P3 and one P2. With each P2 entry pointing to 2MB of physical data.
// So the first 1G of physical RAM is mapped 1:1 with virtual memory.
//
// Previously he used 1GB pages there they are only supported in newer
// cpu's so he's nowusing the 2MB pages.

void test_multiboot(void* mb_info) {
  u32 total_size = *(u32*)mb_info;
  u32 reserved = *(u32*)(mb_info + 4);
  multiboot_header_tag* mb_end = (multiboot_header_tag*)(mb_info + total_size);
  multiboot_header_tag* tag = (multiboot_header_tag*)(mb_info + 8);

  print_uptr_nl("mb_info:    ", mb_info);
  print_int_nl( "total_size: ", total_size);
  print_int_nl( "reserved:   ", reserved);
  print_uptr_nl("mb_end:     ", mb_end);

  while ((tag < mb_end) && (tag->type != 0) && (tag->size != 8)) {
    multiboot_header_tag* next = print_tag(tag);
    if (next == NULL) break;

    if (tag->type == 1) {
      char* cmdline = (char*)tag + 8;
      print_str_str_nl("cmd_line=", cmdline);
    }
    tag = next;
  }
}
