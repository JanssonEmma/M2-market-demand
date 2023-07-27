//01. Find in function CShopManager::Sell:
	dwPrice = item->GetShopBuyPrice();
	
//01. Replace with:
#ifdef ENABLE_ECONOMY_FEATURES
	if ((!IS_SET(item->GetFlag(), ITEM_FLAG_COUNT_PER_1GOLD) || item->GetShopBuyPrice() > g_salesPriceRoofAbsolute))
	{
		//Get current price
		BYTE pos = 0;
		CShop* currentShop = ch->GetShop();
		std::vector<CShop::SHOP_ITEM> shopItems;
		std::vector<CShop::ATTACHED_ITEM> attachedItems;

		if (currentShop->ItemIsShopItem(item->GetVnum()))
		{
			shopItems = currentShop->GetShopItems();
			if (!shopItems.empty())
			{
				do
				{
					if (shopItems[pos].vnum == item->GetVnum())
					{
						dwPrice = shopItems[pos].sales_price;
						break;
					}
					pos++;
				} while (pos < shopItems.size());
			}
			else
				dwPrice = item->GetShopBuyPrice();
		}
		else
		{
			if (currentShop->ItemIsAttachedItem(item->GetVnum()))
				attachedItems = currentShop->GetAttachedItems();
			if (!attachedItems.empty())
			{
				do
				{
					if (attachedItems[pos].dwVnum == item->GetVnum())
					{
						dwPrice = attachedItems[pos].sales_price;
						break;
					}
					pos++;
				} while (pos < attachedItems.size());
			}
			else
				dwPrice = item->GetShopBuyPrice();
		}
	}
	else
		dwPrice = item->GetShopBuyPrice();
#else
	dwPrice = item->GetShopBuyPrice();
#endif

//02. Find all occurances (if existing in your source):
//02. Alternatively remove all occurances if you want to get rid of this nonsense
	dwPrice /= 5;
	
//02. Replace with (if not removed previously):
#ifndef ENABLE_ECONOMY_FEATURES
	dwPrice /= 5;
#endif

//03. Find in same function (CShopManager::Sell):
	DBManager::instance().SendMoneyLog(MONEY_LOG_SHOP, item->GetVnum(), dwPrice);
	
//03. Add after:
#ifdef ENABLE_ECONOMY_FEATURES
	if ((!IS_SET(item->GetFlag(), ITEM_FLAG_COUNT_PER_1GOLD) || item->GetShopBuyPrice() > g_salesPriceRoofAbsolute))
	{
		//Update price
		BYTE i = 0;
		CShop* pkShop = ch->GetShop();
		std::vector<CShop::SHOP_ITEM> updateShopItems;
		std::vector<CShop::ATTACHED_ITEM> updateAttachedItems;

		if (pkShop->ItemIsShopItem(item->GetVnum()))
		{
			updateShopItems = pkShop->GetShopItems();
			if (!updateShopItems.empty())
			{
				do
				{
					if (updateShopItems[i].vnum == item->GetVnum())
					{
						pkShop->SetCurrentPurchaseFloor(i, false);
						pkShop->RecalcPurchasePrice(i);

						pkShop->SetCurrentSalesRoof(i, false, false);
						pkShop->RecalcSalesPrice(i, false);

						pkShop->BroadcastUpdateItemNew(i);
						break;
					}
					i++;
				} while (i < updateShopItems.size());
			}
		}
		else
		{
			if (!pkShop->ItemIsAttachedItem(item->GetVnum()))
				pkShop->AttachItemToShop(item);

			updateAttachedItems = pkShop->GetAttachedItems();
			if (!updateAttachedItems.empty())
			{
				do
				{
					if (updateAttachedItems[i].dwVnum == item->GetVnum())
					{
						pkShop->SetCurrentSalesRoof(i, false, true);
						pkShop->RecalcSalesPrice(i, true);

						break;
					}
					i++;
				} while (i < updateAttachedItems.size());
				pkShop->BroadcastAttachedItems();
			}
		}
	}
#endif