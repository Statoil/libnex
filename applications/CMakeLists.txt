project(nex-apps CXX)

###
# old c++ executable
##
# add_executable(nex2ecl nex2ecl.cpp)
#
# target_include_directories(nex2ecl PRIVATE ../lib/src/include)
# target_link_libraries(nex2ecl PRIVATE nex)
# if (HAVE_WINSOCK2_H)
#     target_link_libraries(nex2ecl PRIVATE ws2_32)
# endif()
# target_compile_options(nex2ecl PRIVATE ${CXX_STANDARD})

install(PROGRAMS nex2ecl DESTINATION ${CMAKE_INSTALL_BINDIR})
