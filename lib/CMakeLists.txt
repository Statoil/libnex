project(libnex CXX)
include(GNUInstallDirs)

add_library(nex src/nexus_plot.cpp
                src/unit.cpp
)

target_compile_definitions(nex PRIVATE ${ntohl})
target_compile_options(nex PRIVATE ${CPP_STANDARD})
target_include_directories(nex PUBLIC src/include)
target_link_libraries(nex PUBLIC ecl)

install(TARGETS nex
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})


if(NOT BUILD_TESTING)
    return()
endif()

file(GLOB plts "${testdata}/*.plt")
foreach (plt ${plts})
    get_filename_component(fl ${plt} NAME)
    configure_file(${plt} test-data/${fl} COPYONLY)
endforeach ()

add_executable(testsuite tests/testsuite.cpp tests/load.cpp tests/spe1.cpp
               tests/units.cpp)
target_link_libraries(testsuite catch2 nex)
target_include_directories(testsuite PRIVATE src/include)
target_compile_options(testsuite PRIVATE ${CPP_STANDARD})
add_test(NAME load COMMAND testsuite [load])
add_test(NAME spe1 COMMAND testsuite [spe1])
add_test(NAME units COMMAND testsuite [units])
