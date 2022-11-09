# bls

Implements [BLS signatures](https://github.com/Chia-Network/bls-signatures) with aggregation using [relic toolkit](https://github.com/relic-toolkit/relic) for cryptographic primitives (pairings, EC, hashing) according to the [IETF BLS RFC](https://datatracker.ietf.org/doc/draft-irtf-cfrg-bls-signature/) with [these curve parameters](https://datatracker.ietf.org/doc/draft-irtf-cfrg-pairing-friendly-curves/) for BLS12-381.

# Table of Contents

-   Usage
-   Install
-   Examples
-   Build

# Usage

```sh
bls
```

```json
{
    "secret": "003f500d9a6fecda122ce9db1e8c931f05758517806cfe9060f2c7c1035c0a67",
    "public": "0ffaa0ea8a1a665cbabc7fea1c588476ae954026eb7647bb6043e0958d3cbf004cca7bf57af1646b92b9d1205bd25911"
}
```

# Options

```sh
  --seed N                            : use hexadecimal seed N where N is 64 hexadecimal characters
  -F S,fromsecret S, --from-secret S  : generate keypairs using S as the secret key
  -g, --generate-seed                 : outputs a randomly generated seed that can be passed to --seed
  -c N, --count N                     : if -g is also specified, generate N random seeds
  -i N, --index N                     : derive the Nth child keypair. can be a CSV
  -s, --from-example-seed             : generate keypairs using static seed
  -h, --help                          : print this help screen
  -v, --verbose                       : print verbose output. can be specified multiple times
  -V, --version                       : print version
  -j, --print-json                    : print JSON keypairs (default)
```

# Install

## Mac, Linux

```sh
curl -sS https://webi.sh/bls | sh
```

## Windows

```pwsh
curl.exe https://webi.ms/bls | powershell
```

## GitHub Releases

1. Download from <https://github.com/dashhive/bls/releases>
2. Move the `bls` file to a place in your PATH, such as `~/bin/`
3. **_macOS only_** remove the quarantine bit:
    ```sh
    xattr -d com.apple.quarantine ~/bin/xsv
    ```

# Example Usage

## Generate Example Secret

```sh
bls --from-example-secret

# same as
bls --seed 003206f418c701193458c013120c5906dc12663ad1520c3e596eb6092c14fe16
```

```json
{
    "secret": "377091f0e728463bc2da7d546c53b9f6b81df4a1cc1ab5bf29c5908b7151a32d",
    "public": "06243290bbcbfd9ae75bdece7981965350208eb5e99b04d5cd24e955ada961f8c0a162dee740be7bdc6c3c0613ba2eb1"
}
```

## Generate Public Key from Secret

```sh
bls --from-secret 377091f0e728463bc2da7d546c53b9f6b81df4a1cc1ab5bf29c5908b7151a32d
```

```json
{
    "secret": "377091f0e728463bc2da7d546c53b9f6b81df4a1cc1ab5bf29c5908b7151a32d",
    "public": "06243290bbcbfd9ae75bdece7981965350208eb5e99b04d5cd24e955ada961f8c0a162dee740be7bdc6c3c0613ba2eb1"
}
```

## Generate seeds:

```sh
bls --generate-seed --count 1
```

```json
{
    "seeds": [
        "f1804bbbde425df07be251ee98db14dec28998c2c0b4341a6ebacdb357771b61"
    ]
}
```

## Generate Child keypairs:

```sh
bls --seed 003206f418c701193458c013120c5906dc12663ad1520c3e596eb6092c14fe16 -i 2,4

# same as
bls --from-secret 377091f0e728463bc2da7d546c53b9f6b81df4a1cc1ab5bf29c5908b7151a32d -i 2,4
```

```json
{
    "parent": {
        "secret": "377091f0e728463bc2da7d546c53b9f6b81df4a1cc1ab5bf29c5908b7151a32d",
        "public": "06243290bbcbfd9ae75bdece7981965350208eb5e99b04d5cd24e955ada961f8c0a162dee740be7bdc6c3c0613ba2eb1"
    },
    "children": [
        {
            "index": 2,
            "secret": "0950f6fda6659e63436fdfeb8d1f77cc50c898230a2cffc28cbd2f69ddb9455c",
            "public": "0847e5908729e738ca4795d1340e67d083b5e2e7429176f415daf85050b1ceef5489eb9832afe73b2195231fdf9e5b79"
        },
        {
            "index": 4,
            "secret": "4623da0da2814d6946e7a8e1164bebf3a686532a53152b56458086343288ef42",
            "public": "90eda32100c9c7bb2970166135af8a686a00f5fef2af2efd56e27c154d3876f9156c627463b1b8fd253fb583d5abe0b7"
        }
    ]
}
```

# Building Manually

## Linux

```sh
sudo apt -y install build-essential cmake
git clone 'https://github.com/dashhive/bls'
cd ./bls/

mkdir ./build/
cd ./build/
cmake ..

make

# copy the binary to some place within your path (optional)
# cp ./src/runtest ~/bin/bls

bls --version
```

## Windows

Make sure you [download and install Cmake](https://cmake.org/download/)

```sh
git clone 'https://github.com/dashhive/bls'
cd bls

mkdir build
cd build
cmake ..
```

The above commands will create a `bls.sln` that you will have to open in [Visual Studio](https://visualstudio.microsoft.com/)

**IMPORTANT**: Visual Studio is not the same as Visual Studio Code!

The community edition of Visual Studio is free.

Once you've created the `bls.sln` file, you will need to open that inside of Visual Studio.

Part of the build targets is `runtest`. Right click on that and click `Build`.

That will create a `bls.exe` file depending on what your target is. Out of the box, you may find that exe to exist somewhere in `bls\build\Debug\src\bls.exe`. Using `explorer.exe`'s find function would be the best way to find this. Or in Visual Studio you might be able to browse to where that exe is.

# Automating releases via GitHub Actions

If you create a tag that starts with 'v' and is followed by numbers and dots, then the github actions workflow will be triggered and a build will be created along with a release.

## Preqrequisites

-   You must setup the following secret:

```sh
PERSONAL_ACCESS_TOKEN
```

And set the value to whoever's personal access token that will be used to call the releases API. See the Settings page for how to setup your secrets.

To trigger a release, you might run the following script:

```sh
git tag v1.1.0
git push origin --tags
```
