#!/bin/bash

# build z3 from source
git clone https://github.com/Z3Prover/z3.git
cd z3
python3 scripts/mk_make.py --prefix=~/.local/
cd build
make
make install

cd ..

# copy opam 
cp ./opam/opam-2.1.0-x86_64-linux ~/.local/bin/opam

# Setup opam
opam init -y --disable-sandboxing
opam switch create ocaml-base-compiler.4.06.1
opam install sail -y
eval $(opam config env)

# Clone sail-riscv 
git clone https://github.com/rems-project/sail-riscv.git

# Make sail 
cd sail-riscv
make
ARCH=RV32 make
ARCH=RV64 make

cd .. 

#Uncomment these if you want to add links to the sail binaries
# Otherwise use the sailpath.sh to set the PATH for the sail binaries
#ln -s sail-riscv/c_emulator/riscv_sim_RV64 /usr/bin/riscv_sim_RV64
#ln -s sail-riscv/c_emulator/riscv_sim_RV32 /usr/bin/riscv_sim_RV32

# Clone the arch-test repo
git clone https://github.com/riscv/riscv-arch-test.git

#Alternate way to clone arch-test repo
#riscof --verbose info arch-tests --clone