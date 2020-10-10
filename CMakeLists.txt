# CMakeList.txt : CMake project for rf-linux, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)
project ("rushfiles-filesystem")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_MODULE_PATH "${CMAKE_HOME_DIRECTORY}/CMake" ${CMAKE_MODULE_PATH})

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_FILE_OFFSET_BITS=64")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_FILE_OFFSET_BITS=64")

find_package(nlohmann_json CONFIG REQUIRED)
find_package(cpprestsdk CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

find_package(FUSE REQUIRED)
include_directories(${FUSE_INCLUDE_DIR})

# Add source to this project's executable.
add_executable (${PROJECT_NAME} "rf-linux.cpp" "rf-linux.h" "rushfiles/models.h" "rushfiles/client.h" "rushfiles/client.cpp" "rushfiles/share.h")

target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json nlohmann_json::nlohmann_json)
target_link_libraries(${PROJECT_NAME} PRIVATE cpprestsdk::cpprest cpprestsdk::cpprestsdk_zlib_internal cpprestsdk::cpprestsdk_boost_internal cpprestsdk::cpprestsdk_openssl_internal)
target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt fmt::fmt-header-only)

target_link_libraries(${PROJECT_NAME} PUBLIC ${FUSE_LIBRARIES})

# TODO: Add tests and install targets if needed.
