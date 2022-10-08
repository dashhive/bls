# bls

Implements [BLS signatures](https://github.com/Chia-Network/bls-signatures) with aggregation using [relic toolkit](https://github.com/relic-toolkit/relic) for cryptographic primitives (pairings, EC, hashing) according to the [IETF BLS RFC](https://datatracker.ietf.org/doc/draft-irtf-cfrg-bls-signature/) with [these curve parameters](https://datatracker.ietf.org/doc/draft-irtf-cfrg-pairing-friendly-curves/) for BLS12-381.

# Table of Contents

-   Usage
-   Build
-   Install

# Usage

# Options

```sh
  --seed N                            : use hexadecimal seed N where N is 64 hexadecimal characters
  -F S,fromsecret S, --from-secret S  : generate keypairs using S as the secret key
  -G, --generate-seed                 : outputs a randomly generated seed that can be passed to --seed
  -c N, --count N                     : if -G is also specified, generate N random seeds
  -i N, --index N                     : derive the Nth child keypair. can be a CSV
  -s, --from-example-seed             : generate keypairs using static seed
  -h, --help                          : print this help screen
  -v, --verbose                       : print verbose output. can be specified multiple times
  -V, --version                       : print version
  -g, --gen-dash-from-secret          : generate dash-cli from-secret output
  -j, --print-json                    : print JSON keypairs (default)
```

## Help page

```sh
./bls --help

# or:

./bls -h
```

## Generate using the hard-coded static seed:

```sh
./bls --from-example-seed
```

```json
{"secret":"377091f0e728463bc2da7d546c53b9f6b81df4a1cc1ab5bf29c5908b7151a32d","public":"06243290bbcbfd9ae75bdece7981965350208eb5e99b04d5cd24e955ada961f8c0a162dee740be7bdc6c3c0613ba2eb1"}
```

# or:

```sh
./bls -s
```

## Generate using a completely random seed:

```sh
./bls
```

## Generate using a specific secret key:

```sh
./bls --from-secret 5f657ab173a6744227fd5467893acd6da4fed1aa2b141380bafab78194629a54
```

```json
{"secret":"5f657ab173a6744227fd5467893acd6da4fed1aa2b141380bafab78194629a54","public":"8a4c5cb69462e448b3620735f4e70e9dbf4e42ccc40fb4c83d8cf60240632f2eb0e9b6e4b8437078ef1766a42b9838ec"}
```

## Generate using a specific seed:

```sh
./bls --seed 10a5496b5594fb13c288c892385a9b5df75af352042f67b4f24b2a6fa565f657
```

```json
{"secret":"6a968bade16f75c719c209b7bffe463cf6210978dfd970acc880766ffb049d62","public":"163690ecc6d4a07ad4cf1643b256157305009bd8338d00b3cec199a27be38be18fa68566ba484de66f5fe9735171ec5c"}
```

## Generate a seed:

The `--generate-seed` flag will allow you to generate a random seed

```sh
./bls --generate-seed
```

### Generating `N` seeds:

If `--count N` is used with `--generate-seed`, the utility will generate `N` random seeds:

```sh
./bls --generate-seed --count 3
```

```json
{
    "seeds": [
        "5af028b010bfedb52ec0f635eb8c83ccef9539d4431ae803c69681878791715a",
        "0bb029fc53884bd4599453a14828b31020eb41135c0d0829d290dc49d471b68d",
        "d5e9d8edbad86432a1b871bc09bfbb59d241ab916f1cbc3fb01621274b5094e1"
    ]
}
```

# or

```sh
./bls -G -c 3
```

The seeds are stored in an array under the `seeds` key

## Generate `X,Y,Z` child indices:

```sh
./bls -i 2,4,8
```

```json
{
    "parent": {
        "secret": "0139bfbc0f4626d8817efcde60927699450c134e5528ff258be3a73966f59548",
        "public": "801665837afeb647bcff2b3325307cc998d327486361a7fa9513f6e5dced54b4c5eef65dafd4d740bee63af451d09bb0"
    },
    "children": [
        {
            "index": 2,
            "secret": "0a91709770559aad2a19a65e8130e3cc3f91876e65709be5a9e5d91deff47dad",
            "public": "99ac882a7605814e395f16179357c961df7fafcf8ff4fd64e080261db8661ef7cf53a13a772f82cf767c718701d77f89"
        },
        {
            "index": 4,
            "secret": "2ddca8d4094f200d1af0e9103a13fe20d0253af8e0ca1b1379bb6be875e1143e",
            "public": "09ab18481eeb21a32793708b54495ff29eca27ba0afe48e87abd6df0ebcd8bec0b6a18f1306499576a2780a8af9d2a3e"
        },
        {
            "index": 8,
            "secret": "3128d6b24e871a306fb050ad73a89ea37c99315b9cb1879e00f68d6d08c03bbb",
            "public": "8f9bfccdafe90dc9ebaa16959638fb65563ed988eeba5001a04f67a059adeea7c69ffc29908434788485e1bb89452c38"
        }
    ]
}
```

The above output prettified:

```json
{
    "parent": {
        "secret": "0139bfbc0f4626d8817efcde60927699450c134e5528ff258be3a73966f59548",
        "public": "801665837afeb647bcff2b3325307cc998d327486361a7fa9513f6e5dced54b4c5eef65dafd4d740bee63af451d09bb0"
    },
    "children": [
        {
            "index": 2,
            "secret": "0a91709770559aad2a19a65e8130e3cc3f91876e65709be5a9e5d91deff47dad",
            "public": "99ac882a7605814e395f16179357c961df7fafcf8ff4fd64e080261db8661ef7cf53a13a772f82cf767c718701d77f89"
        },
        {
            "index": 4,
            "secret": "2ddca8d4094f200d1af0e9103a13fe20d0253af8e0ca1b1379bb6be875e1143e",
            "public": "09ab18481eeb21a32793708b54495ff29eca27ba0afe48e87abd6df0ebcd8bec0b6a18f1306499576a2780a8af9d2a3e"
        },
        {
            "index": 8,
            "secret": "3128d6b24e871a306fb050ad73a89ea37c99315b9cb1879e00f68d6d08c03bbb",
            "public": "8f9bfccdafe90dc9ebaa16959638fb65563ed988eeba5001a04f67a059adeea7c69ffc29908434788485e1bb89452c38"
        }
    ]
}
```

-   The JSON output will always have two top level keys `parent` and `children`.
    -   The derived child index is at `children[N].index`
-   The `-i` (or `--index`) options can work with `--seed`,`--from-secret`,or `--from-example-secret` (piped to `jq` for humans):

```sh
./bls --seed 5af028b010bfedb52ec0f635eb8c83ccef9539d4431ae803c69681878791715a --index 2,4,8 | jq
{
  "parent": {
    "secret": "21aacfe4550d76551a0dfde9f632bdfc3ba8d9a983910c3e635f31ae6ce6c9ab",
    "public": "054d6d2433d5eec5b2e547cf355d7c143f1491a56afc79f2e85ded3da32d1cbe448b08e41b995f5ccecc13c10877aaab"
  },
  "children": [
    {
      "index": 2,
      "secret": "6ba982226c50d68da5bc0d7b9496053746fba9300f239273a056326723f279c4",
      "public": "92415f3f4bd45acb939c35e4f2f9803baa37fed317e69173e43fbafa720dba122e34fc73c1758df10b17ac089e854e8f"
    },
    {
      "index": 4,
      "secret": "2e3910d4aa19626ebbca3d0566a1ab3a4df36774abd8a207272945607f49205d",
      "public": "928c7255623239fc3de9b0e9279f85b7e74441a9353eae0414ec9561fc710d05933d068d6aae92844ebfd2aca44e5841"
    },
    {
      "index": 8,
      "secret": "5f4c7a8d71cffea422982afa997798210da3dec39f4d7b15e3e6cb253de75fe6",
      "public": "8b35c325ec3f61f02de0f66ed700247bbb7c811c4eab8e8b42b53241ce7a35c4d6e2e800f7e435e3a7ea6294d01259b9"
    }
  ]
}

# or
./bls --seed 5af028b010bfedb52ec0f635eb8c83ccef9539d4431ae803c69681878791715a -i 2,4,8 | jq

# The above command will have identical output to this:
./bls --from-secret 21aacfe4550d76551a0dfde9f632bdfc3ba8d9a983910c3e635f31ae6ce6c9ab -i 2,4,8 | jq

```

### Generating `N` seeds:

If `--count N` is used with `--generate-seed`, the utility will generate `N` random seeds:

```sh
./bls --generate-seed --count 3
```

```json
{"seeds":["5af028b010bfedb52ec0f635eb8c83ccef9539d4431ae803c69681878791715a","0bb029fc53884bd4599453a14828b31020eb41135c0d0829d290dc49d471b68d","d5e9d8edbad86432a1b871bc09bfbb59d241ab916f1cbc3fb01621274b5094e1"]}

# or

```sh
./bls -G -c 3
```

The seeds are stored in an array under the `seeds` key

## Generate `X,Y,Z` child indices:

```sh
./bls -i 2,4,8
```

```json
{"parent": {"secret": "0139bfbc0f4626d8817efcde60927699450c134e5528ff258be3a73966f59548","public": "801665837afeb647bcff2b3325307cc998d327486361a7fa9513f6e5dced54b4c5eef65dafd4d740bee63af451d09bb0"},"children": [{"index": 2,"secret": "0a91709770559aad2a19a65e8130e3cc3f91876e65709be5a9e5d91deff47dad","public": "99ac882a7605814e395f16179357c961df7fafcf8ff4fd64e080261db8661ef7cf53a13a772f82cf767c718701d77f89"},{"index": 4,"secret": "2ddca8d4094f200d1af0e9103a13fe20d0253af8e0ca1b1379bb6be875e1143e","public": "09ab18481eeb21a32793708b54495ff29eca27ba0afe48e87abd6df0ebcd8bec0b6a18f1306499576a2780a8af9d2a3e"},{"index": 8,"secret": "3128d6b24e871a306fb050ad73a89ea37c99315b9cb1879e00f68d6d08c03bbb","public": "8f9bfccdafe90dc9ebaa16959638fb65563ed988eeba5001a04f67a059adeea7c69ffc29908434788485e1bb89452c38"}]}
```

The above output prettified:
```json
{
  "parent": {
    "secret": "0139bfbc0f4626d8817efcde60927699450c134e5528ff258be3a73966f59548",
    "public": "801665837afeb647bcff2b3325307cc998d327486361a7fa9513f6e5dced54b4c5eef65dafd4d740bee63af451d09bb0"
  },
  "children": [
    {
      "index": 2,
      "secret": "0a91709770559aad2a19a65e8130e3cc3f91876e65709be5a9e5d91deff47dad",
      "public": "99ac882a7605814e395f16179357c961df7fafcf8ff4fd64e080261db8661ef7cf53a13a772f82cf767c718701d77f89"
    },
    {
      "index": 4,
      "secret": "2ddca8d4094f200d1af0e9103a13fe20d0253af8e0ca1b1379bb6be875e1143e",
      "public": "09ab18481eeb21a32793708b54495ff29eca27ba0afe48e87abd6df0ebcd8bec0b6a18f1306499576a2780a8af9d2a3e"
    },
    {
      "index": 8,
      "secret": "3128d6b24e871a306fb050ad73a89ea37c99315b9cb1879e00f68d6d08c03bbb",
      "public": "8f9bfccdafe90dc9ebaa16959638fb65563ed988eeba5001a04f67a059adeea7c69ffc29908434788485e1bb89452c38"
    }
  ]
}
```
- The JSON output will always have two top level keys `parent` and `children`.
	- The derived child index is at `children[N].index`
- The `-i` (or `--index`) options can work with `--seed`,`--from-secret`,or `--from-example-secret` (piped to `jq` for humans):
```sh
./bls --seed 5af028b010bfedb52ec0f635eb8c83ccef9539d4431ae803c69681878791715a --index 2,4,8 | jq
{
  "parent": {
    "secret": "21aacfe4550d76551a0dfde9f632bdfc3ba8d9a983910c3e635f31ae6ce6c9ab",
    "public": "054d6d2433d5eec5b2e547cf355d7c143f1491a56afc79f2e85ded3da32d1cbe448b08e41b995f5ccecc13c10877aaab"
  },
  "children": [
    {
      "index": 2,
      "secret": "6ba982226c50d68da5bc0d7b9496053746fba9300f239273a056326723f279c4",
      "public": "92415f3f4bd45acb939c35e4f2f9803baa37fed317e69173e43fbafa720dba122e34fc73c1758df10b17ac089e854e8f"
    },
    {
      "index": 4,
      "secret": "2e3910d4aa19626ebbca3d0566a1ab3a4df36774abd8a207272945607f49205d",
      "public": "928c7255623239fc3de9b0e9279f85b7e74441a9353eae0414ec9561fc710d05933d068d6aae92844ebfd2aca44e5841"
    },
    {
      "index": 8,
      "secret": "5f4c7a8d71cffea422982afa997798210da3dec39f4d7b15e3e6cb253de75fe6",
      "public": "8b35c325ec3f61f02de0f66ed700247bbb7c811c4eab8e8b42b53241ce7a35c4d6e2e800f7e435e3a7ea6294d01259b9"
    }
  ]
}

# or
./bls --seed 5af028b010bfedb52ec0f635eb8c83ccef9539d4431ae803c69681878791715a -i 2,4,8 | jq

# The above command will have identical output to this:
./bls --from-secret 21aacfe4550d76551a0dfde9f632bdfc3ba8d9a983910c3e635f31ae6ce6c9ab -i 2,4,8 | jq

```

### Generating `N` seeds:
- If `--count N` is used with `--generate-seed`, the utility will generate `N` random seeds:
```sh
./bls --generate-seed --count 3
{"seeds":["5af028b010bfedb52ec0f635eb8c83ccef9539d4431ae803c69681878791715a","0bb029fc53884bd4599453a14828b31020eb41135c0d0829d290dc49d471b68d","d5e9d8edbad86432a1b871bc09bfbb59d241ab916f1cbc3fb01621274b5094e1"]}

# or
./bls -G -c 3
```
- The seeds are stored in an array under the `seeds` key

## Generate `X,Y,Z` child indices:
```sh
./bls -i 2,4,8
{"parent": {"secret": "0139bfbc0f4626d8817efcde60927699450c134e5528ff258be3a73966f59548","public": "801665837afeb647bcff2b3325307cc998d327486361a7fa9513f6e5dced54b4c5eef65dafd4d740bee63af451d09bb0"},"children": [{"index": 2,"secret": "0a91709770559aad2a19a65e8130e3cc3f91876e65709be5a9e5d91deff47dad","public": "99ac882a7605814e395f16179357c961df7fafcf8ff4fd64e080261db8661ef7cf53a13a772f82cf767c718701d77f89"},{"index": 4,"secret": "2ddca8d4094f200d1af0e9103a13fe20d0253af8e0ca1b1379bb6be875e1143e","public": "09ab18481eeb21a32793708b54495ff29eca27ba0afe48e87abd6df0ebcd8bec0b6a18f1306499576a2780a8af9d2a3e"},{"index": 8,"secret": "3128d6b24e871a306fb050ad73a89ea37c99315b9cb1879e00f68d6d08c03bbb","public": "8f9bfccdafe90dc9ebaa16959638fb65563ed988eeba5001a04f67a059adeea7c69ffc29908434788485e1bb89452c38"}]}
```

The above output prettified:
```json
{
  "parent": {
    "secret": "0139bfbc0f4626d8817efcde60927699450c134e5528ff258be3a73966f59548",
    "public": "801665837afeb647bcff2b3325307cc998d327486361a7fa9513f6e5dced54b4c5eef65dafd4d740bee63af451d09bb0"
  },
  "children": [
    {
      "index": 2,
      "secret": "0a91709770559aad2a19a65e8130e3cc3f91876e65709be5a9e5d91deff47dad",
      "public": "99ac882a7605814e395f16179357c961df7fafcf8ff4fd64e080261db8661ef7cf53a13a772f82cf767c718701d77f89"
    },
    {
      "index": 4,
      "secret": "2ddca8d4094f200d1af0e9103a13fe20d0253af8e0ca1b1379bb6be875e1143e",
      "public": "09ab18481eeb21a32793708b54495ff29eca27ba0afe48e87abd6df0ebcd8bec0b6a18f1306499576a2780a8af9d2a3e"
    },
    {
      "index": 8,
      "secret": "3128d6b24e871a306fb050ad73a89ea37c99315b9cb1879e00f68d6d08c03bbb",
      "public": "8f9bfccdafe90dc9ebaa16959638fb65563ed988eeba5001a04f67a059adeea7c69ffc29908434788485e1bb89452c38"
    }
  ]
}
```
- The JSON output will always have two top level keys `parent` and `children`.
	- The derived child index is at `children[N].index`
- The `-i` (or `--index`) options can work with `--seed`,`--from-secret`,or `--from-example-secret` (piped to `jq` for humans):
```sh
./bls --seed 5af028b010bfedb52ec0f635eb8c83ccef9539d4431ae803c69681878791715a --index 2,4,8 | jq
{
  "parent": {
    "secret": "21aacfe4550d76551a0dfde9f632bdfc3ba8d9a983910c3e635f31ae6ce6c9ab",
    "public": "054d6d2433d5eec5b2e547cf355d7c143f1491a56afc79f2e85ded3da32d1cbe448b08e41b995f5ccecc13c10877aaab"
  },
  "children": [
    {
      "index": 2,
      "secret": "6ba982226c50d68da5bc0d7b9496053746fba9300f239273a056326723f279c4",
      "public": "92415f3f4bd45acb939c35e4f2f9803baa37fed317e69173e43fbafa720dba122e34fc73c1758df10b17ac089e854e8f"
    },
    {
      "index": 4,
      "secret": "2e3910d4aa19626ebbca3d0566a1ab3a4df36774abd8a207272945607f49205d",
      "public": "928c7255623239fc3de9b0e9279f85b7e74441a9353eae0414ec9561fc710d05933d068d6aae92844ebfd2aca44e5841"
    },
    {
      "index": 8,
      "secret": "5f4c7a8d71cffea422982afa997798210da3dec39f4d7b15e3e6cb253de75fe6",
      "public": "8b35c325ec3f61f02de0f66ed700247bbb7c811c4eab8e8b42b53241ce7a35c4d6e2e800f7e435e3a7ea6294d01259b9"
    }
  ]
}

# or
./bls --seed 5af028b010bfedb52ec0f635eb8c83ccef9539d4431ae803c69681878791715a -i 2,4,8 | jq

# The above command will have identical output to this:
./bls --from-secret 21aacfe4550d76551a0dfde9f632bdfc3ba8d9a983910c3e635f31ae6ce6c9ab -i 2,4,8 | jq

```

## Generate a command to run dash-cli to verify a keypair

```sh
./bls --gen-dash-from-secret
./dash-cli bls fromsecret 30d8906b50234e92694e46a6038bf7c47a1b8dbe6d7343ddc90d87de43d0c0ab

EXPECTED OUTPUT:
{"secret":"30d8906b50234e92694e46a6038bf7c47a1b8dbe6d7343ddc90d87de43d0c0ab","public":"05377d51e89f71b330aaa49059e3f660373a20c53981561d58578b5471b3bbc1cf9acf2ab20902aaa62929b1fdeaee45"}

# or:

./bls -g
```

## Multiple verbose flags can increase verbosity. Up to 4 (which is lots of debug output)

```sh
./bls -v -v -v -v
[DEBUG][FILE:bls/src/test.cpp][LINE:563]: Generated a sk. Using that...
[ULTRA_DEBUG][FILE:bls/src/test.cpp][LINE:181]: Dumping json...
{"secret":"[ULTRA_DEBUG][FILE:bls/src/test.cpp][LINE:183]: Dumping serialized Private Key (SK)
493d0c5879cdf09c3c5fe5b2911fa829013ed8baa900d9d229c789fb071d3f98",[ULTRA_DEBUG][FILE:bls/src/test.cpp][LINE:185]: Dumped SK
"public":"[ULTRA_DEBUG][FILE:bls/src/test.cpp][LINE:187]: Dumping serialized private key (PK)
10d72b3712af1ad84f65d0fbf23b8823e6394cd23383ec1a15ffa0dfafff8cf0f649552e30cf45bd3c3706e9a8c6f871"[ULTRA_DEBUG][FILE:bls/src/test.cpp][LINE:189]: Dumped PK
}
[INFO][FILE:bls/src/test.cpp]: Done generating key pair JSON
```

## Print version info

```sh
./bls --version

# or:

./bls -V

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

1. Make sure you [download and install Cmake](https://cmake.org/download/)

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

-   [ ] Workflow for building on Linux arm64
-   [ ] Workflow for building on Windows x86-64, arm64

# Environments where this has been tested

-   [x] Devuan Chimaera x86-64
-   [x] MacOS / M1
-   [x] Raspberry Pi 4
-   [ ] Raspberry Pi 3
-   [x] Ubuntu Focal x86-64
-   [x] Ubuntu Jammy Jellyfish x86-64
-   [x] Ubuntu Jammy Jellyfish ARM64
-   [x] Debian Bullseye x86-64
-   [x] Windows x86-64 (experimental)
    -   note: Windows support is experimental as of version `v1.0.0`
    -   want to earn some Dash? If you help us QA the Windows version, we will pay you in Dash
