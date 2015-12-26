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

MK_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
CUR_DIR := $(notdir $(patsubst %/,%,$(dir $(MK_PATH))))

CC_FLAVOR ?= x86_64
LK_FLAVOR ?= x86_64
BITNESS ?= 64

NASM=nasm
CC=$(CC_FLAVOR)-elf-gcc
LK=$(LK_FLAVOR)-elf-gcc

CFLAGS:=-m$(BITNESS) -std=c11 -ffreestanding -O2 -static -Wall -Wextra -nostdlib -nostartfiles -nodefaultlibs

.PHONY: all
all: kmain.elf kmain.gas.elf

kmain.elf: mb2.o boot.o kmain.o link.ld
	$(LK) $(CFLAGS) -Wl,-n,-T,link.ld -o $@ mb2.o boot.o kmain.o -lgcc
	objdump -x -d -s -mi386 $@ > $@.txt

kmain.gas.elf: mb2.o boot.gas.o kmain.o link.ld
	$(LK) $(CFLAGS) -Wl,-n,-T,link.ld -o $@ mb2.o boot.gas.o kmain.o -lgcc
	objdump -x -d -s -mi386 $@ > $@.txt

%.o: %.asm
	$(NASM) -felf64 $< -o $@
	objdump -x -d -s -mi386 $@ > $@.txt

%.o: %.S
	$(CC) $(CFLAGS) -c $< -o $@
	objdump -x -d -s -mi386 $@ > $@.txt

mb2.o: mb2.S

bootgas.o: bootgas.S

boot.o: boot.asm

kmain.o: kmain.c
	$(CC) $(CFLAGS) -c kmain.c -o kmain.o
	objdump -x -d -s -mi386 $@ > $@.txt

iso.img: kmain.elf grub.cfg
	mkdir -p isofiles/boot/grub
	cp kmain.elf isofiles/boot/
	cp grub.cfg isofiles/boot/grub
	grub-mkrescue -o $@ isofiles 2> /dev/null

iso.gas.img: kmain.gas.elf grub.gas.cfg
	mkdir -p isofiles.gas/boot/grub
	cp kmain.gas.elf isofiles.gas/boot/
	cp grub.gas.cfg isofiles.gas/boot/grub
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
	rm -rf boot.gas.o boot.gas.o.txt
	rm -rf boot.o boot.o.txt
	rm -rf mb2.o mb2.o.txt
	rm -rf kmain.o kmain.o.txt
	rm -rf kmain.elf kmain.elf.txt
	rm -rf kmain.gas.elf kmain.gas.elf.txt
	rm -rf iso.gas.img
	rm -rf iso.img
	rm -rf isofiles
	rm -rf isofiles.gas
