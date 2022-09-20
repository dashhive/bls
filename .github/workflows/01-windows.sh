#!/bin/bash

CMAKE_URL=https://github.com/Kitware/CMake/releases/download/v3.24.2/cmake-3.24.2.tar.gz
UTILS_DIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &>/dev/null && pwd)
UTILS_DIR="${UTILS_DIR}/utils"

SOURCE_DIR="$1"
ARCHITECTURE="$2"

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

#get_cmake
#
#export PATH="${UTILS_DIR}/cmake/bin:$PATH"

cd $SOURCE_DIR

BUILD_DIR=${SOURCE_DIR}/build
#if [[ -d "${BUILD_DIR}" ]]; then
#	rm -rf "${BUILD_DIR}"
#fi

mkdir build
cd build
cmake ..

cmake --build .
