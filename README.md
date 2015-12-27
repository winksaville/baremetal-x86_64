# baremetal-x86_64

A a minimal bare metal program that runs on qemu-system-x86_64
it outputs "OK\n" to a COM1 at 0x3f8. This is based on Philipp
Opperman's blog_os initialization code I've ported to [baremetal-po-x86_64]
(https://github.com/winksaville/baremetal-po-x86_64).
The big difference is I'm porting as much as possible to C.

Philipp also references Remzi H. and Andrea C. Arpaci-Dusseau book
[Operating Systems:Three Easy Pieces](http://pages.cs.wisc.edu/~remzi/OSTEP/)

License
---
[Apache 2.0 license](http://www.apache.org/licenses/).

Sources
---
Get the sources for this project from github.
```
mkdir -p ~/prgs
cd ~/prgs
git clone https://github.com/winksaville/baremetal-x86_64.git
cd baremetal-x86_64
```

Create crosstool chain using crosstool-ng.
---
Modified as desired, config.x86_64-pc-elf builds a static
cross tool chain with no libc for baremetal. Prior to running
the commands below you'll want to add the following to
your $PATH:
 - $HOME/opt/bin
 - $HOME/opt/x-tools/x86_64-unknown-elf/bin
```
git clone https://github.com/crosstool-ng/crosstool-ng
cd crosstool-ng
./bootstrap
./configure --prefix=$HOME/opt
make
make install
cd ..
mkdir -p x86_64-elf
cd x86_64-elf
cp ../config.x86_64-unknown-elf .config
ct-ng build
cd ..
```

Build and run nasm version
---
```
make run
```
The output is:
```
x86_64-elf-gcc -m64 -std=c11 -ffreestanding -O2 -static -Wall -Wextra -nostdlib -nostartfiles -nodefaultlibs -c mb2.S -o mb2.o
objdump -x -d -s -mi386 mb2.o > mb2.o.txt
nasm -felf64 boot.asm -o boot.o
objdump -x -d -s -mi386 boot.o > boot.o.txt
x86_64-elf-gcc -m64 -std=c11 -ffreestanding -O2 -static -Wall -Wextra -nostdlib -nostartfiles -nodefaultlibs -c kmain.c -o kmain.o
objdump -x -d -s -mi386 kmain.o > kmain.o.txt
x86_64-elf-gcc -m64 -std=c11 -ffreestanding -O2 -static -Wall -Wextra -nostdlib -nostartfiles -nodefaultlibs -Wl,-n,-T,link.ld -o kmain.elf mb2.o boot.o kmain.o -lgcc
objdump -x -d -s -mi386 kmain.elf > kmain.elf.txt
mkdir -p isofiles/boot/grub
cp kmain.elf isofiles/boot/
cp grub.cfg isofiles/boot/grub
grub-mkrescue -o iso.img isofiles 2> /dev/null
qemu-system-x86_64 -nographic -no-reboot -drive format=raw,file=iso.img
mb_info:    0000000000100780
total_size: 00000490
reserved:   00000000
mb_end:     0000000000100C10
tag=0000000000100788 type=00000001 size=00000021
cmd_line=kmain.elf 1234 5678 9ABC
tag=00000000001007B0 type=00000002 size=00000018
tag=00000000001007C8 type=0000000A size=0000001C
tag=00000000001007E8 type=00000006 size=000000A0
tag=0000000000100888 type=00000009 size=00000314
tag=0000000000100BA0 type=00000004 size=00000010
tag=0000000000100BB0 type=00000005 size=00000014
tag=0000000000100BC8 type=00000008 size=00000020
tag=0000000000100BE8 type=0000000E size=0000001C
```

Build and run gas version
---
```
make run.gas
```
The output is:
```
x86_64-elf-gcc -m64 -std=c11 -ffreestanding -O2 -static -Wall -Wextra -nostdlib -nostartfiles -nodefaultlibs -c boot.gas.S -o boot.gas.o
objdump -x -d -s -mi386 boot.gas.o > boot.gas.o.txt
x86_64-elf-gcc -m64 -std=c11 -ffreestanding -O2 -static -Wall -Wextra -nostdlib -nostartfiles -nodefaultlibs -Wl,-n,-T,link.ld -o kmain.gas.elf mb2.o boot.gas.o kmain.o -lgcc
objdump -x -d -s -mi386 kmain.gas.elf > kmain.gas.elf.txt
mkdir -p isofiles.gas/boot/grub
cp kmain.gas.elf isofiles.gas/boot/
cp grub.gas.cfg isofiles.gas/boot/grub/grub.cfg
grub-mkrescue -o iso.gas.img isofiles.gas 2> /dev/null
qemu-system-x86_64 -nographic -no-reboot -drive format=raw,file=iso.gas.img
mb_info:    0000000000100818
total_size: 00000490
reserved:   00000000
mb_end:     0000000000100CA8
tag=0000000000100820 type=00000001 size=00000025
cmd_line=kmain.gas.elf 1234 5678 9ABC
tag=0000000000100848 type=00000002 size=00000018
tag=0000000000100860 type=0000000A size=0000001C
tag=0000000000100880 type=00000006 size=000000A0
tag=0000000000100920 type=00000009 size=00000314
tag=0000000000100C38 type=00000004 size=00000010
tag=0000000000100C48 type=00000005 size=00000014
tag=0000000000100C60 type=00000008 size=00000020
tag=0000000000100C80 type=0000000E size=0000001C
