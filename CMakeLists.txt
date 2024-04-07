# ./CMakeList.txt
cmake_minimum_required (VERSION 3.20)

project ("OpenUSD-setup-vcpkg")

# Because the USD plugin file directory resolution expects the dlls to be in a "/bin" folder
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin" CACHE STRING "" )

add_subdirectory ("OpenUSD-setup-vcpkg")
