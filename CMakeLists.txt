cmake_minimum_required(VERSION 2.8.12)
project(nex)

include(CheckFunctionExists)
include(CheckIncludeFile)
include(CTest)

find_package(ecl REQUIRED)

check_include_file(netinet/in.h HAVE_NETINET_IN_H)
check_include_file(arpa/inet.h  HAVE_ARPA_INET_H)
check_include_file(winsock2.h   HAVE_WINSOCK2_H)
if (HAVE_NETINET_IN_H)
    list(APPEND ntohl -DHAVE_NETINET_IN_H)
elseif (HAVE_ARPA_INET_H)
    list(APPEND ntohl -DHAVE_ARPA_INET_H)
elseif (HAVE_WINSOCK2_H)
    set(ws2 ws2_32)
    list(APPEND ntohl -DHAVE_WINSOCK2_H)
else()
    message(FATAL_ERROR "Could not find htons")
endif()

set(CXX_STANDARD -std=c++11 CACHE STRING "C++ revision flag")
set(testdata ${CMAKE_CURRENT_SOURCE_DIR}/tests/data)

add_subdirectory(lib)
add_subdirectory(applications)
