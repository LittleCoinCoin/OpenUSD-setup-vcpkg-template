
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
@brief Function reproducing the first item of the Pixar USD tutorial.
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
@brief Function reproducing the third item of the Pixar USD tutorial.
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

/*!
@brief Function reproducing the fourth item of the Pixar USD tutorial
@see https://openusd.org/release/tut_referencing_layers.html
*/
void TestFunction_PixarTutorial_ReferencingLayers()
{
	std::cout << "** TestFunction_ReferencingLayers **" << std::endl;

	// -- Step 1
	std::cout << "---- Step 1 ----" << std::endl;

	/*! Python code from the tutorial
	    
		from pxr import Usd, UsdGeom
		stage = Usd.Stage.Open('HelloWorld.usda')
		hello = stage.GetPrimAtPath('/hello')
		stage.SetDefaultPrim(hello)
		UsdGeom.XformCommonAPI(hello).SetTranslate((4, 5, 6))
		print(stage.GetRootLayer().ExportToString())
		stage.GetRootLayer().Save()
	*/

	// Write the C++ equivalent of the python code above
	pxr::UsdStageRefPtr stage = pxr::UsdStage::Open("HelloWorld.usda");
	pxr::UsdPrim hello = stage->GetPrimAtPath(pxr::SdfPath("/hello"));
	stage->SetDefaultPrim(hello);
	pxr::UsdGeomXformCommonAPI(hello).SetTranslate(pxr::GfVec3d(4, 5, 6));

	std::string fileResult;
	stage->GetRootLayer()->ExportToString(&fileResult);
	std::cout << "Content of file HelloWorld.usda:\n" << fileResult << std::endl;

	// -- Step 2
	std::cout << "---- Step 2 ----" << std::endl;

	/*! Python code from the tutorial
		refStage = Usd.Stage.CreateNew('RefExample.usda')
		refSphere = refStage.OverridePrim('/refSphere')
		print(refStage.GetRootLayer().ExportToString())
	*/

	// Write the C++ equivalent of the python code above
	pxr::UsdStageRefPtr refStage = pxr::UsdStage::CreateNew("RefExample.usda");
	pxr::UsdPrim refSphere = refStage->OverridePrim(pxr::SdfPath("/refSphere"));

	refStage->GetRootLayer()->ExportToString(&fileResult);
	std::cout << "Content of file RefExample.usda:\n" << fileResult << std::endl;

	// -- Step 3
	std::cout << "---- Step 3 ----" << std::endl;

	/*! Python 
		refSphere.GetReferences().AddReference('./HelloWorld.usda')
		print(refStage.GetRootLayer().ExportToString())
		refStage.GetRootLayer().Save()
	*/

	// Write the C++ equivalent of the python code above
	refSphere.GetReferences().AddReference("./HelloWorld.usda");

	refStage->GetRootLayer()->ExportToString(&fileResult);
	std::cout << "Content of file RefExample.usda after referencing HelloWorld.usda:\n" << fileResult << std::endl;

	// -- Step 4
	std::cout << "---- Step 4 ----" << std::endl;

	/*! Python code from the tutorial
		refXform = UsdGeom.Xformable(refSphere)
		refXform.SetXformOpOrder([])
		print(refStage.GetRootLayer().ExportToString())
	*/

	// Write the C++ equivalent of the python code above
	pxr::UsdGeomXformable refXform(refSphere);
	refXform.SetXformOpOrder({});

	refStage->GetRootLayer()->ExportToString(&fileResult);
	std::cout << "Content of file RefExample.usda after setting XformOpOrder to empty:\n" << fileResult << std::endl;

	// -- Step 5
	std::cout << "---- Step 5 ----" << std::endl;

	/*! Python code from the tutorial
		refSphere2 = refStage.OverridePrim('/refSphere2')
		refSphere2.GetReferences().AddReference('./HelloWorld.usda')
		print(refStage.GetRootLayer().ExportToString())
		refStage.GetRootLayer().Save()
	*/

	// Write the C++ equivalent of the python code above
	pxr::UsdPrim refSphere2 = refStage->OverridePrim(pxr::SdfPath("/refSphere2"));
	refSphere2.GetReferences().AddReference("./HelloWorld.usda");

	refStage->GetRootLayer()->ExportToString(&fileResult);
	std::cout << "Content of file RefExample.usda after adding a second reference to HelloWorld.usda:\n" << fileResult << std::endl;

	// -- Step 6
	std::cout << "---- Step 6 ----" << std::endl;

	/*! Python code from the tutorial
		overSphere = UsdGeom.Sphere.Get(refStage, '/refSphere2/world')
		overSphere.GetDisplayColorAttr().Set( [(1, 0, 0)] )
		print(refStage.GetRootLayer().ExportToString())
		refStage.GetRootLayer().Save()
	*/

	// Write the C++ equivalent of the python code above
	pxr::UsdGeomSphere overSphere = pxr::UsdGeomSphere::Get(refStage, pxr::SdfPath("/refSphere2/world"));
	overSphere.GetDisplayColorAttr().Set(pxr::VtVec3fArray({ pxr::GfVec3f(1, 0, 0) }));

	refStage->GetRootLayer()->ExportToString(&fileResult);
	std::cout << "Content of file RefExample.usda after setting the display color of /refSphere2/world to red:\n" << fileResult << std::endl;

	stage->GetRootLayer()->Save();
	refStage->GetRootLayer()->Save();
}

/*!
@brief Function reproducing the sixth item of the Pixar USD tutorial
@see https://openusd.org/release/tut_traversing_stage.html
*/
void TestFunction_PixarTutorial_StageTraversal()
{
	// This step of the tutorial originally uses usdview and the python interpreter.
	// I will adapt the demonstrated commands to C++ code.

	std::cout << "** TestFunction_PixarTutorial_StageTraversal **" << std::endl;

	// -- Step 1
	std::cout << "---- Step 1 ----" << std::endl;

	/*! The Python commands in the interpreter
		[x for x in usdviewApi.stage.Traverse()]
		# Output: [Usd.Prim(</refSphere>), Usd.Prim(</refSphere/world>), Usd.Prim(</refSphere2>), Usd.Prim(</refSphere2/world>)]
	*/

	// Write the C++ equivalent of the python code above
	pxr::UsdStageRefPtr refStage = pxr::UsdStage::Open("RefExample.usda");
	pxr::UsdPrimRange allPrims = refStage->Traverse();
	std::cout << "All prims in the stage of RefExample.usda:" << std::endl;
	for (pxr::UsdPrim prim : allPrims)
	{
		std::cout << prim.GetPath() << std::endl;
	}

	// -- Step 2
	std::cout << "---- Step 2 ----" << std::endl;

	/*! The Python commands in the interpreter
		[x for x in usdviewApi.stage.Traverse() if UsdGeom.Sphere(x)]
		# Output: [Usd.Prim(</refSphere/world>), Usd.Prim(</refSphere2/world>)]
	*/

	// Write the C++ equivalent of the python code above
	std::vector<pxr::UsdGeomSphere> allSpheres;
	for (auto it = allPrims.begin(); it != allPrims.end(); ++it)
	{
		if ((*it).IsA<pxr::UsdGeomSphere>())
		{
			allSpheres.push_back(pxr::UsdGeomSphere(*it));
		}
	}

	std::cout << "All prims in the stage of RefExample.usda that are UsdGeomSpheres:\n" << std::endl;
	for (pxr::UsdGeomSphere sphere : allSpheres)
	{
		std::cout << sphere.GetPath() << std::endl;
	}
	

	// -- Step 3
	std::cout << "---- Step 3 ----" << std::endl;

	/*! The Python commands in the interpreter
		
		primIter = iter(Usd.PrimRange.PreAndPostVisit(usdviewApi.stage.GetPseudoRoot()))
		for x in primIter: print(x, primIter.IsPostVisit())

		# Output:
		Usd.Prim(</>) False
		Usd.Prim(</refSphere>) False
		Usd.Prim(</refSphere/world>) False
		Usd.Prim(</refSphere/world>) True
		Usd.Prim(</refSphere>) True
		Usd.Prim(</refSphere2>) False
		Usd.Prim(</refSphere2/world>) False
		Usd.Prim(</refSphere2/world>) True
		Usd.Prim(</refSphere2>) True
		Usd.Prim(</>) True
	*/

	// Write the C++ equivalent of the python code above
	pxr::UsdPrimRange pseudoRoot = pxr::UsdPrimRange::PreAndPostVisit(refStage->GetPseudoRoot());
	std::cout << "All prims in the stage of RefExample.usda with pre and post visit:\n" << std::endl;
	for (auto it = pseudoRoot.begin(); it != pseudoRoot.end(); ++it)
	{
		std::cout << (*it).GetPath() << " " << (it.IsPostVisit() ? "True" : "False") << std::endl;
	}

	// -- Step 4
	std::cout << "---- Step 4 ----" << std::endl;

	/*! The Python commands in the interpreter
		
		# First, manually inactivate the reference to refSphere2

		# Then Print the prims in the stage
		[x for x in usdviewApi.stage.Traverse()]
		# Output: [Usd.Prim(</refSphere>), Usd.Prim(</refSphere/world>)]

		[x for x in usdviewApi.stage.TraverseAll()]
		# Output: [Usd.Prim(</refSphere>), Usd.Prim(</refSphere/world>), Usd.Prim(</refSphere2>)]
	*/

	// Write the C++ equivalent of the python code above
	refStage->OverridePrim(pxr::SdfPath("/refSphere2")).SetActive(false);

	//print the stage to check that it's marked as inactive
	std::string fileResult;
	refStage->GetRootLayer()->ExportToString(&fileResult);
	std::cout << "Content of file RefExample.usda after deactivating refSphere2:\n" << fileResult << std::endl;

	//Accoding to documentation: "refStage->Traverse(): Traverse the active, loaded, defined, non-abstract prims on this stage depth-first."
	std::cout << "Prims (active, loaded, defined, non-abstract) in the stage of RefExample.usda after deactivating refSphere2:" << std::endl;
	for (pxr::UsdPrim prim : refStage->Traverse())
	{
		std::cout << prim.GetPath() << std::endl;
	}

	std::cout << std::endl; //just for layout of the ouput

	//According to documentation "refStage->TraverseAll(): Traverse all the prims on this stage depth-first."
	std::cout << "All Prims in the stage of RefExample.usda after deactivating refSphere2:" << std::endl;
	for (pxr::UsdPrim prim : refStage->TraverseAll())
	{
		std::cout << prim.GetPath() << std::endl;
	}
}

/*!
@brief Function reproducing the Pixar USD tutorial on authoring variants
@see https://openusd.org/release/tut_authoring_variants.html
*/
void TestFunction_PixarTutorial_AuthoringVariants()
{
	std::cout << "** TestFunction_PixarTutorial_AuthoringVariants **" << std::endl;

// -- Step 1
	std::cout << "---- Step 1 ----" << std::endl;

	/*! Python code from the tutorial
		from pxr import Usd, UsdGeom
		stage = Usd.Stage.Open('HelloWorld.usda')
		colorAttr = UsdGeom.Gprim.Get(stage, '/hello/world').GetDisplayColorAttr()
		colorAttr.Clear()
		print(stage.GetRootLayer().ExportToString())
	*/

	// Write the C++ equivalent of the python code above

	//note: Local "opinions" in prims are stronger than variant selections.
	//		Therefore, the color of /hello/world is going to be cleared before adding 
	//		variants to a variant set.
	pxr::UsdStageRefPtr stage = pxr::UsdStage::Open("HelloWorld.usda");
	pxr::UsdGeomGprim gprim = pxr::UsdGeomGprim::Get(stage, pxr::SdfPath("/hello/world"));
	pxr::UsdAttribute colorAttr = gprim.GetDisplayColorAttr();
	colorAttr.Clear();
	
	std::string fileResult;
	stage->GetRootLayer()->ExportToString(&fileResult);
	std::cout << "Content of file HelloWorld.usda after clearing the display color of /hello/world:\n" << fileResult << std::endl;

	// -- Step 2
	std::cout << "---- Step 2 ----" << std::endl;

	/*!Python code from the tutorial
		rootPrim = stage.GetPrimAtPath('/hello')
		vset = rootPrim.GetVariantSets().AddVariantSet('shadingVariant')
		print(stage.GetRootLayer().ExportToString())
	*/
	
	// Write the C++ equivalent of the python code above
	pxr::UsdPrim rootPrim = stage->GetPrimAtPath(pxr::SdfPath("/hello"));
	pxr::UsdVariantSet vset = rootPrim.GetVariantSets().AddVariantSet("shadingVariant");

	stage->GetRootLayer()->ExportToString(&fileResult);
	std::cout << "Content of file HelloWorld.usda after adding a variant set to /hello:\n" << fileResult << std::endl;

	// -- Step 3
	std::cout << "---- Step 3 ----" << std::endl;

	/*! Python code from the tutorial
		vset.AddVariant('red')
		vset.AddVariant('blue')
		vset.AddVariant('green')
		print(stage.GetRootLayer().ExportToString())
	*/

	// Write the C++ equivalent of the python code above
	vset.AddVariant("red");
	vset.AddVariant("blue");
	vset.AddVariant("green");
	
	stage->GetRootLayer()->ExportToString(&fileResult);
	std::cout << "Content of file HelloWorld.usda after adding variants to the variant set of /hello:\n" << fileResult << std::endl;

	// -- Step 4 & 5
	std::cout << "---- Step 4 & 5 ----" << std::endl;

	/*!
		vset.SetVariantSelection('red')
		with vset.GetVariantEditContext():
			colorAttr.Set([(1,0,0)])

		vset.SetVariantSelection('blue')
		with vset.GetVariantEditContext():
			colorAttr.Set([(0,0,1)])

		vset.SetVariantSelection('green')
		with vset.GetVariantEditContext():
			colorAttr.Set([(0,1,0)])

		print(stage.GetRootLayer().ExportToString())
	*/

	// Write the C++ equivalent of the python code above
	vset.SetVariantSelection("red");
	{
		pxr::UsdEditContext context(vset.GetVariantEditContext());
		colorAttr.Set(pxr::VtVec3fArray({ pxr::GfVec3f(1, 0, 0) }));
	}

	vset.SetVariantSelection("blue");
	{
		pxr::UsdEditContext context(vset.GetVariantEditContext());
		colorAttr.Set(pxr::VtVec3fArray({ pxr::GfVec3f(0, 0, 1) }));
	}

	vset.SetVariantSelection("green");//note: this is going to stay as the default selected variant in helloWorld.usda
	{
		pxr::UsdEditContext context(vset.GetVariantEditContext());
		colorAttr.Set(pxr::VtVec3fArray({ pxr::GfVec3f(0, 1, 0) }));
	}

	stage->GetRootLayer()->ExportToString(&fileResult);
	std::cout << "Content of file HelloWorld.usda after setting the color of /hello/world according to the variant selection:\n" << fileResult << std::endl;

	// -- Step 6
	std::cout << "---- Step 6 ----" << std::endl;

	/*! Python code from the tutorial
		# Show flattened view of the stage
		print(stage.ExportToString(addSourceFileComment=False))
	*/

	// Write the C++ equivalent of the python code above
	stage->ExportToString(&fileResult, false);
	//note: Only the default variant is going to be shown in the flattened view
	//		In particular, the green color is going to be used for the attribute displayColor of /hello/world
	std::cout << "Flattened view of the stage of HelloWorld.usda:\n" << fileResult << std::endl;

	// -- Step 7
	std::cout << "---- Step 7 ----" << std::endl;

	/*! Python code from the tutorial
		# save in a new file HelloWorldWithVariants.usda
		stage.GetRootLayer().Export('HelloWorldWithVariants.usda')
	*/

	// Write the C++ equivalent of the python code above
	stage->GetRootLayer()->Export("HelloWorldWithVariants.usda");
	std::cout << "The stage of HelloWorld.usda with variants has been saved in HelloWorldWithVariants.usda." << std::endl;

}

/*!
@brief A utility function to wrap around the creation of a new stage.
@details This function is used in the Pixar tutorial on transformations and animations.
@return A new stage.
@see https://openusd.org/release/tut_xforms.html
*/
pxr::UsdStageRefPtr MakeInitialStage(std::string& _path)
{
	/*! Python code from the tutorial
		stage = Usd.Stage.CreateNew(path)
		UsdGeom.SetStageUpAxis(stage, UsdGeom.Tokens.z)
		stage.SetStartTimeCode(1)
		stage.SetEndTimeCode(192)
		return stage
	*/

	// Write the C++ equivalent of the python code above
	pxr::UsdStageRefPtr stage = pxr::UsdStage::CreateNew(_path);
	pxr::UsdGeomSetStageUpAxis(stage, pxr::UsdGeomTokens->z);
	stage->SetStartTimeCode(1);
	stage->SetEndTimeCode(192);
	return stage;
}

/*!
@brief A utility function to add a reference to a geometry.
@details This function is used in the Pixar tutorial on transformations and animations.
@return The xform prim that was added to the stage.
@see https://openusd.org/release/tut_xforms.html#id3
*/
pxr::UsdGeomXform AddReferenceToGeometry(pxr::UsdStageRefPtr _stage, std::string _path)
{
	/*! Python code from the tutorial
		geom = UsdGeom.Xform.Define(stage, path)
		geom.GetPrim().GetReferences().AddReference('./top.geom.usd')
		return geom
	*/

	// Write the C++ equivalent of the python code above
	pxr::UsdGeomXform geom = pxr::UsdGeomXform::Define(_stage, pxr::SdfPath(_path));
	geom.GetPrim().GetReferences().AddReference("./extras/top.geom.usd");
	return geom;
}

/*!
@brief A utility function to add a spin transformation to a geometry.
@details This function is used in the Pixar tutorial on transformations and animations.
@see https://openusd.org/release/tut_xforms.html#id5
*/
void AddSpin(pxr::UsdGeomXform _geom)
{
	/*! Python code from the tutorial
		spin = top.AddRotateZOp(opSuffix='spin')
		spin.Set(time=1, value=0)
		spin.Set(time=192, value=1440)
	*/

	// Write the C++ equivalent of the python code above
	pxr::UsdGeomXformOp spin = _geom.AddRotateZOp(pxr::UsdGeomXformOp::PrecisionFloat, pxr::TfToken("spin"));
	spin.Set(0.f, 1.f);//value is first and time is second
	spin.Set(1440.f, 192.f);//value is first and time is second
}

/*!
@brief A utility function to add a tilt transformation to a geometry.
@details This function is used in the Pixar tutorial on transformations and animations.
@see https://openusd.org/release/tut_xforms.html#id7
*/
void AddTilt(pxr::UsdGeomXform _geom)
{
	/*! Python code from the tutorial
		tilt = top.AddRotateXOp(opSuffix='tilt')
		tilt.Set(value=12)
	*/

	// Write the C++ equivalent of the python code above
	pxr::UsdGeomXformOp tilt = _geom.AddRotateXOp(pxr::UsdGeomXformOp::PrecisionFloat, pxr::TfToken("tilt"));
	tilt.Set(12.f);
}

/*!
@brief A utility function to add an offset transformation to a geometry.
@details This function is used in the Pixar tutorial on transformations and animations.
@see https://openusd.org/release/tut_xforms.html#id9
*/
void AddOffset(pxr::UsdGeomXform _geom)
{
	/*! Python  code from the tutorial
		top.AddTranslateOp(opSuffix='offset').Set(value=(0, 0.1, 0))
	*/

	// Write the C++ equivalent of the python code above
	pxr::UsdGeomXformOp offset = _geom.AddTranslateOp(pxr::UsdGeomXformOp::PrecisionFloat, pxr::TfToken("offset"));
	offset.Set(pxr::GfVec3f(0, 0.1, 0));
}

/*!
@brief A utility function to add a precession transformation to a geometry.
@details This function is used in the Pixar tutorial on transformations and animations.
@see https://openusd.org/release/tut_xforms.html#id9
*/
void AddPrecession(pxr::UsdGeomXform _geom)
{
	/*! Python code from the tutorial
		precess = top.AddRotateZOp(opSuffix='precess')
		precess.Set(time=1, value=0)
		precess.Set(time=192, value=360)
	*/

	// Write the C++ equivalent of the python code above
	pxr::UsdGeomXformOp precession = _geom.AddRotateZOp(pxr::UsdGeomXformOp::PrecisionFloat, pxr::TfToken("precess"));
	precession.Set(0.f, 1.0f);
	precession.Set(360.f, 192.f);
}

void TestFunction_PixarTutorial_TransformationsAndAnimations()
{
	
	std::cout << std::endl << "** TestFunction_PixarTutorial_TransformationsAndAnimations **" << std::endl;

	// -- Step 1
	std::cout << "---- Step 1 ----" << std::endl;

	/*! Python code from the tutorial
		stage = MakeInitialStage('Step1.usda')
		stage.SetMetadata('comment', 'Step 1: Start and end time codes')
		stage.Save()
		print(stage.GetRootLayer().ExportToString())
	*/

	// Write the C++ equivalent of the python code above
	std::string path = "Step1.usda";
	pxr::UsdStageRefPtr stage = MakeInitialStage(path);
	stage->SetMetadata(pxr::TfToken("comment"), "Step 1: Start and end time codes");
	stage->GetRootLayer()->Save();

	std::string fileResult;
	stage->GetRootLayer()->ExportToString(&fileResult);
	std::cout << "Content of file Step1.usda:\n" << fileResult << std::endl;

	// -- Step 2
	std::cout << "---- Step 2 ----" << std::endl;

	/*! Python code from the tutorial
		stage = MakeInitialStage('Step2.usda')
		stage.SetMetadata('comment', 'Step 2: Geometry reference')
		top = AddReferenceToGeometry(stage, '/Top')
		stage.Save()
		print(stage.GetRootLayer().ExportToString())
	*/

	// Write the C++ equivalent of the python code above
	path = "Step2.usda";
	stage = MakeInitialStage(path);
	stage->SetMetadata(pxr::TfToken("comment"), "Step 2: Geometry reference");
	pxr::UsdGeomXform top = AddReferenceToGeometry(stage, "/Top");
	stage->GetRootLayer()->Save();

	stage->GetRootLayer()->ExportToString(&fileResult);
	std::cout << "Content of file Step2.usda:\n" << fileResult << std::endl;

	// -- Step 3
	std::cout << "---- Step 3 ----" << std::endl;

	/*! Python code from the tutorial
		stage = MakeInitialStage('Step3.usda')
		stage.SetMetadata('comment', 'Step 3: Adding spin animation')
		top = AddReferenceToGeometry(stage, '/Top')
		AddSpin(top)
		stage.Save()
		print(stage.GetRootLayer().ExportToString())
	*/

	// Write the C++ equivalent of the python code above
	path = "Step3.usda";
	stage = MakeInitialStage(path);
	stage->SetMetadata(pxr::TfToken("comment"), "Step 3: Adding spin animation");
	top = AddReferenceToGeometry(stage, "/Top");
	AddSpin(top);
	stage->GetRootLayer()->Save();

	stage->GetRootLayer()->ExportToString(&fileResult);
	std::cout << "Content of file Step3.usda:\n" << fileResult << std::endl;

	// -- Step 4
	std::cout << "---- Step 4 ----" << std::endl;

	/*! Python code from the tutorial
		stage = MakeInitialStage('Step4.usda')
		stage.SetMetadata('comment', 'Step 4: Adding tilt')
		top = AddReferenceToGeometry(stage, '/Top')
		AddTilt(top)
		AddSpin(top)
		stage.Save()
		print(stage.GetRootLayer().ExportToString())
	*/

	// Write the C++ equivalent of the python code above
	path = "Step4.usda";
	stage = MakeInitialStage(path);
	stage->SetMetadata(pxr::TfToken("comment"), "Step 4: Adding tilt");
	top = AddReferenceToGeometry(stage, "/Top");
	AddTilt(top);
	AddSpin(top);
	stage->GetRootLayer()->Save();

	stage->GetRootLayer()->ExportToString(&fileResult);
	std::cout << "Content of file Step4.usda:\n" << fileResult << std::endl;

	// -- Step 4A
	std::cout << "---- Step 4A ----" << std::endl;

	/*! Python code from the tutorial
		stage = MakeInitialStage('Step4A.usda')
		stage.SetMetadata('comment', 'Step 4A: Adding spin and tilt')
		top = AddReferenceToGeometry(stage, '/Top')
		AddSpin(top)
		AddTilt(top)
		stage.Save()
		print(stage.GetRootLayer().ExportToString())
	*/

	// Write the C++ equivalent of the python code above
	path = "Step4A.usda";
	stage = MakeInitialStage(path);
	stage->SetMetadata(pxr::TfToken("comment"), "Step 4A: Adding spin and tilt");
	top = AddReferenceToGeometry(stage, "/Top");
	AddSpin(top);
	AddTilt(top);
	stage->GetRootLayer()->Save();

	stage->GetRootLayer()->ExportToString(&fileResult);
	std::cout << "Content of file Step4A.usda (Added Spin BEFORE Tilt):\n" << fileResult << std::endl;

	// -- Step 5
	std::cout << "---- Step 5 ----" << std::endl;

	/*! Python code from the tutorial
		stage = MakeInitialStage('Step5.usda')
		stage.SetMetadata('comment', 'Step 5: Adding precession and offset')
		top = AddReferenceToGeometry(stage, '/Top')
		AddPrecession(top)
		AddOffset(top)
		AddTilt(top)
		AddSpin(top)
		stage.Save()
		print(stage.GetRootLayer().ExportToString())
	*/

	// Write the C++ equivalent of the python code above
	path = "Step5.usda";
	stage = MakeInitialStage(path);
	stage->SetMetadata(pxr::TfToken("comment"), "Step 5: Adding precession and offset");
	top = AddReferenceToGeometry(stage, "/Top");
	AddPrecession(top);
	AddOffset(top);
	AddTilt(top);
	AddSpin(top);
	stage->GetRootLayer()->Save();

	stage->GetRootLayer()->ExportToString(&fileResult);
	std::cout << "Content of file Step5.usda:\n" << fileResult << std::endl;

	// -- Step 6
	std::cout << "---- Step 6 ----" << std::endl;

	/*! Python code from the tutorial
		# Use animated layer from Step5
		anim_layer_path = './Step5.usda'

		stage = MakeInitialStage('Step6.usda')
		stage.SetMetadata('comment', 'Step 6: Layer offsets and animation')

		left = UsdGeom.Xform.Define(stage, '/Left')
		left_top = UsdGeom.Xform.Define(stage, '/Left/Top')
		left_top.GetPrim().GetReferences().AddReference(assetPath = anim_layer_path, primPath = '/Top')

		middle = UsdGeom.Xform.Define(stage, '/Middle')
		middle.AddTranslateOp().Set(value=(2, 0, 0))
		middle_top = UsdGeom.Xform.Define(stage, '/Middle/Top')
		middle_top.GetPrim().GetReferences().AddReference(assetPath = anim_layer_path, primPath = '/Top', layerOffset = Sdf.LayerOffset(offset=96))

		right = UsdGeom.Xform.Define(stage, '/Right')
		right.AddTranslateOp().Set(value=(4, 0, 0))
		right_top = UsdGeom.Xform.Define(stage, '/Right/Top')
		right_top.GetPrim().GetReferences().AddReference(assetPath = anim_layer_path, primPath = '/Top', layerOffset = Sdf.LayerOffset(scale=0.25))

		stage.Save()
		print(stage.GetRootLayer().ExportToString())
	*/

	// Write the C++ equivalent of the python code above
	std::string anim_layer_path = "./Step5.usda";

	path = "Step6.usda";
	stage = MakeInitialStage(path);
	stage->SetMetadata(pxr::TfToken("comment"), "Step 6: Layer offsets and animation");

	pxr::UsdGeomXform left = pxr::UsdGeomXform::Define(stage, pxr::SdfPath("/Left"));
	pxr::UsdGeomXform left_top = pxr::UsdGeomXform::Define(stage, pxr::SdfPath("/Left/Top"));
	left_top.GetPrim().GetReferences().AddReference(anim_layer_path, pxr::SdfPath("/Top"));

	pxr::UsdGeomXform middle = pxr::UsdGeomXform::Define(stage, pxr::SdfPath("/Middle"));
	middle.AddTranslateOp().Set(pxr::GfVec3d(2, 0, 0));
	pxr::UsdGeomXform middle_top = pxr::UsdGeomXform::Define(stage, pxr::SdfPath("/Middle/Top"));
	middle_top.GetPrim().GetReferences().AddReference(anim_layer_path, pxr::SdfPath("/Top"), pxr::SdfLayerOffset(96.));

	pxr::UsdGeomXform right = pxr::UsdGeomXform::Define(stage, pxr::SdfPath("/Right"));
	right.AddTranslateOp().Set(pxr::GfVec3d(4, 0, 0));
	pxr::UsdGeomXform right_top = pxr::UsdGeomXform::Define(stage, pxr::SdfPath("/Right/Top"));
	right_top.GetPrim().GetReferences().AddReference(anim_layer_path, pxr::SdfPath("/Top"), pxr::SdfLayerOffset(0.0, 0.25));

	stage->GetRootLayer()->Save();

	stage->GetRootLayer()->ExportToString(&fileResult);
	std::cout << "Content of file Step6.usda:\n" << fileResult << std::endl;
}

int main()
{

	TestFunction_StageCreation();

	TestFunction_PixarTutorial_HelloWorld();

	TestFunction_PixarTutorial_InspectAndAuthorProperties();

	TestFunction_PixarTutorial_ReferencingLayers();

	TestFunction_PixarTutorial_StageTraversal();

	TestFunction_PixarTutorial_AuthoringVariants();

	TestFunction_PixarTutorial_TransformationsAndAnimations();

	std::cout << "End of main." << std::endl;

	return 0;
}
