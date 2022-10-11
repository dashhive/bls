#include "bls.hpp"
#include "hdkeys.hpp"
#include "schemes.hpp"
#include <sstream>
#include <optional>

extern "C" {
#include "relic.h"
}
#include "test-utils.hpp"
#define DASH_BLS_VERSION "v.2.0.0"

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
/**
 above seed will generate
{"secret":"377091f0e728463bc2da7d546c53b9f6b81df4a1cc1ab5bf29c5908b7151a32d","public":"06243290bbcbfd9ae75bdece7981965350208eb5e99b04d5cd24e955ada961f8c0a162dee740be7bdc6c3c0613ba2eb1"}
*/
template <typename T>
std::optional<T> stoi_optional(std::string_view str) {
	T i;
	std::stringstream ss;
	ss << str;
	ss >> i;
	if(ss.fail()) {
		return std::nullopt;
	} else {
		return i;
	}
}

std::vector<uint8_t> user_seed;

std::tuple<bool,std::string,std::vector<uint32_t>> parse_indices(std::string_view str) {
	std::vector<uint32_t> indices;
	std::string current;
	for(const auto& ch : str) {
		if(isdigit(ch)) {
			current += ch;
			continue;
		}
		if(ch == ',') {
			if(current.length()) {
				auto opt = stoi_optional<int>(current);
				if(opt.has_value() == false || opt.value() <= 0) {
					return {false,std::string("indices must be positive non-zero numbers. Encountered issue processing string at: '") + current + "'",indices};
				}
				uint32_t i = stoi_optional<uint32_t>(current).value();
				indices.emplace_back(i);
				current.clear();
			}
		}
	}
	if(current.length()) {
		auto opt = stoi_optional<int>(current);
		if(opt.has_value() == false || opt.value() <= 0) {
			return {false,std::string("indices must be positive non-zero numbers. Encountered issue processing string at: '") + current + "'",indices};
		}
		uint32_t i = stoi_optional<uint32_t>(current).value();
		indices.emplace_back(opt.value());
	}
	return {true,"",indices};
}
std::tuple<bool,std::string,std::vector<uint8_t>> parse_hexstring(std::string_view str) {
	std::vector<uint8_t> converted;
	std::string current;
	for(const auto& ch : str) {
		if(isdigit(ch) || (tolower(ch) >= 97 && tolower(ch) <= 102)) {
			current += ch;
		} else {
			return {0,std::string("character '") + std::to_string(ch) + "' is not a valid hexadecimal character",converted};
		}
		if(current.length() == 2) {
			uint8_t actual_byte = 0;
			uint8_t byte_0 = tolower(current[0]);
			uint8_t byte_1 = tolower(current[1]);
			if(byte_0 >= 97) {
				actual_byte = byte_0 - 87;
			} else {
				actual_byte = byte_0 - 48;
			}
			actual_byte *= 16;
			if(byte_1 >= 97) {
				actual_byte += byte_1 - 87;
			} else {
				actual_byte += byte_1 - 48;
			}
			converted.emplace_back(actual_byte);
			current.clear();
		}
	}
	return {true,"",converted};
}
std::tuple<bool,std::string,std::vector<uint8_t>> parse_seed(std::string_view seed) {
	if(seed.length() != 64) {
		return {false,std::string("seed must be exactly 64 hexadecimal characters. the seed length you supplied is ") + std::to_string(seed.length()) + " characters long",{}};
	}
	return parse_hexstring(seed);
}
std::tuple<bool,std::string,std::vector<uint8_t>> from_parsed_secret(std::string_view secret) {
	if(secret.length() != 64) {
		return {false,std::string("secret must be exactly 64 hexadecimal characters. the seed length you supplied is ") + std::to_string(secret.length()) + " characters long",{}};
	}
	return parse_hexstring(secret);
}

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
	std::cout << "{\"secret\":\"";
	ULTRAD("Dumping serialized Private Key (SK)");
	std::cout << Util::HexStr(sk.Serialize()) << "\",";
	ULTRAD("Dumped SK");
	std::cout << "\"public\":\"";
	ULTRAD("Dumping serialized private key (PK)");
	std::cout << Util::HexStr(pk.Serialize(true)) << "\"";
	ULTRAD("Dumped PK");
	std::cout << "}\n";
	INFO("Done generating key pair JSON");
}
static PrivateKey generate_hd_keypair_from_parent_sk(const PrivateKey& sk,const uint32_t& index) {
	LOGGING("generate_hd_keypair_from_parent_sk entry");
	PrivateKey child;
	LOGGING("starting private key loop");
	while(true) {
		ULTRAD("Looping...");
		try {
			bls::HDKeys hdk;
			ULTRAD("Attempting Private Key generation (from existing sk)...");
			child = hdk.DeriveChildSk(sk,index);
			ULTRAD("Private Key seems good.");
			break;
		} catch(const std::exception& e) {
			DEBUG("Caught exception: ");
			DEBUG(e.what());
			DEBUG("Caught exception while generating private key. Retrying...");
		}
	}
	return child;
}
PrivateKey aug_generate_sk_from_seed(const std::vector<uint8_t>& seed) {
	return bls::AugSchemeMPL().KeyGen(seed);
}

PrivateKey generate_sk_from_seed(const std::vector<uint8_t>& seed) {
	LOGGING("generate_sk_from_seed entry");
	INFO("Generating private key from seed");
	if(verbosity > 2) {
		log_seed(seed);
	}
	ULTRAD("Attempting Private Key generation...");
	return aug_generate_sk_from_seed(seed);
}

static std::pair<PrivateKey,std::vector<PrivateKey>> derive_keypairs_from_sk_index(
        const PrivateKey& parent_sk,
const std::vector<uint32_t>& indices) {
	std::vector<PrivateKey> derived;
	PrivateKey child_sk;
	for(const auto& index : indices) {
		while(true) {
			try {
				child_sk = generate_hd_keypair_from_parent_sk(parent_sk,index);
				derived.emplace_back(child_sk);
				break;
			} catch(const std::exception& e) {
				DEBUG("Caught exception: ");
				DEBUG(e.what());
				DEBUG("Caught exception while generating derived private key. Retrying...");
			}
		}
	}
	return {parent_sk,derived};
}

static void generate_keypair(bool use_random,output_format o_format) {
	LOGGING("generate_keypair entry");
	PrivateKey sk;
	std::vector<uint8_t> seed;
	bool use_user_seed = user_seed.size();
	bool call_random = false;
	if(use_user_seed) {
		seed = user_seed;
		call_random = false;
	} else if(use_random) {
		call_random = true;
	} else {
		seed = static_seed;
		call_random = false;
	}
	LOGGING("starting private key loop");
	while(true) {
		ULTRAD("Looping...");
		try {
			if(call_random) {
				DEBUG("Using random seed...");
				seed = getRandomSeed();
			}
			INFO("Generating private key from seed");
			if(verbosity > 2) {
				log_seed(seed);
			}
			ULTRAD("Attempting Private Key generation...");
			sk = generate_sk_from_seed(seed);
			ULTRAD("Private Key seems good.");
			break;
		} catch(const std::exception& e) {
			DEBUG("Caught exception: ");
			DEBUG(e.what());
			DEBUG("Caught exception while generating private key. Retrying...");
		}
	}
	ULTRAD("Fetching Public Key Value");
	uint8_t commands_ran = 0;
	if(o_format & JSON_KEYPAIR && (!(o_format & DASH_FROM_SECRET_COMMAND))) {
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
std::string generate_seed() {
	DEBUG("generate_seed entry");
	while(true) {
		try {
			DEBUG("Trying to generate random seed...");
			auto seed = getRandomSeed();
			auto key = aug_generate_sk_from_seed(seed);
			return Util::HexStr(seed);
		} catch(const std::exception& e) {
			DEBUG("Caught exception: ");
			DEBUG(e.what());
			DEBUG("caught exception while generating a random seed. Retrying...");
		}
	}
	return "";
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
static void version() {
	std::cout << DASH_BLS_VERSION << "\n";
}
static void usage(const char* exe) {
	std::cout <<
	    "Usage: " << exe << " [options]\n" <<
	    "\n" <<
	    "Options:\n" <<
	    " --seed N                            : use hexadecimal seed N where N is 64 hexadecimal characters\n" <<
	    " -F N,fromsecret N, --from-secret N  : generate keypairs using N as the secret key\n" <<
	    " -G, --generate-seed                 : outputs a randomly generated seed that can be passed to --seed\n" <<
	    " -c N, --count N                     : if -G is also specified, generate N random seeds\n" <<
	    " -i N, --index N                     : derive the Nth child keypair. can be a CSV\n" <<
	    " -s, --from-example-seed             : generate keypairs using static seed\n" <<
	    " -h, --help                          : print this help screen\n" <<
	    " -v, --verbose                       : print verbose output. can be specified multiple times\n" <<
	    " -V, --version                       : print version\n" <<
	    " -g, --gen-dash-from-secret          : generate dash-cli from-secret output\n" <<
	    "\n" <<
	    "version: " << DASH_BLS_VERSION << "\n" <<
	    "project url: https://github.com/dashhive/bls\n" <<
	    "\n";
}

int main(int argc,char** argv) {
	int format = JSON_KEYPAIR;
	static const std::vector<std::string> help_list{"--help","-h"};
	static const std::vector<std::string> static_seed_list{"--from-example-secret","-s"};
	static const std::vector<std::string> verbose_list{"--verbose","-v"};
	static const std::vector<std::string> dash_cli_from_secret_list{"--gen-dash-from-secret","-g"};
	static const std::vector<std::string> print_json_list{"--print-json","-j"};
	static const std::vector<std::string> version_list{"--version","-V"};
	static const std::vector<std::string> from_secret_list{"fromsecret","--from-secret","-F"};
	static const std::vector<std::string> from_seed_list{"--seed"};
	static const std::vector<std::string> index_list{"-i","--index"};
	static const std::vector<std::string> generate_seed_list{"-G","--generate-seed"};
	static const std::vector<std::string> count_list{"-c","--count"};
	bool do_static = false;
	bool recognized_flag = false;
	bool show_usage = false;
	bool capture_seed_next = false;
	bool capture_index = false;
	bool use_user_supplied_seed = false;
	bool gen_seed = false;
	bool capture_count = false;
	bool capture_from_secret = false;
	std::size_t generate_seed_count = 1;
	std::vector<std::string> unrecognized_flags;
	std::vector<uint32_t> indices;
	std::string user_supplied_secret;
	if(argc > 1) {
		for(uint8_t i = 1; i < argc; i++) {
			recognized_flag = false;
			std::string s(argv[i]);
			if(multi_cmp_or(s,from_seed_list)) {
				capture_seed_next = true;
				continue;
			}
			if(multi_cmp_or(s,count_list)) {
				capture_count = true;
				continue;
			}
			if(capture_count) {
				capture_count = false;
				auto opt = stoi_optional<int>(s);
				if(opt.has_value() == false || opt.value() <= 0) {
					std::cerr << "error: count must be a positive non-zero integer. Instead, we got: '" << s << "'\n";
					return 10;
				}
				generate_seed_count = opt.value();
				continue;
			}
			if(multi_cmp_or(s,generate_seed_list)) {
				gen_seed = true;
				continue;
			}
			if(multi_cmp_or(s,index_list)) {
				capture_index = true;
				if(i + 1 >= argc) {
					std::cerr << "error: " << s << " requires an argument\n";
					return 8;
				}
				recognized_flag = true;
				continue;
			}
			if(capture_index) {
				capture_index = false;
				auto parse_tuple = parse_indices(s);
				if(!std::get<0>(parse_tuple)) {
					std::cerr << "error: " << std::get<1>(parse_tuple) << "\n";
					return 9;
				}
				indices = std::get<2>(parse_tuple);
				for(auto in : indices) {
					DEBUG(std::string("index requested: ") + std::to_string(in));
				}
				recognized_flag = true;
				continue;
			}
			if(multi_cmp_or(s,from_secret_list)) {
				if(i + 1 >= argc) {
					std::cerr << "error: " << s << " requires an argument\n";
					return 5;
				}
				capture_from_secret = true;
				continue;
			}
			if(capture_from_secret) {
				capture_from_secret = false;
				user_supplied_secret = s;
				continue;
			}
			if(capture_seed_next) {
				use_user_supplied_seed = true;
				auto result = parse_seed(s);
				if(!std::get<0>(result)) {
					std::cerr << "error: " << std::get<1>(result) << "\n";
					return 6;
				}
				user_seed = std::get<2>(result);
				capture_seed_next = false;
				continue;
			}
			if(multi_cmp_or(s,help_list)) {
				show_usage = true;
				continue;
			}
			if(multi_cmp_or(s,version_list)) {
				version();
				return 0;
			}
			if(multi_cmp_or(s,static_seed_list)) {
				do_static = true;
				recognized_flag = true;
			}
			if(multi_cmp_or(s,verbose_list)) {
				recognized_flag = true;
				++verbosity;
			}
			if(multi_cmp_or(s,dash_cli_from_secret_list)) {
				recognized_flag = true;
				format = (format ^ DASH_FROM_SECRET_COMMAND);
			}
			if(multi_cmp_or(s,print_json_list)) {
				recognized_flag = true;
				format = (format ^ JSON_KEYPAIR);
			}
			if(!recognized_flag) {
				unrecognized_flags.push_back(s);
				continue;
			}
		}
		if(unrecognized_flags.size()) {
			for(std::size_t i=0; i < unrecognized_flags.size(); ++i) {
				std::cerr << "error '" << unrecognized_flags[i] << "' is not a recognized flag\n";
			}
			return 4;
		}
		if(show_usage) {
			usage(argv[0]);
			return 0;
		}
	}
	if(gen_seed) {
		std::cout << "{\"seeds\":[";
		for(std::size_t i =0; i < generate_seed_count; i++) {
			std::cout << "\"" << generate_seed() << "\"";
			if(i + 1 < generate_seed_count) {
				std::cout << ",";
			}
		}
		std::cout << "]}";
		exit(0);
	}
	PrivateKey sk;
	if(user_supplied_secret.length()) {
		/**
		 * Use has supplied us with an sk on the commandline.
		 * Attempt to parse and import that sk.
		 */
		auto t = from_parsed_secret(user_supplied_secret);
		if(!std::get<0>(t)) {
			std::cerr << "error: invalid secret key supplied: " << std::get<1>(t) << "\n";
			return 11;
		}
		auto user_sk = std::get<2>(t);
		sk = PrivateKey::FromBytes(bls::Bytes((const uint8_t*)&user_sk[0],user_sk.size()));
	} else {
		/**
		 * The user has _not_ asked us to use a user-supplied sk.
		 * Instead, they've either asked that we use a static seed,
		 * a user supplied seed, or a completely random seed.
		 *
		 * We will attempt to generate the sk using one of these seeds.
		 * It's sometimes possible that there is an unrecoverable exception
		 * that occurs when we specify a seed that the algorithm doesn't like.
		 * The most common unrecoverable sk generation exception is:
		 *
		        "PrivateKey byte data must be less than the group order"
			 This exception is thrown in src/privatekey.cpp in the FromBytes function

		 */
		std::vector<uint8_t> seed;
		enum generation_type {
			STATIC_SEED,
			USER_SUPPLIED,
			RANDOM,
		};
		generation_type gen_type;
		if(do_static) {
			gen_type = STATIC_SEED;
		} else if(use_user_supplied_seed) {
			gen_type = USER_SUPPLIED;
		} else {
			gen_type = RANDOM;
		}
		while(true) {
			try {
				switch(gen_type) {
					case STATIC_SEED:
						sk = generate_sk_from_seed(static_seed);
						break;
					case USER_SUPPLIED:
						sk = generate_sk_from_seed(user_seed);
						break;
					default:
					case RANDOM:
						sk = aug_generate_sk_from_seed(getRandomSeed());
						break;
				}
				DEBUG("Generated a sk. Using that...");
				break;
			} catch(const std::exception& e) {
				switch(gen_type) {
					default:
						break;
					case STATIC_SEED:
					case USER_SUPPLIED:
						/**
						 * This is not a recoverable exception if the user supplied us with a seed and it throws here.
						 * We just have to ask the user to give us a different seed.
						 */
						std::cerr << "error: the particular seed you are using is invalid: " << e.what() << "\n";
						return 11;
				}
				DEBUG("Caught exception: ");
				DEBUG(e.what());
				DEBUG("caught exception while trying to generate_sk_from_seed. Retrying...");
				return 10;
			}
		}
	}
	/**
	 * If we get here, then that means one of two things are for certain:
	 * 1) we have generated an sk successfully
	 * 2) the user would like for us to derive or dump keypairs
	 */
	if(indices.size()) {
		/**
		 * If the user specified -G -c N
		 *
		 * -- note that N can be a csv. If that's the case, we loop over the csv
		 *  values and take those indices and print them
		 */
		auto pair = derive_keypairs_from_sk_index(sk,indices);
		std::cout << "{" <<
		    "\"parent\": {" <<
		    "\"secret\": \"" << Util::HexStr(pair.first.Serialize()) << "\"," <<
		    "\"public\": \"" << Util::HexStr(pair.first.GetG1Element().Serialize(true)) << "\"" <<
		    "}," <<
		    "\"children\": [";
		std::size_t i = 0;
		for(const auto& child_sk : pair.second) {
			std::cout << "{" <<
			    "\"index\": " << indices[i] << "," <<
			    "\"secret\": \"" << Util::HexStr(child_sk.Serialize()) << "\"," <<
			    "\"public\": \"" << Util::HexStr(child_sk.GetG1Element().Serialize(true)) << "\"" <<
			    "}";
			if(i + 1 < indices.size()) {
				std::cout << ",";
			}
			++i;
		}
		std::cout << "]}";
	} else {
		/**
		 * This block will be executed if the user wants to print out
		 * the keypair or generate the dash-cli fromsecret command.
		 */
		uint8_t commands_ran = 0;
		if(format & JSON_KEYPAIR && (!(format & DASH_FROM_SECRET_COMMAND))) {
			dump_json_keypair(sk);
			++commands_ran;
		}
		if(format & DASH_FROM_SECRET_COMMAND) {
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
	return 0;
}
#undef LOGGING
#undef INFO
#undef DEBUG
#undef ULTRAD
