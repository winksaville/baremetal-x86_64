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

CC=$(CC_FLAVOR)-unknown-elf-gcc
LK=$(LK_FLAVOR)-unknown-elf-gcc

CFLAGS:=-m$(BITNESS) -std=c11 -ffreestanding -O2 -g -static -Wall -Wextra -nostdlib -nostartfiles -nodefaultlibs

kmain.elf: mb2.o boot.o kmain.o link.ld
	$(LK) $(CFLAGS) -Wl,-n,-T,link.ld -o $@ mb2.o boot.o kmain.o -lgcc

%.o: %.S
	$(CC) $(CFLAGS) -c $< -o $@

mb2.o: mb2.S

boot.o: boot.S

kmain.o: kmain.c
	$(CC) $(CFLAGS) -c kmain.c -o kmain.o

iso.img: kmain.elf grub.cfg
	mkdir -p isofiles/boot/grub
	cp kmain.elf isofiles/boot/
	cp grub.cfg isofiles/boot/grub
	grub-mkrescue -o $@ isofiles 2> /dev/null

.PHONY: run
run: iso.img
	qemu-system-x86_64 -nographic -no-reboot -drive format=raw,file=$<

#NOT WORKING, I couldn't set a break point. Since
# grub is used to load my file something else has
# to happen?
.PHONY: dbg
dbg: iso.img
	qemu-system-x86_64 -s -S -nographic -no-reboot -drive format=raw,file=$<

.PHONY: clean
clean:
	rm -rf mb2.o boot.o kmain.o kmain.elf iso.img isofiles
