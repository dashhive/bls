#!/bin/bash

CMAKE_URL=https://github.com/Kitware/CMake/releases/download/v3.24.2/cmake-3.24.2.tar.gz
UTILS_DIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &>/dev/null && pwd)
UTILS_DIR="${UTILS_DIR}/utils"

SOURCE_DIR="$1"
TARGZ="$2"
BRANCH="$3"

echo $SOURCE_DIR
ls $SOURCE_DIR

function get_cmake(){
	CURL_OPTS='-4 -L'

	CMAKE_DOWNLOAD="${UTILS_DIR}/cmake.tar.gz"
	CMAKE_GZ="${UTILS_DIR}/cmake.tar.gz"
	CMAKE_TAR="${UTILS_DIR}/cmake.tar"
	echo "CMAKE_DOWNLOAD: $CMAKE_DOWNLOAD"

	if [[ ! -d $UTILS_DIR ]]; then
		mkdir $UTILS_DIR
	fi

	if [[ ! -f "$CMAKE_DOWNLOAD" ]]; then
		curl $CURL_OPTS $CMAKE_URL -o "$CMAKE_DOWNLOAD"
	fi

	cd "$UTILS_DIR"

	gzip -d "$CMAKE_GZ"
	tar xvf "$CMAKE_TAR"
}

function build_cmake(){
	cd "$UTILS_DIR"

	cd 'cmake-3.24.2'

	pwd

	CMAKE_INSTALL="${UTILS_DIR}/cmake"
	mkdir -p "$CMAKE_INSTALL"

	./configure --prefix=${CMAKE_INSTALL}

	make

	make install
}

get_cmake

xcode-select --install

export PATH="${UTILS_DIR}/cmake/bin:$PATH"

cd $SOURCE_DIR

BUILD_DIR=${SOURCE_DIR}/build
if [[ -d "${BUILD_DIR}" ]]; then
	rm -rf "${BUILD_DIR}"
fi

mkdir "${BUILD_DIR}"
cd "${BUILD_DIR}"
cmake ..

make 

RUNTEST="${BUILD_DIR}/src/runtest"
EXE="${BUILD_DIR}/bls"

mv "$RUNTEST" "$EXE"

chmod +x "$EXE"

$EXE -s |
	tr -d '[:space:]' |
	grep '{"secret":"377091f0e728463bc2da7d546c53b9f6b81df4a1cc1ab5bf29c5908b7151a32d","public":"06243290bbcbfd9ae75bdece7981965350208eb5e99b04d5cd24e955ada961f8c0a162dee740be7bdc6c3c0613ba2eb1"}'

if [[ $? -ne 0 ]]; then
	echo 'deterministic keypair invalid'
else
	echo 'SUCCESS'
fi

cd "${BUILD_DIR}"
tar cvzf $TARGZ bls
