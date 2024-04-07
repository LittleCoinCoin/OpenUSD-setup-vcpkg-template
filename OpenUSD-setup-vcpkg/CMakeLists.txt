# ./OpenUSD-setup-vcpkg/CMakeList.txt

set(TargetName "OpenUSD-setup-vcpkg")

add_executable (${TargetName}

"src/main.cpp"

)

if(WIN32)
	message("Building for Windows: disabling min/max macros from minwin.h which collide with OpenUSD max member function in robin_growth_policy.h")
	add_compile_definitions(NOMINMAX)
endif()

set_property(TARGET ${TargetName} PROPERTY CXX_STANDARD 20)

find_package(TBB CONFIG REQUIRED)
target_link_libraries(${TargetName} PRIVATE TBB::tbb)

find_package(pxr CONFIG REQUIRED)
target_link_libraries(${TargetName} PRIVATE

ar
arch
gf
js
kind
ndr
pcp
plug
sdf
sdr
tf
trace
usd
usdGeom
usdHydra
usdLux
usdMedia
usdPhysics
usdRender
usdShade
usdSkel
usdUI
usdUtils
usdVol
vt
work
)


target_include_directories(${TargetName}
							PUBLIC
							"./headers"
)

add_custom_command(TARGET ${TargetName}
	POST_BUILD
	#COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:${TargetName}>/<plugin-subdir>"
	#COMMAND ${CMAKE_COMMAND} -E copy_if_different "<plugin-from-vcpkg-install>" "$<TARGET_FILE_DIR:${TargetName}>/<plugin-subdir>"
	
	
	COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/lib/usd/"
	
	# Easy solution: copy all directories from "/lib/usd" of OpenUSD installed
	# by vcpkg to the "/lib/usd" of your application.
	#COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different  "${CMAKE_BINARY_DIR}/vcpkg_installed/x64-windows/lib/usd" "${CMAKE_BINARY_DIR}/lib/usd"
	
	#base plugin file
	COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_BINARY_DIR}/vcpkg_installed/x64-windows/lib/usd/plugInfo.json" "${CMAKE_BINARY_DIR}/lib/usd/"
	
	#for the default asset resolver plugin
	#required even for the very basic action of creating a new stage
	COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/lib/usd/ar"
	COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different "${CMAKE_BINARY_DIR}/vcpkg_installed/x64-windows/lib/usd/ar" "${CMAKE_BINARY_DIR}/lib/usd/ar"
	
	#for the scene description format plugin
	#required even for the very basic action of creating a new stage
	COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/lib/usd/sdf"
	COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different "${CMAKE_BINARY_DIR}/vcpkg_installed/x64-windows/lib/usd/sdf" "${CMAKE_BINARY_DIR}/lib/usd/sdf"
	
	#for many things (judging by the number of plugin information given in the )
	#required even for the very basic action of creating a new stage
	COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/lib/usd/usd"
	COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different "${CMAKE_BINARY_DIR}/vcpkg_installed/x64-windows/lib/usd/usd" "${CMAKE_BINARY_DIR}/lib/usd/usd"
	
	#required for any of the built-in prims (sphere, mesh, etc.)
	COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/lib/usd/usdGeom"
	COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different "${CMAKE_BINARY_DIR}/vcpkg_installed/x64-windows/lib/usd/usdGeom" "${CMAKE_BINARY_DIR}/lib/usd/usdGeom"
)