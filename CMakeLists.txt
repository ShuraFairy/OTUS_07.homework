cmake_minimum_required(VERSION 3.5)

if ($ENV{GITHUB_RUN_NUMBER})
    project (bulk VERSION 0.0.$ENV{GITHUB_RUN_NUMBER})
else()
    project (bulk VERSION 0.0.1)
endif()

find_package(Doxygen)

option(WITH_BOOST_TEST "Whether to build Boost test" ON)
option(WITH_GOOGLE_TEST "Whether to build Google test" ON)
message(STATUS "Enable testing: ${WITH_GOOGLE_TEST}")

configure_file(version.h.in version.h)

add_executable(bulk main.cpp commands.cpp logger.cpp)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set_target_properties(bulk PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

if(WITH_GOOGLE_TEST)
    include(FetchContent)

    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        release-1.11.0
    )

    FetchContent_GetProperties(googletest)

    if(NOT googletest_POPULATED)
        FetchContent_Populate(googletest)
        # Prevent GoogleTest from overriding our compiler/linker options
        # when building with Visual Studio
        set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
        # Prevent GoogleTest from using PThreads
        set(gtest_disable_pthreads ON CACHE BOOL "" FORCE)

        # adds the targers: gtest, gtest_main, gmock, gmock_main
        add_subdirectory(
        ${googletest_SOURCE_DIR}
        ${googletest_BINARY_DIR}
        )

    # Silence std::tr1 warning on MSVC
    if(MSVC)
        foreach(_tgt gtest gtest_main gmock gmock_main)
            target_compile_definitions(${_tgt}
            PRIVATE
                "_SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING"
            )
        endforeach()
    endif()
endif()

    add_executable(google_test google_unit_test.cpp commands.cpp logger.cpp)
        
    if(MSVC)
       set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
    else(MSVC)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
    endif(MSVC)

    target_link_libraries(google_test gtest gtest_main)
endif()

if(WITH_BOOST_TEST)
    find_package(Boost COMPONENTS unit_test_framework REQUIRED)
    add_executable(boost_test boost_unit_test.cpp commands.cpp logger.cpp)

    set_target_properties(boost_test PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
    )

    set_target_properties(boost_test PROPERTIES
        COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
        INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
    )

    target_link_libraries(boost_test
        ${Boost_LIBRARIES}
    )
endif()

if (MSVC)
    target_compile_options(bulk PRIVATE
        /W4
    )    
    if(WITH_BOOST_TEST)
        target_compile_options(boost_test PRIVATE
            /W4
        )
    endif()
    if(WITH_GOOGLE_TEST)
        target_compile_options(google_test PRIVATE
            /W4
        )
    endif()
else ()
    target_compile_options(bulk PRIVATE
        -Wall -Wextra -pedantic -Werror
    )    
    if(WITH_BOOST_TEST)
        target_compile_options(boost_test PRIVATE
            -Wall -Wextra -pedantic -Werror
        )
    endif()
    if(WITH_GOOGLE_TEST)
        target_compile_options(google_test PRIVATE
            -Wall -Wextra -pedantic -Werror
        )
    endif()
endif()

install(TARGETS bulk RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)

if(WITH_BOOST_TEST)
    enable_testing()
    add_test(boost_test boost_test)
endif()

if(WITH_GOOGLE_TEST)
    enable_testing()
    add_test(google_test google_test)
endif()

