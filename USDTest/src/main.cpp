
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
	std::cout << "** TestFunction_PixarTutorial_HelloWorld **" << std::endl;

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
	std::cout << "Content of file HelloWorld.usda:\n" << fileResult << std::endl;

	stage->GetRootLayer()->Save(); // Save the stage at the same location as the application.
}

/*!
@brief Function reproducing the second example of the Pixar USD tutorial.
@see https://openusd.org/release/tut_inspect_and_author_props.html
*/
void TestFunction_PixarTutorial_InspectAndAuthorProperties()
{
	std::cout << "** TestFunction_PixarTutorial_InspectAndAuthorProperties **" << std::endl;
	
	/*! Python code from the tutorial
	
		from pxr import Usd, Vt
		stage = Usd.Stage.Open('HelloWorld.usda')
		xform = stage.GetPrimAtPath('/hello')
		sphere = stage.GetPrimAtPath('/hello/world')
	
	*/

	//Write the C++ equivalent of the above python code
	pxr::UsdStageRefPtr stage = pxr::UsdStage::Open("HelloWorld.usda");
	pxr::UsdPrim xform = stage->GetPrimAtPath(pxr::SdfPath("/hello"));
	pxr::UsdPrim sphere = stage->GetPrimAtPath(pxr::SdfPath("/hello/world"));

	/*! Python code from the tutorial
	
		# Inspect the properties of the sphere
		xform.GetPropertyNames()
		# Output:['proxyPrim', 'purpose', 'visibility', 'xformOpOrder']

		sphere.GetPropertyNames()
		# Output: ['doubleSided', 'extent', 'orientation', 'primvars:displayColor',
					'primvars:displayOpacity', 'proxyPrim', 'purpose', 'radius',
					'visibility', 'xformOpOrder']

		# Author a value to the extent property
		extentAttr = sphere.GetAttribute('extent')
		extentAttr.Get()
		# Output: Vt.Vec3fArray(2, (Gf.Vec3f(-1.0, -1.0, -1.0), Gf.Vec3f(1.0, 1.0, 1.0)))
	
		# Author a value to the radius property
		radiusAttr = sphere.GetAttribute('radius')
		radiusAttr.Set(2)
		# Output: True
		extentAttr.Set(extentAttr.Get() * 2)
		# Output: True

		sphereSchema = UsdGeom.Sphere(sphere)
		color = sphereSchema.GetDisplayColorAttr()
		color.Set([(0,0,1)])
		
		# Save the stage
		stage.GetRootLayer().Save()
	*/

	//Write the C++ equivalent of the above python code
	pxr::TfTokenVector xformProperties = xform.GetPropertyNames();
	std::cout << "xformProperties: [";
	for (pxr::TfToken token : xformProperties)
	{
		std::cout << token.GetString() << ",";
	}
	std::cout << "]" << std::endl;

	pxr::TfTokenVector sphereProperties = sphere.GetPropertyNames();
	std::cout << "sphereProperties: [";
	for (pxr::TfToken token : sphereProperties)
	{
		std::cout << token.GetString() << ",";
	}
	std::cout << "]" << std::endl;


	pxr::UsdAttribute extentAttr = sphere.GetAttribute(pxr::TfToken("extent"));
	pxr::VtVec3fArray extentValue;
	extentAttr.Get(&extentValue);
	std::cout << "extentAttr.Get(): (";
	for (pxr::GfVec3f vec : extentValue)
	{
		std::cout << vec << ",";
	}
	std::cout << ")" << std::endl;

	pxr::UsdAttribute radiusAttr = sphere.GetAttribute(pxr::TfToken("radius"));
	
	std::cout << "Setting \"radius\" to 2.0 and multipliying ExtendedErrorParamTypes by 2."<< std::endl;
	radiusAttr.Set(2.0);//must be a double
	extentAttr.Set(extentValue * 2.0);

	std::cout << "Setting \"primvars:displayColor\" to (0,0,1)" << std::endl;
	pxr::UsdGeomSphere sphereSchema(sphere);
	pxr::UsdAttribute color = sphereSchema.GetDisplayColorAttr();
	color.Set(pxr::VtVec3fArray({ pxr::GfVec3f(0,0,1) }));

	std::string fileResult;
	stage->GetRootLayer()->ExportToString(&fileResult);
	std::cout << "New content of Hello World:\n" << fileResult << std::endl;

	stage->GetRootLayer()->Save(); // Save the stage at the same location as the application.

}

int main()
{

	TestFunction_StageCreation();

	TestFunction_PixarTutorial_HelloWorld();

	TestFunction_PixarTutorial_InspectAndAuthorProperties();

	std::cout << "End of main." << std::endl;

	return 0;
}
