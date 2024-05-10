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

	#### Copy extras to the binary directory to bring a few files useful for the replication of the tutorials. ####
	COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/bin/extras"
	COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different "${CMAKE_HOME_DIRECTORY}/extras" "${CMAKE_BINARY_DIR}/bin/extras"


	#### Copy the required plugins to the binary directory. ####
	#### Mandatory for the application to run. ####
	COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/${TargetName}/lib/usd/"
	
	# Easy solution: copy all directories from "/lib/usd" of OpenUSD installed
	# by vcpkg to the "/lib/usd" of your application.
	#COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different  "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib/usd" "${CMAKE_BINARY_DIR}/${TargetName}/lib/usd"
	
	#base plugin file
	COMMAND ${CMAKE_COMMAND} -E copy_if_different "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib/usd/plugInfo.json" "${CMAKE_BINARY_DIR}/${TargetName}/lib/usd/"
	
	#for the default asset resolver plugin
	#required even for the very basic action of creating a new stage
	COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/${TargetName}/lib/usd/ar"
	COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib/usd/ar" "${CMAKE_BINARY_DIR}/${TargetName}/lib/usd/ar"
	
	#for the scene description format plugin
	#required even for the very basic action of creating a new stage
	COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/${TargetName}/lib/usd/sdf"
	COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib/usd/sdf" "${CMAKE_BINARY_DIR}/${TargetName}/lib/usd/sdf"
	
	#for many things (judging by the number of plugin information given in the )
	#required even for the very basic action of creating a new stage
	COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/${TargetName}/lib/usd/usd"
	COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib/usd/usd" "${CMAKE_BINARY_DIR}/${TargetName}/lib/usd/usd"
	
	#required for any of the built-in prims (sphere, mesh, etc.)
	COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/${TargetName}/lib/usd/usdGeom"
	COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib/usd/usdGeom" "${CMAKE_BINARY_DIR}/${TargetName}/lib/usd/usdGeom"
	
	#required for materials, introduced in the tutorial on simple shading (TestFunction_PixarTutorial_SimpleShading)
	COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/${TargetName}/lib/usd/usdShade"
	COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib/usd/usdShade" "${CMAKE_BINARY_DIR}/${TargetName}/lib/usd/usdShade"
)