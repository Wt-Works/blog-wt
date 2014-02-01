## GENERAL ##

cmake_minimum_required(VERSION 2.8)
set(WT_INSTALL_DIR $ENV{WT_INSTALL_DIR}) #provide wt install directory in environment var
#set(WT_INSTALL_DIR "/path/to/wt-install-dir") #alternatively, provide it here
include_directories(${WT_INSTALL_DIR}/include)
link_directories(${WT_INSTALL_DIR}/lib) 

## PROJECT SPECIFIC ##

PROJECT(BLOG)

## SOURCES ##

file(GLOB_RECURSE sources src/*.c*)
file(GLOB_RECURSE headers src/*.h src/*.hpp) # optional, for use in generated IDE projects

## TARGET ##

set(TARGET blog.wt)
add_executable(${TARGET} ${sources} ${headers})

## LINKING ##

set(WT_CONNECTOR "wthttp" CACHE STRING "Connector used (wthttp or wtfcgi)")
set(CMAKE_BUILD_TYPE Debug CACHE STRING " Debug Release RelWithDebInfo MinSizeRel" FORCE )

target_link_libraries(
  ${TARGET}
  optimized ${WT_CONNECTOR} debug ${WT_CONNECTOR}d
  optimized wt              debug wtd
  optimized wtdbo           debug wtdbod
  optimized wtdbosqlite3    debug wtdbosqlite3d
)

## IDE and CONVENIENCE ##

source_group(sources FILES ${sources}) # for some IDE's (MSVC), to show files grouped
source_group(headers FILES ${headers})

## TODO ##

# ASCIIDOC #

# CRYPT #

# Test whether crypt(3) is provided by libc. If it's not, check if
# libcrypt exists and if it provides crypt(3).

#INCLUDE(CheckFunctionExists)
#INCLUDE(CheckLibraryExists)

#CHECK_FUNCTION_EXISTS(crypt CRYPT_EXISTS)
#IF(NOT CRYPT_EXISTS)
#  CHECK_LIBRARY_EXISTS(crypt crypt "" CRYPT_LIB_EXISTS)
#  IF(CRYPT_LIB_EXISTS)
#    SET(CMAKE_REQUIRED_LIBRARIES "crypt")
#    CHECK_FUNCTION_EXISTS(crypt CRYPT_EXISTS)
#  ENDIF(CRYPT_LIB_EXISTS)
#ENDIF(NOT CRYPT_EXISTS)

#IF(CRYPT_LIB_EXISTS)
#  TARGET_LINK_LIBRARIES(${TARGET} crypt)
#ENDIF(CRYPT_LIB_EXISTS)
