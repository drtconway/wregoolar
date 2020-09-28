cmake_minimum_required(VERSION 3.16)

find_package(Boost REQUIRED COMPONENTS unit_test_framework)

set(testsources
    basic.cpp)

foreach(testsource ${testsources})
    get_filename_component(exename wregoolar-test-${testsource} NAME_WE)
    add_executable(${exename} ${testsource})
    set_target_properties(${exename} PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
    )
    target_include_directories(${exename} PRIVATE ${Boost_INCLUDE_DIRS})
    target_link_libraries(${exename} Boost::unit_test_framework)
    add_test(NAME ${exename} COMMAND ${exename})
endforeach(testsource)
