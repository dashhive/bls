# bls
Implements [BLS signatures](https://github.com/Chia-Network/bls-signatures) with aggregation using [relic toolkit](https://github.com/relic-toolkit/relic) for cryptographic primitives (pairings, EC, hashing) according to the [IETF BLS RFC](https://datatracker.ietf.org/doc/draft-irtf-cfrg-bls-signature/) with [these curve parameters](https://datatracker.ietf.org/doc/draft-irtf-cfrg-pairing-friendly-curves/) for BLS12-381.

# Usage
```sh
./bls --help

# or:

./bls -h

# Example: generate from the hard-coded static seed:
./bls --static-seed

# or:

./bls -s

# Generate using a completely random seed:
./bls

# Generate a command to run dash-cli to verify a keypair
./bls --gen-dash-from-secret

# or:

./bls -g

# Example: multiple verbose flags can increase verbosity. Up to 4 (which is lots of debug output)
./bls -v -v -v -v

# Print version info
./bls --version

# or:

./bls -V

```

# Options
``` sh
 -s, --static-seed                  : generate keypairs using static seed
 -V, --version                      : print version
 -h, --help                         : print help screen
 -v, --verbose                      : print verbose output. can be specified multiple times
 -g, --gen-dash-from-secret         : generate dash-cli from-secret output
```

# How to Build on Linux
```sh
sudo apt -y install build-essential cmake
git clone 'https://github.com/dashhive/bls'
cd bls

mkdir build
cd build
cmake ..

make

# copy the binary to some place within your path (optional)
# sudo cp ./src/runtest /usr/bin/bls

bls --version
```


### Install

**Mac**, **Linux**:

```bash
curl -sS https://webinstall.dev/bls | bash
```

**Windows 10**:

Not supported

# Install via Github Releases

## Macos 11
```sh
wget 'https://github.com/dashhive/bls/releases/download/v1.0.0/macos11.tar.gz'
tar xvzf ./macos11.tar.gz
./bls
```

## Macos 12
```sh
wget 'https://github.com/dashhive/bls/releases/download/v1.0.0/macos12.tar.gz'
tar xvzf ./macos12.tar.gz
./bls
```

## Ubuntu Focal x86-64
```sh
wget 'https://github.com/dashhive/bls/releases/download/v1.0.0/ubuntu-focal-x86-64.tar.gz'
tar xvzf ./ubuntu-focal-x86-64.tar.gz
./bls
```

## Ubuntu Jammy Jellyfish x86-64
```sh
wget 'https://github.com/dashhive/bls/releases/download/v1.0.0/ubuntu-jammy-jellyfish-x86-64.tar.gz'
tar xvzf ./ubuntu-jammy-jellyfish-x86-64.tar.gz
./bls
```

## Ubuntu ARM64
```sh
wget 'https://github.com/dashhive/bls/releases/download/v1.0.0/ubuntu-22-04-arm64.tar.gz'
tar xvzf ./ubuntu-22-04-arm64.tar.gz
./bls
```

# TODO
- [ ] Workflow for building on Linux arm64 
- [ ] Workflow for building on Windows x86-64, arm64

# Environments where this has been tested
- [x] Devuan Chimaera x86-64
- [x] MacOS / M1
- [x] Raspberry Pi 4
- [ ] Raspberry Pi 3
- [x] Ubuntu Focal x86-64
- [x] Ubuntu Jammy Jellyfish x86-64
- [x] Ubuntu Jammy Jellyfish ARM64
- [x] Debian Bullseye x86-64

```
Distributor ID:	Devuan
Description:	Devuan GNU/Linux 4 (chimaera)
Release:	4
Codename:	chimaera

cmake version 3.18.4

CMake suite maintained and supported by Kitware (kitware.com/cmake).
```

# Version
v.1.0.0
