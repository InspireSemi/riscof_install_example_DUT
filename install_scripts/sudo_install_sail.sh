#!/bin/bash

# Install dependencies
sudo apt-get install opam  build-essential libgmp-dev z3 pkg-config zlib1g-dev

###############
# this will update to the latest version of opam
sudo add-apt-repository ppa:avsm/ppa
sudo apt update
###############