/*
 * Copyright 2015 Wink Saville
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

  // [Mulitboot 2 header](http://nongnu.askapache.com/grub/phcoder/multiboot.pdf)
  .set MAGIC, 0xE85250D6          // Mulitboot 2 magic number
  .set FLAGS, 0                   // 0 is i386 protected mode
  .set CHECKSUM, -(MAGIC + FLAGS + (mbh_end - mbh_start)) // Checksum

  // The multiboot header
  .section .multiboot
  .align 4
mbh_start:
  .long MAGIC
  .long FLAGS
  .long mbh_end - mbh_start
  .long CHECKSUM

  // End tag
  .word 0                         // type
  .word 0                         // flags
  .long 8                         // size
mbh_end:

