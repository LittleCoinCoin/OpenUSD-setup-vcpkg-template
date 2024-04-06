
#include <iostream>
#include <string>
#include "main.h"

/*!
@brief The most basic test function that creates a new USD stage.
@details I expect this function to run right away without any issues.
		 In fact, it requires at least the plugins "ar", "sdf", and "usd"
		 at runtime.
*/
void TestFunction_StageCreation()
{
	// Create a new stage
	pxr::UsdStageRefPtr stage = pxr::UsdStage::CreateInMemory();
}

/*!
@brief Function reproducing the first example of the Pixar USD tutorial.
@see https://openusd.org/release/tut_helloworld.html 
	 https://openusd.org/release/tut_helloworld_redux.html
*/
void TestFunction_PixarTutorial_HelloWorld()
{
	/* Python code from the tutorial
		
		from pxr import Usd, UsdGeom
		stage = Usd.Stage.CreateNew('HelloWorld.usda')
		xformPrim = UsdGeom.Xform.Define(stage, '/hello')
		spherePrim = UsdGeom.Sphere.Define(stage, '/hello/world')
		stage.GetRootLayer().Save()
	*/

	//Write the C++ equivalent of the above python code
	pxr::UsdStageRefPtr stage = pxr::UsdStage::CreateNew("HelloWorld.usda");
	pxr::UsdGeomXform xformPrim = pxr::UsdGeomXform::Define(stage, pxr::SdfPath("/hello"));
	pxr::UsdGeomSphere spherePrim = pxr::UsdGeomSphere::Define(stage, pxr::SdfPath("/hello/world"));

	std::string fileResult;
	stage->GetRootLayer()->ExportToString(&fileResult);
	std::cout << fileResult << std::endl;

	stage->GetRootLayer()->Save(); // Save the stage at the same location as the application.
}

int main()
{

	TestFunction_StageCreation();

	TestFunction_PixarTutorial_HelloWorld();

	std::cout << "End of main." << std::endl;

	return 0;
}
