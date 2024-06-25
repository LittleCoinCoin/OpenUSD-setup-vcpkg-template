# ./CMakeList.txt
cmake_minimum_required (VERSION 3.20)

set(TargetName "OpenUSD-setup-vcpkg")
project (${TargetName})

# Because the USD plugin file directory resolution expects the dlls to be in a "/bin" folder
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${TargetName}/$<CONFIG>/bin" CACHE STRING "" )

add_subdirectory (${TargetName})
