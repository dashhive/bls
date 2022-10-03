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
	grep '{"secret":"003206f418c701193458c013120c5906dc12663ad1520c3e596eb6092c14fe16","public":"86267afa0bc64fb10757afa93198acaf353b11fae85d19e7265f3825abe70501e68c5bc7c816c3c57b1ff7a74298a32f"}'

if [[ $? -ne 0 ]]; then
	echo 'deterministic keypair invalid'
else
	echo 'SUCCESS'
fi

cd "${BUILD_DIR}"
tar cvzf $TARGZ bls
