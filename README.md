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

Build and run
---
```
make run
```

Type 'ctrl-a' x to exit.
