#!/bin/bash

MAKE_CORES=1

CMAKE_URL=https://github.com/Kitware/CMake/releases/download/v3.24.2/cmake-3.24.2.tar.gz
UTILS_DIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &>/dev/null && pwd)
UTILS_DIR="${UTILS_DIR}/utils"

# TODO: FIXME: point this to the base directory of bls-signatures
SOURCE_DIR="/home/runner/work/bls-signatures-cli-tool/bls-signatures-cli-tool/"

echo $SOURCE_DIR
ls $SOURCE_DIR

function get_cmake(){

	# TODO: FIXME: remove socks5 stuff
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

function get_build_essential(){
	sudo apt -y install build-essential gcc g++ make libssl-dev
}


function build_cmake(){
	cd "$UTILS_DIR"

	cd 'cmake-3.24.2'

	pwd

	CMAKE_INSTALL="${UTILS_DIR}/cmake"
	mkdir -p "$CMAKE_INSTALL"

	./configure --prefix=${CMAKE_INSTALL}

	make -j "$MAKE_CORES"

	make install
}

get_build_essential

get_cmake

export PATH="${UTILS_DIR}/cmake/bin:$PATH"

cd $SOURCE_DIR

BUILD_DIR=${SOURCE_DIR}/build
if [[ -d "${BUILD_DIR}" ]]; then
	rm -rf "${BUILD_DIR}"
fi

mkdir "${BUILD_DIR}"
cd "${BUILD_DIR}"
cmake ..

make -j "$MAKE_CORES"
