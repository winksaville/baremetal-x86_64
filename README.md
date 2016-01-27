# baremetal-x86_64

A a bare metal program that runs on qemu-system-x86_64
I use it for experimentation, the initial commit only output
"OK" to the serial port it now does quite a bit more.

It is based on Philipp Opperman's [boot_os]
(https://github.com/phil-opp/blog_os/tree/multiboot_bootstrap/src/arch/x86_64)
and his blog post [A minimal x86 kernel](http://os.phil-opp.com/multiboot-kernel.html).
I've ported his blog_os code to my [baremetal-po-x86_64]
(https://github.com/winksaville/baremetal-po-x86_64) with a simplified
directory structure which is very much minimal.

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

Create crosstool chain.
---

Two choices use my [vendor-install-tools](https://github.com/winksaville/vendor-install-tools)
or follow this instructions which installs just a tweaked version of gcc 5.3.1

Build install crosstool-ng

First get crosstool-ng, build and install. Modified as desired,
config.x86_64-unknown-elf, which builds a static
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
```
Next get a modified version of GCC that I've created which
supports interrupt attribute and use ct-ng to build and install.
Eventaully gcc 7.x.x will support interrupt attribute.
```
mkdir -p x86_64-elf
cd x86_64-elf
git clone https://github.com/winksaville/gcc.git --depth 1 --single-branch --branch wink-intr-attr
cp ../config.x86_64-unknown-elf .config
ct-ng build
cd ..
```
Build and run
---
```
make run
```
