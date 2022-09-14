#include "bls.hpp"
extern "C" {
#include "relic.h"
}
#include "test-utils.hpp"
#define DASH_BLS_VERSION "v.1.1.0"

using namespace bls;
static uint8_t verbosity = 1;

enum output_format : uint8_t {
	JSON_KEYPAIR = (1 << 0),
	DASH_FROM_SECRET_COMMAND = (1 << 1),
};

static const std::vector<uint8_t> static_seed = {
	0,  50, 6,  244, 24,  199, 1,  25,  52,  88,  192,
	19, 18, 12, 89,  6,   220, 18, 102, 58,  209, 82,
	12, 62, 89, 110, 182, 9,   44, 20,  254, 22
};
void log_seed(const std::vector<uint8_t>& s) {
	std::cerr << "[verbose(" << std::to_string(verbosity) << ")]: SEED VALUE: " << Util::HexStr(s) << "\n";
}

void log(std::string msg, uint8_t if_verbosity,std::size_t line=0) {
	if(verbosity > if_verbosity) {
		std::string type;
		switch(if_verbosity) {
			case 1:
				type = "LOGGING";
				break;
			case 2:
				type = "INFO";
				break;
			case 3:
				type = "DEBUG";
				break;
			default:
				if(if_verbosity > 3) {
					type = "ULTRA_DEBUG";
					break;
				}
				break;
		}
		if(line && if_verbosity > 2) {
			std::cerr << "[" << type << "][FILE:" << __FILE__ << "][LINE:" << line << "]: " << msg << "\n";
		} else {
			std::cerr << "[" << type << "][FILE:" << __FILE__ <<"]: " << msg << "\n";
		}
	}
}

#define LOGGING(A) log(A,1)
#define INFO(A) log(A,2,__LINE__)
#define DEBUG(A) log(A,3,__LINE__)
#define ULTRAD(A) log(A,4,__LINE__)

static void dump_json_keypair(const PrivateKey& sk) {
	G1Element pk = sk.GetG1Element();
	ULTRAD("Dumping json...");
	std::cout << "{\n";
	std::cout << "  \"secret\": \"";
	ULTRAD("Dumping serialized Private Key (SK)");
	std::cout << Util::HexStr(sk.Serialize()) << "\",\n";
	ULTRAD("Dumped SK");
	std::cout << "  \"public\": \"";
	ULTRAD("Dumping serialized private key (PK)");
	std::cout << Util::HexStr(pk.Serialize(true)) << "\"\n";
	ULTRAD("Dumped PK");
	std::cout << "}\n";
	INFO("Done generating key pair JSON");
}
static void generate_keypair(bool use_random,output_format o_format) {
	LOGGING("generate_keypair entry");
	PrivateKey sk;
	std::vector<uint8_t> seed = static_seed;
	LOGGING("starting private key loop");
	while(true) {
		ULTRAD("Looping...");
		try {
			if(use_random) {
				DEBUG("Using random seed...");
				seed = getRandomSeed();
			} else {
				DEBUG("Using static seed...");
			}
			INFO("Generating private key from seed");
			if(verbosity > 2) {
				log_seed(seed);
			}
			ULTRAD("Attempting Private Key generation...");
			sk = PrivateKey::FromBytes(bls::Bytes((const uint8_t*)&seed[0],seed.size()));
			ULTRAD("Private Key seems good.");
			break;
		} catch(...) {
			DEBUG("Caught exception while generating private key. Retrying...");
		}
	}
	ULTRAD("Fetching Public Key Value");
	uint8_t commands_ran = 0;
	if(o_format & JSON_KEYPAIR) {
		dump_json_keypair(sk);
		++commands_ran;
	}
	if(o_format & DASH_FROM_SECRET_COMMAND) {
		ULTRAD("Dumping dash-cli fromsecret command...");
		std::cout << "./dash-cli bls fromsecret " << Util::HexStr(sk.Serialize()) << "\n";
		std::cout << "\nEXPECTED OUTPUT:\n";
		dump_json_keypair(sk);
		++commands_ran;
	}
	if(!commands_ran) {
		LOGGING("Unknown output format specified");
	}
}
static void generate_static_keypairs(output_format of) {
	generate_keypair(false,of);
}
static void generate_random_keypairs(output_format of) {
	generate_keypair(true,of);
}
static bool multi_cmp_or(const std::string& argv, const std::vector<std::string>& with) {
	for(const auto& str : with) {
		if(argv.compare(str.data()) == 0) {
			return true;
		}
	}
	return false;
}
static void usage(const char* exe) {
	std::cout <<
	    "Usage: " << exe << " [options]\n" <<
	    "\n" <<
	    "Options:\n" <<
	    " -s|--static-seed|/s|/staticSeed    : generate keypairs using static seed\n" <<
	    " -h|--help|/?|/help                 : print this help screen\n" <<
	    " -v|--verbose|/verbose              : print verbose output. can be specified multiple times\n" <<
	    " --gen-dash-from-secret|-g|         \n" <<
	    "   /genDashFromSecret               : generate dash-cli from-secret output\n" <<
	    " --print-json|-j|/printJSON         : print JSON keypairs (default)\n" <<
	    //" --json-seed-file=FILE              : TODO\n" <<
	    "\n" <<
	    "version: " << DASH_BLS_VERSION << "\n" <<
	    "project url: https://github.com/wmerfalen/bls-signatures\n" <<
	    "\n";
}
int main(int argc,char** argv) {
	int format = JSON_KEYPAIR;
	static const std::vector<std::string> help_list{"--help","-h","/?"};
	static const std::vector<std::string> static_seed_list{"--static-seed","-s","/s","/staticSeed"};
	static const std::vector<std::string> verbose_list{"--verbose","-v","/verbose"};
	static const std::vector<std::string> dash_cli_from_secret_list{"--gen-dash-from-secret","-g","/genDashFromSecret"};
	static const std::vector<std::string> print_json_list{"--print-json","-j","/printJSON"};
	bool do_static = false;
	if(argc > 1) {
		for(uint8_t i = 1; i < argc; i++) {
			std::string s(argv[i]);
			if(multi_cmp_or(s,help_list)) {
				usage(argv[0]);
				return 3;
			}
			if(multi_cmp_or(s,static_seed_list)) {
				do_static = true;
			}
			if(multi_cmp_or(s,verbose_list)) {
				++verbosity;
			}
			if(multi_cmp_or(s,dash_cli_from_secret_list)) {
				format = (format ^ DASH_FROM_SECRET_COMMAND);
			}
			if(multi_cmp_or(s,print_json_list)) {
				format = (format ^ JSON_KEYPAIR);
			}
		}
	}
	if(do_static) {
		generate_static_keypairs((output_format)format);
	} else {
		generate_random_keypairs((output_format)format);
	}
	return 2;
}
#undef LOGGING
#undef INFO
#undef DEBUG
#undef ULTRAD
