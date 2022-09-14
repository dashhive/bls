# Before you begin
STOP: This is not the repository you are looking for.

The purpose of this code is to provide a very specific use-case for the Dash Incubator project. The BLS code in this repo has not been modified by me. I simply made this repo for a *very* specific internal purpose as requested by the folks running the Dash Incubator project.

# AGAIN: THIS IS NOT THE REPO YOU ARE LOOKING FOR

# The original README.md
The original `bls-signatures` README.md is at `VENDOR.md`.

# Purpose
To simply print a keypair based on a static seed input.
This code is a shortcut to what you would normally get from running `dashd` and using the RPC command: `dash-cli bls generate`.

Of course, the `dash-cli` will utilize a strong randomized number, so the above command will not align with your code unless you patched `src/bls/bls.cpp` like so:
``` cpp
#ifndef BUILD_BITCOIN_INTERNAL
void CBLSSecretKey::MakeNewKey() {
	unsigned char buf[32];
	while(true) {
		//GetStrongRandBytes(buf, sizeof(buf));
		const std::vector<uint8_t> seed = {
			0,  50, 6,  244, 24,  199, 1,  25,  52,  88,  192,
			19, 18, 12, 89,  6,   220, 18, 102, 58,  209, 82,
			12, 62, 89, 110, 182, 9,   44, 20,  254, 22
		};
		try {
			impl = bls::PrivateKey::FromBytes(bls::Bytes((const uint8_t*)&seed[0], SerSize));
			break;
		} catch(...) {
		}
	}
	fValid = true;
	cachedHash.SetNull();
}
#endif
```

You would then re-run the make file that was generated by CMake, restart `dashd`, then re-run `dash-cli bls generate`. You will ALWAYS get this output:
``` json
{
  "secret": "003206f418c701193458c013120c5906dc12663ad1520c3e596eb6092c14fe16",
  "public": "86267afa0bc64fb10757afa93198acaf353b11fae85d19e7265f3825abe70501e68c5bc7c816c3c57b1ff7a74298a32f"
}
```

# Where is this cli utility?
I figured, why change the CMake file and add another dependency. So instead of adding a `src/main.cpp`, I simply replaced `src/test.cpp` with my code. 

# How to build and run?
It's the same way you'd build any CMake project:
``` sh
mkdir build
cd build
cmake ..
make
```

# Again, where is the cli utility?
After you run the above CMake command, you will then run `./build/src/runtest`.

# Author
William Merfalen

# Version
1.0

# Dash bounty
TBD
