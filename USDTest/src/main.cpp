
#include "main.h"

#include "pxr/usd/usd/stage.h"

int main()
{
	pxr::UsdStageRefPtr stage = pxr::UsdStage::CreateInMemory();

	return 0;
}
