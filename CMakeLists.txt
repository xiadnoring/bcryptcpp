cmake_minimum_required(VERSION 3.1)
project(bcryptcpp)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

# option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)

if (WITH_CONAN)
    message(STATUS "${PROJECT_NAME}: with conan build")
    find_package(fmt REQUIRED)
else ()
    message(STATUS "${PROJECT_NAME}: default build")
    find_package(PkgConfig REQUIRED)

    pkg_check_modules(FMT REQUIRED fmt)
endif ()

set(CMAKE_CXX_STANDARD 17)

if (BCRYPT_CPP_TEST)
    add_executable(${PROJECT_NAME} src/main.cpp
            src/bcryptcpp.cpp
            include/bcryptcpp.h
            include/errors.h
            src/legacy.h)
else ()
    add_library(${PROJECT_NAME} src/bcryptcpp.cpp
            include/bcryptcpp.h
            include/errors.h
            src/legacy.h)
endif ()
if (WITH_CONAN)
    target_link_libraries(${PROJECT_NAME} fmt::fmt)
else ()
    target_link_libraries(${PROJECT_NAME} PUBLIC ${FMT_LIBRARIES})
    target_include_directories(${PROJECT_NAME} PUBLIC ${FMT_INCLUDE_DIRS})
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