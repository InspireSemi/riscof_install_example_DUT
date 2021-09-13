#!/bin/bash

git clone https://github.com/riscv/riscv-isa-sim.git
cd riscv-isa-sim
mkdir build
cd build
../configure --prefix=~/.local
make
# this will install to your /home/<username>/.local directory
make install 