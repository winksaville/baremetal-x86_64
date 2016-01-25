# Copyright 2015 wink saville
#
# licensed under the apache license, version 2.0 (the "license");
# you may not use this file except in compliance with the license.
# you may obtain a copy of the license at
#
#     http://www.apache.org/licenses/license-2.0
#
# unless required by applicable law or agreed to in writing, software
# distributed under the license is distributed on an "as is" basis,
# without warranties or conditions of any kind, either express or implied.
# see the license for the specific language governing permissions and
# limitations under the license.

# Turn off builtin implicit rules
.SUFFIXES:

MK_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
CUR_DIR := $(notdir $(patsubst %/,%,$(dir $(MK_PATH))))

CC_FLAVOR ?= x86_64
LK_FLAVOR ?= x86_64
BITNESS ?= 64

NASM=nasm
CC=$(CC_FLAVOR)-elf-gcc
LK=$(LK_FLAVOR)-elf-gcc

# See [System V ABI at OSDev.org](http://wiki.osdev.org/System_V_ABI) it says:
#
# "The solution is to build all kernel code with -mno-red-zone or by handling
# interrupts in kernel mode on another stack"
#
# Here I'm choosing to use -mno-red-zone.
# Other info at OSDev.org you do a
# [site search](https://www.google.com/search?q=site%3Aosdev.org%20%22red%20zone%22&rct=j)
# Another good article from
# [Eric Kidd](http://www.randomhacks.net/2015/11/11/bare-metal-rust-custom-target-kernel-space/)
# Eric also notes that Floating Point registers must be preserved properly
# for now its assumed that there is no floating.
CFLAGS:=-m$(BITNESS) -std=c11 -ffreestanding -O2 -mno-red-zone -static -Wall -Wextra -nostdlib -nostartfiles -nodefaultlibs

OBJ_FILES:=mb2.o kmain.o print.o test_multiboot.o test_interrupts.o

.PHONY: all
all: kmain.elf kmain.gas.elf

kmain.elf: boot.o $(OBJ_FILES) link.ld
	$(LK) $(CFLAGS) -Wl,-n,-T,link.ld -o $@ boot.o $(OBJ_FILES) -lgcc
	objdump -x -d -s -mi386 $@ > $@.txt

kmain.gas.elf: boot.gas.o $(OBJ_FILES) link.ld
	$(LK) $(CFLAGS) -Wl,-n,-T,link.ld -o $@ boot.gas.o $(OBJ_FILES) -lgcc
	#objdump -x -d -s -mi386 $@ > $@.txt
	objdump -x -d -s $@ > $@.txt

%.o: %.asm
	$(NASM) -felf64 $< -o $@

%.o: %.S
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
	objdump -d $@ > $@.txt

mb2.o: mb2.S

boot.gas.o: boot.gas.S

boot.o: boot.asm

print.o: print.c inttypes.h print.h

test_multiboot.o: test_multiboot.c test_multiboot.h inttypes.h print.h

test_interrupts.o: test_interrupts.c x86_64_descriptors.h inttypes.h test_interrupts.h print.h

kmain.o: kmain.c x86_64_regs.h x86_64_descriptors.h inttypes.h test_multiboot.h test_interrupts.h print.h

iso.img: kmain.elf grub.cfg
	mkdir -p isofiles/boot/grub
	cp kmain.elf isofiles/boot/
	cp grub.cfg isofiles/boot/grub
	grub-mkrescue -o $@ isofiles 2> /dev/null

iso.gas.img: kmain.gas.elf grub.gas.cfg
	mkdir -p isofiles.gas/boot/grub
	cp kmain.gas.elf isofiles.gas/boot/
	cp grub.gas.cfg isofiles.gas/boot/grub/grub.cfg
	grub-mkrescue -o $@ isofiles.gas 2> /dev/null

.PHONY: run.gas
run.gas: iso.gas.img
	qemu-system-x86_64 -nographic -no-reboot -drive format=raw,file=$<

.PHONY: run
run: iso.img
	qemu-system-x86_64 -nographic -no-reboot -drive format=raw,file=$<

#NOT WORKING, I couldn't set a break point. Since
# grub is used to load my file something else has
# to happen?
.PHONY: dbg
dbg: iso.img
	qemu-system-x86_64 -s -S -nographic -no-reboot -drive format=raw,file=$<

.PHONY: dbg.gas
dbg.gas: iso.gas.img
	qemu-system-x86_64 -s -S -nographic -no-reboot -drive format=raw,file=$<

.PHONY: clean
clean:
	rm -rf $(OBJ_FILES)
	rm -rf *.o
	rm -rf *.elf
	rm -rf *.o.txt
	rm -rf *.elf.txt
	rm -rf *.img
	rm -rf isofiles
	rm -rf isofiles.gas
