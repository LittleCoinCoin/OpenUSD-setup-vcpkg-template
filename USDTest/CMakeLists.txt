# ./USDTest/CMakeList.txt


add_executable (USDTest

"src/main.cpp"

)

if(WIN32)
	message("Building for Windows: disabling min/max macros from minwin.h which collide with OpenUSD max member function in robin_growth_policy.h")
	add_compile_definitions(NOMINMAX)
endif()

set_property(TARGET USDTest PROPERTY CXX_STANDARD 20)

find_package(TBB CONFIG REQUIRED)
target_link_libraries(USDTest PRIVATE TBB::tbb)

find_package(pxr CONFIG REQUIRED)
target_link_libraries(USDTest PRIVATE

#ar
#arch
#gf
#js
#kind
#ndr
#pcp
#plug
#sdf
#sdr
#tf
#trace
usd
#usdGeom
#usdHydra
#usdLux
#usdMedia
#usdPhysics
#usdRender
#usdShade
#usdSkel
#usdUI
#usdUtils
#usdVol
#vt
#work


)


target_include_directories(USDTest
							PUBLIC
							"./headers"
)