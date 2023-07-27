//01. Find in function playerGetISellItemPrice:
		iPrice = pItemData->GetISellItemPrice() * CPythonPlayer::Instance().GetItemCount(Cell);
		
//01. Replace with:
#ifdef ENABLE_ECONOMY_FEATURES
	{
		const int index = CPythonShop::instance().GetAttachedItemIndex(pItemData->GetTable()->dwVnum);

		if (index != -1)
		{
			iPrice = CPythonShop::instance().GetAttachItemDataSalesPrice(index) * CPythonPlayer::Instance().GetItemCount(Cell);
		}
		else
			iPrice = pItemData->GetISellItemPrice() * CPythonPlayer::Instance().GetItemCount(Cell);
	}
#else
		iPrice = pItemData->GetISellItemPrice() * CPythonPlayer::Instance().GetItemCount(Cell);
#endif

//02. Find all occurances (if existing in your source):
//02. Alternatively remove all occurances if you want to get rid of this nonsense
	iPrice /= 5;
	
//02. Replace with (if not removed previously):
#ifndef ENABLE_ECONOMY_FEATURES
	iPrice /= 5;
#endif