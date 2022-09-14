
// Copyright 2020 Chia Network Inc

// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//    http://www.apache.org/licenses/LICENSE-2.0

// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#define CATCH_CONFIG_RUNNER
#include <thread>

#include "bls.hpp"
#include "catch.hpp"
extern "C" {
#include "relic.h"
}
#include "test-utils.hpp"

using namespace bls;

const std::vector<uint8_t> seed = {
	0,  50, 6,  244, 24,  199, 1,  25,  52,  88,  192,
	19, 18, 12, 89,  6,   220, 18, 102, 58,  209, 82,
	12, 62, 89, 110, 182, 9,   44, 20,  254, 22
};

void pk() {
	PrivateKey sk = PrivateKey::FromByteVector(seed, true);
	G1Element pk = sk.GetG1Element();
	std::cout << "Private key: '" << Util::HexStr(sk.Serialize()) << "'\n";
	std::cout << "Public key: '" << Util::HexStr(pk.Serialize()) << "'\n";
}

int main(int argc,char** argv) {
	std::cout << "main()\n";
	pk();
	return 42;
}
