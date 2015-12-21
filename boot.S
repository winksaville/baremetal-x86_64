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

  // Start up code
  .section .text
  .global _start
_start:
  movl  $stack_top, %esp  // Initialize stack
  call  kmain             // Call main

  // If kmain returns we hang
  cli                     // Turn off interrupts
hang:
  hlt                     // Halt
  jmp hang                // Loop if we unhalt

  // Stack
  .section .bootstrap_stack
  .skip 16384
stack_top:

