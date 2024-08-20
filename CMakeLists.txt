cmake_minimum_required(VERSION 3.1)
project(bcryptcpp)

find_package(libxcrypt REQUIRED)
find_package(fmt REQUIRED)

set(CMAKE_CXX_STANDARD 14)

add_library(${PROJECT_NAME} src/bcrypt.cpp
        include/bcrypt.h
        include/errors.h)

target_link_libraries(${PROJECT_NAME} libxcrypt::libxcrypt fmt::fmt)
target_include_directories(${PROJECT_NAME} PRIVATE include)

include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME} EXPORT BcryptCppConfig
        ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR})
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})

install(EXPORT BcryptCppConfig DESTINATION lib/cmake/${PROJECT_NAME})

export(TARGETS ${PROJECT_NAME} FILE BcryptCppConfig.cmake)