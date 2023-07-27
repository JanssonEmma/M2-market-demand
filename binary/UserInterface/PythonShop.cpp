//01. Find function CPythonShop::GetItemData
//01. Add new functions after:
#ifdef ENABLE_ECONOMY_FEATURES
void CPythonShop::InitAttachedItemData(DWORD attachedItemDataSize)
{
	if (m_attachedShopItems.empty() || m_attachedShopItems.size() < attachedItemDataSize)
		m_attachedShopItems.resize(attachedItemDataSize);
}

void CPythonShop::SetAttachItemData(BYTE tabIdx, const TAttachedShopItems& c_rAttachedShopItemData)
{
	m_attachedShopItems[tabIdx] = c_rAttachedShopItemData;
}

int CPythonShop::GetAttachedItemIndex(DWORD dwVnum)
{
	if (!m_attachedShopItems.empty())
	{
		DWORD i = 0;
		do
		{
			if (m_attachedShopItems[i].dwVnum == dwVnum)
				return i;
			i++;
		} while (i < m_attachedShopItems.size());
	}
	return -1;
}

DWORD CPythonShop::GetAttachItemDataSalesPrice(int index)
{
	return m_attachedShopItems[index].sales_price;
}
#endif

//02. Find in function CPythonShop::Open:
	m_isMainPlayerPrivateShop = isMainPrivateShop;
	
//02. Add after:
#ifdef ENABLE_ECONOMY_FEATURES
	m_attachedShopItems.clear();
#endif

//03. Find in function CPythonShop::Close:
	m_isMainPlayerPrivateShop = FALSE;
	
//03. Add after:
#ifdef ENABLE_ECONOMY_FEATURES
	m_attachedShopItems.clear();
#endif

//04. Find function shopAddPrivateShopItemStock
//04. Add new functions after:
#ifdef ENABLE_ECONOMY_FEATURES
PyObject* shopGetItemPurchaseDemand(PyObject* poSelf, PyObject* poArgs)
{
	int iIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iIndex))
		return Py_BuildException();
	const TShopItemData* c_pItemData;
	if (CPythonShop::Instance().GetItemData(iIndex, &c_pItemData))
		return Py_BuildValue("f", c_pItemData->purchase_demand);
	return Py_BuildValue("f", 0.0f);
}

PyObject* shopGetItemSalesDemand(PyObject* poSelf, PyObject* poArgs)
{
	int iIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iIndex))
		return Py_BuildException();
	const TShopItemData* c_pItemData;
	if (CPythonShop::Instance().GetItemData(iIndex, &c_pItemData))
		return Py_BuildValue("f", c_pItemData->sales_demand);
	return Py_BuildValue("f", 0.0f);
}

PyObject* shopGetItemSalesPrice(PyObject* poSelf, PyObject* poArgs)
{
	int iIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iIndex))
		return Py_BuildException();
	const TShopItemData* c_pItemData;
	if (CPythonShop::Instance().GetItemData(iIndex, &c_pItemData))
		return Py_BuildValue("i", c_pItemData->sales_price);

	return Py_BuildValue("i", 0);
}
#endif

//05. Find:
		{ "GetItemPrice",				shopGetItemPrice,				METH_VARARGS },
		
//05. Add after:
#ifdef ENABLE_ECONOMY_FEATURES
		{ "GetItemPurchaseDemand",		shopGetItemPurchaseDemand,		METH_VARARGS },
		{ "GetItemSalesDemand",			shopGetItemSalesDemand,			METH_VARARGS },
		{ "GetItemSalesPrice",			shopGetItemSalesPrice,			METH_VARARGS },
#endif