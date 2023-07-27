//01. Find:
#include "InstanceBase.h"

//01. Add after:
#ifdef ENABLE_ECONOMY_FEATURES
#include "PythonShop.h"
#endif

//02. Find in function itemGetISellItemPrice:
	return Py_BuildValue("i", pItemData->GetISellItemPrice());
	
//02. Change to:
#ifdef ENABLE_ECONOMY_FEATURES
	const int index = CPythonShop::instance().GetAttachedItemIndex(pItemData->GetTable()->dwVnum);

	if (index != -1)
	{
		return Py_BuildValue("i", CPythonShop::instance().GetAttachItemDataSalesPrice(index));
	}
	else
		return Py_BuildValue("i", pItemData->GetISellItemPrice());
#else
	return Py_BuildValue("i", pItemData->GetISellItemPrice());
#endif

