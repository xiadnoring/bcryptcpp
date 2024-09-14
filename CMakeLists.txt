cmake_minimum_required(VERSION 3.1)
project(bcryptcpp)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

# option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)
if (!WITH_CONAN)
    set(CMAKE_CXX_STANDARD 20)
endif ()

find_package(PkgConfig REQUIRED)

set(CPP_FORMAT_NOT_SUPPORT OFF)
if (CMAKE_CXX_STANDARD LESS 20)
    set(CPP_FORMAT_NOT_SUPPORT ON)
else ()
    add_compile_definitions("BCRYPT_CPP_FORMAT_SUPPORT")
endif ()

if (WITH_CONAN)
    message(STATUS "${PROJECT_NAME}: with conan build")
    if (CPP_FORMAT_NOT_SUPPORT)
        find_package(fmt REQUIRED)
    endif ()
else ()
    message(STATUS "${PROJECT_NAME}: default build")

    if (CPP_FORMAT_NOT_SUPPORT)
        pkg_check_modules(FMT REQUIRED fmt)
    endif ()
endif ()

if (BCRYPT_CPP_TEST)
    add_executable(${PROJECT_NAME} src/main.cpp
            src/bcryptcpp.cpp
            include/bcryptcpp.hpp
            include/errors.hpp
            src/legacy.h)
else ()
    add_library(${PROJECT_NAME} src/bcryptcpp.cpp
            include/bcryptcpp.hpp
            include/errors.hpp
            src/legacy.h)
endif ()
if (WITH_CONAN)
    if (CPP_FORMAT_NOT_SUPPORT)
        target_link_libraries(${PROJECT_NAME} fmt::fmt)
    endif ()
else ()
    if (CPP_FORMAT_NOT_SUPPORT)
        target_link_libraries(${PROJECT_NAME} PUBLIC ${FMT_LIBRARIES})
        target_include_directories(${PROJECT_NAME} PUBLIC ${FMT_INCLUDE_DIRS})
    endif ()
endif ()

target_include_directories(${PROJECT_NAME} PRIVATE include)

if (BCRYPT_CPP_TEST)
else ()
    set_target_properties(${PROJECT_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ${BUILD_SHARED_LIBS})
    target_compile_definitions(${PROJECT_NAME} PRIVATE "EXPORTING_BCRYPT")

    include(GNUInstallDirs)

    install(TARGETS ${PROJECT_NAME} EXPORT BcryptCppConfig
            ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
            LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
            RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR})
    install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})

    install(EXPORT BcryptCppConfig DESTINATION lib/cmake/${PROJECT_NAME})

    export(TARGETS ${PROJECT_NAME} FILE BcryptCppConfig.cmake)
endif ()