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

# How to Build on Windows
1) Make sure you [download and install Cmake](https://cmake.org/download/)
```
git clone 'https://github.com/dashhive/bls'
cd bls

mkdir build
cd build
cmake ..

```

The above commands will create a `bls.sln` that you will have to open in [Visual Studio](https://visualstudio.microsoft.com/)
## IMPORTANT: Visual Studio is not the same as Visual Studio Code!

The community edition of Visual Studio is free.

Once you've created the `bls.sln` file, you will need to open that inside of Visual Studio.

Part of the build targets is `runtest`. Right click on that and click `Build`.

That will create a `bls.exe` file depending on what your target is. Out of the box, you may find that exe to exist somewhere in `bls\build\Debug\src\bls.exe`. Using `explorer.exe`'s find function would be the best way to find this. Or in Visual Studio you might be able to browse to where that exe is.


### Install

**Mac**, **Linux**:

```bash
curl -sS https://webinstall.dev/bls | bash
```

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

## Windows
### NOTE: Windows support is experimental as of version `v1.0.0`
Download the `windows-x86-64-bls.zip` (or the `.tar.gz` if you prefer) file from the official release at: https://github.com/dashhive/bls/releases/tag/v1.0.0

Unzip the file and run `./bls.exe`

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
- [x] Windows x86-64 (experimental)
	- note: Windows support is experimental as of version `v1.0.0`
	- want to earn some Dash? If you help us QA the Windows version, we will pay you in Dash

# Version
v.1.0.0
