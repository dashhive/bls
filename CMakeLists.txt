CMAKE_MINIMUM_REQUIRED(VERSION 3.14.0 FATAL_ERROR)
set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
#set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
#set(BUILD_SHARED_LIBS OFF)
#set(CMAKE_EXE_LINKER_FLAGS "-static")

IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE "RELEASE")
ENDIF()

project(BLS)

#set(BUILD_BLS_PYTHON_BINDINGS 1 CACHE INTEGER "")
set(BUILD_BLS_TESTS 1 CACHE INTEGER "")
#set(BUILD_BLS_BENCHMARKS 0 CACHE INTEGER "")

#message(STATUS "Build python bindings: ${BUILD_BLS_PYTHON_BINDINGS}")
message(STATUS "Build tests: ${BUILD_BLS_TESTS}")
#message(STATUS "Build benchmarks: ${BUILD_BLS_BENCHMARKS}")

# Add path for custom modules
set(CMAKE_MODULE_PATH
	${CMAKE_MODULE_PATH}
	${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules
)

# Make sure to statically link libsodium when building wheels on ci
#if(DEFINED ENV{CIBUILDWHEEL})
  set(sodium_USE_STATIC_LIBS TRUE)
#  message("libsodium will be statically linked because $CIBUILDWHEEL is defined.")
#endif()

#find_package(sodium)
#if (SODIUM_FOUND)
#  message(STATUS "Found libsodium")
#  message(STATUS "Sodium include dir = ${sodium_INCLUDE_DIR}")
#  set(BLSALLOC_SODIUM "1" CACHE STRING "")
#  include_directories(${sodium_INCLUDE_DIR})
#endif()

set(STBIN TRUE)
set(STLIB "ON" CACHE STRING "")
#find_package(gmp)
#if (GMP_FOUND)
#  message(STATUS "Found libgmp")
#  set(ARITH "gmp" CACHE STRING "")
#else()
  set(ARITH "easy" CACHE STRING "")
	#endif()

set(TIMER "CYCLE" CACHE STRING "")
set(CHECK "off" CACHE STRING "")
set(VERBS "off" CACHE STRING "")
set(ALLOC "AUTO" CACHE STRING "")
set(SHLIB "OFF" CACHE STRING "")
set(STBIN TRUE)
set(STLIB "ON" CACHE STRING "")
set(DOCUM "off" CACHE STRING "")

set(FP_PRIME 381 CACHE INTEGER "")

IF (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  set(SEED "UDEV" CACHE STRING "")
  set(FP_QNRES "off" CACHE STRING "")
	set(MULTI "PTHREAD" CACHE STRING "")
ELSEIF (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  set(SEED "WCGR" CACHE STRING "")
  set(FP_QNRES "on" CACHE STRING "")
ELSE()
	set(MULTI "PTHREAD" CACHE STRING "")
	set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
	set(BUILD_SHARED_LIBS OFF)
	set(CMAKE_EXE_LINKER_FLAGS "-static")
  set(SEED "UDEV" CACHE STRING "")
  set(FP_QNRES "on" CACHE STRING "")
ENDIF()
set(STBIN "ON" CACHE STRING "")

set(FP_METHD "INTEG;INTEG;INTEG;MONTY;LOWER;SLIDE" CACHE STRING "")
set(COMP "-O3 -funroll-loops -fomit-frame-pointer" CACHE STRING "")
set(FP_PMERS "off" CACHE STRING "")
set(FPX_METHD "INTEG;INTEG;LAZYR" CACHE STRING "")
set(EP_PLAIN "off" CACHE STRING "")
set(EP_SUPER "off" CACHE STRING "")
# Disable relic tests and benchmarks
set(TESTS 0 CACHE INTEGER "")
set(BENCH 0 CACHE INTEGER "")
set(QUIET 1 CACHE INTEGER "")

set(PP_EXT "LAZYR" CACHE STRING "")
set(PP_METHD "LAZYR;OATEP" CACHE STRING "")

add_subdirectory(src)
