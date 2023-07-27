//01. Find in function CShop::Create:
	m_dwNPCVnum = dwNPCVnum;
	
//01. Add after:
#ifdef ENABLE_ECONOMY_FEATURES
	m_attachedItemsPos = 0;
#endif

//02. Find in function CShop::SetShopItems:
		item.itemid = 0;
		
//02. Add after:
#ifdef ENABLE_ECONOMY_FEATURES
		/* 
		* init with no demand at all
		* CInputDB.Boot -> CShopManager::instance().Initialize -> shop->Create -> this method
		*/
		item.purchase_demand = 0.0f;
		item.sales_price = item_table->dwShopBuyPrice;
		item.purchaseFloorHit = false;
		item.salesRoofHit = false;
#endif

//02. Add as last statement of the function (CShop::SetShopItems):
#ifdef ENABLE_ECONOMY_FEATURES
	m_itemVectorDefaults.assign(m_itemVector.begin(), m_itemVector.end());
#endif

//03. Find in function CShop::Buy:
    return (SHOP_SUBHEADER_GC_OK);
	
//03. Add before:
#ifdef ENABLE_ECONOMY_FEATURES
	SetCurrentPurchaseFloor(pos, true);
	RecalcPurchasePrice(pos);

	SetCurrentSalesRoof(pos, true, false);
	RecalcSalesPrice(pos, false);

	BroadcastUpdateItemNew(pos);
#endif

//04. Find in function CShop::AddGuest:
		pack2.items[i].vnum = item.vnum;
		
//04. Add after:
#ifdef ENABLE_ECONOMY_FEATURES
		pack2.items[i].purchase_demand = item.purchase_demand;
		pack2.items[i].sales_demand = item.sales_demand;
		pack2.items[i].sales_price = item.sales_price;
		pack2.items[i].purchaseFloorHit = item.purchaseFloorHit;
		pack2.items[i].salesRoofHit = item.salesRoofHit;
#endif

//05. Find in function CShop::AddGuest:
	ch->GetDesc()->BufferedPacket(&pack, sizeof(TPacketGCShop));
	ch->GetDesc()->Packet(&pack2, sizeof(TPacketGCShopStart));
	
//05. Add after:
#ifdef ENABLE_ECONOMY_FEATURES
	if (!m_attachedItems.empty())
	{
		TPacketGCShop pack_attach;
		pack_attach.header = HEADER_GC_SHOP;
		pack_attach.subheader = SHOP_SUBHEADER_GC_ATTACH_ITEM;

		TPacketGCShopAttachItem pack_attach2;

		memset(&pack_attach2, 0, sizeof(pack_attach2));
		pack_attach2.dwSize = m_attachedItems.size();

		for (DWORD i = 0; i < m_attachedItems.size(); ++i)
		{
			const ATTACHED_ITEM& attachedItem = m_attachedItems[i];

#ifdef ENABLE_SHOP_BLACKLIST
			//HIVALUE_ITEM_EVENT
			if (quest::CQuestManager::instance().GetEventFlag("hivalue_item_sell") == 0)
			{
				//Ãàº¹ÀÇ ±¸½½ && ¸¸³âÇÑÃ¶ ÀÌº¥Æ® 
				if (attachedItem.vnum == 70024 || attachedItem.vnum == 70035)
				{
					continue;
				}
			}
#endif
			//END_HIVALUE_ITEM_EVENT

			pack_attach2.items[i].dwVnum = attachedItem.dwVnum;
			pack_attach2.items[i].sales_demand = attachedItem.sales_demand;
			pack_attach2.items[i].sales_price = attachedItem.sales_price;
			pack_attach2.items[i].salesRoofHit = attachedItem.salesRoofHit;
		}

		pack_attach.size = sizeof(pack_attach) + sizeof(pack_attach2);

		ch->GetDesc()->BufferedPacket(&pack_attach, sizeof(TPacketGCShop));
		ch->GetDesc()->Packet(&pack_attach2, sizeof(TPacketGCShopAttachItem));
	}
#endif

//06. Add new functions (somewhere):
#ifdef ENABLE_ECONOMY_FEATURES
float CShop::GetRandomShopItemPurchaseDemand()
{
	srand(static_cast<unsigned>(time(0)));
	return static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / g_maxShopItemPurchaseDemand));
}

float CShop::GetRandomShopItemSalesDemand()
{
	srand(static_cast<unsigned>(time(0)));
	return static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / g_maxShopItemSalesDemand));
}

float CShop::Precision(float f, int places)
{
	float n = std::pow(10.0f, places);
	return std::round(f * n) / n;
}

void CShop::RecalcPurchasePrice(BYTE pos)
{
	m_itemVector[pos].price = CalcPurchasePrice(m_itemVector[pos],pos);
}

void CShop::RecalcSalesPrice(BYTE pos, bool isAttached)
{
	long newSalesPrice = 0;
	if (isAttached)
	{
		newSalesPrice = CalcAttachedSalesPrice(m_attachedItems[pos], pos);
		if (newSalesPrice < g_salesPriceRoofAbsolute)
			m_attachedItems[pos].sales_price = g_salesPriceRoofAbsolute;
		else
			m_attachedItems[pos].sales_price = newSalesPrice;
	}	
	else
	{
		newSalesPrice = CalcSalesPrice(m_itemVector[pos], pos);
		if (newSalesPrice < g_salesPriceRoofAbsolute)
			m_itemVector[pos].sales_price = g_salesPriceRoofAbsolute;
		else
			m_itemVector[pos].sales_price = newSalesPrice;
	}
}

void CShop::BroadcastUpdateItemNew(BYTE pos)
{
	GuestMapType::iterator it;

	it = m_map_guest.begin();

	while (it != m_map_guest.end())
	{
		bool b_isOtherEmpire = false;
		LPCHARACTER ch = it->first;
		LPCHARACTER shop_owner = ch->GetShopOwner();
		if (shop_owner)
		{
			TPacketGCShop pack;
			TPacketGCShopUpdateItem pack2;

			TEMP_BUFFER	buf;

			pack.header = HEADER_GC_SHOP;
			pack.subheader = SHOP_SUBHEADER_GC_UPDATE_ITEM;
			pack.size = sizeof(pack) + sizeof(pack2);

			pack2.pos = pos;

			if (m_pkPC && !m_itemVector[pos].pkItem)
				pack2.item.vnum = 0;
			else
			{
				pack2.item.vnum = m_itemVector[pos].vnum;
				if (m_itemVector[pos].pkItem)
				{
					thecore_memcpy(pack2.item.alSockets, m_itemVector[pos].pkItem->GetSockets(), sizeof(pack2.item.alSockets));
					thecore_memcpy(pack2.item.aAttr, m_itemVector[pos].pkItem->GetAttributes(), sizeof(pack2.item.aAttr));
				}
				else
				{
					memset(pack2.item.alSockets, 0, sizeof(pack2.item.alSockets));
					memset(pack2.item.aAttr, 0, sizeof(pack2.item.aAttr));
				}
			}

			pack2.item.price = m_itemVector[pos].price;
			pack2.item.count = m_itemVector[pos].count;
			pack2.item.purchase_demand = m_itemVector[pos].purchase_demand;
			pack2.item.sales_demand = m_itemVector[pos].sales_demand;
			pack2.item.sales_price = m_itemVector[pos].sales_price;
			pack2.item.purchaseFloorHit = m_itemVector[pos].purchaseFloorHit;
			pack2.item.salesRoofHit = m_itemVector[pos].salesRoofHit;

			if (ch->GetDesc())
			{
				b_isOtherEmpire = (ch->GetEmpire() != shop_owner->GetEmpire());
#ifdef ENABLE_NEWSTUFF
				if ((b_isOtherEmpire) && !g_bEmpireShopPriceTripleDisable)
#else
				if (b_isOtherEmpire)
#endif
				{
					pack2.item.price *= 3;
				}

				buf.write(&pack, sizeof(pack));
				buf.write(&pack2, sizeof(pack2));

				ch->GetDesc()->Packet(buf.read_peek(), buf.size());
			}
		}
		++it;
	}
}

void CShop::BroadcastAttachedItems()
{
	if (!m_attachedItems.empty())
	{
		GuestMapType::iterator it;
		it = m_map_guest.begin();

		while (it != m_map_guest.end())
		{
			LPCHARACTER ch = it->first;
			LPCHARACTER shop_owner = ch->GetShopOwner();
			TPacketGCShop pack_attach;
			pack_attach.header = HEADER_GC_SHOP;
			pack_attach.subheader = SHOP_SUBHEADER_GC_ATTACH_ITEM;

			TPacketGCShopAttachItem pack_attach2;
			TEMP_BUFFER	buf;

			memset(&pack_attach2, 0, sizeof(pack_attach2));
			pack_attach2.dwSize = m_attachedItems.size();

			for (DWORD i = 0; i < m_attachedItems.size(); ++i)
			{
				const ATTACHED_ITEM& attachedItem = m_attachedItems[i];

#ifdef ENABLE_SHOP_BLACKLIST
				//HIVALUE_ITEM_EVENT
				if (quest::CQuestManager::instance().GetEventFlag("hivalue_item_sell") == 0)
				{
					//Ãàº¹ÀÇ ±¸½½ && ¸¸³âÇÑÃ¶ ÀÌº¥Æ® 
					if (attachedItem.vnum == 70024 || attachedItem.vnum == 70035)
					{
						continue;
					}
				}
#endif
				//END_HIVALUE_ITEM_EVENT

				pack_attach2.items[i].dwVnum = attachedItem.dwVnum;
				pack_attach2.items[i].sales_demand = attachedItem.sales_demand;
				pack_attach2.items[i].sales_price = attachedItem.sales_price;
				pack_attach2.items[i].salesRoofHit = attachedItem.salesRoofHit;
			}

			pack_attach.size = sizeof(pack_attach) + sizeof(pack_attach2);

			buf.write(&pack_attach, sizeof(pack_attach));
			buf.write(&pack_attach2, sizeof(pack_attach2));

			ch->GetDesc()->Packet(buf.read_peek(), buf.size());

			++it;
		}
	}
}

void CShop::SetCurrentPurchaseFloor(BYTE pos, bool fromPurchase)
{
	float f_currentPurchaseDemand = 0.0f;
	f_currentPurchaseDemand = m_itemVector[pos].purchase_demand;
	if (fromPurchase)
		f_currentPurchaseDemand += GetRandomShopItemPurchaseDemand();
	else
		f_currentPurchaseDemand -= GetRandomShopItemPurchaseDemand();

	f_currentPurchaseDemand = Precision(f_currentPurchaseDemand);

	if (fabsf(f_currentPurchaseDemand) >= g_purchasePriceFloorPct)
	{
		if (fromPurchase)
			m_itemVector[pos].purchase_demand = g_purchasePriceFloorPct;
		else
			m_itemVector[pos].purchase_demand = g_purchasePriceFloorPct * -1;
		m_itemVector[pos].purchaseFloorHit = true;
	}
	else
	{
		m_itemVector[pos].purchaseFloorHit = false;
		if (fabsf(f_currentPurchaseDemand) == 0.0f)
			m_itemVector[pos].purchase_demand = 0.0f;
		else
			m_itemVector[pos].purchase_demand = f_currentPurchaseDemand;
	}
}

void CShop::SetCurrentSalesRoof(BYTE pos, bool fromPurchase, bool isAttached)
{
	float f_currentSalesDemand = 0.0f;
	if (isAttached)
		f_currentSalesDemand = m_attachedItems[pos].sales_demand;
	else
		f_currentSalesDemand = m_itemVector[pos].sales_demand;
	if (fromPurchase)
		f_currentSalesDemand -= GetRandomShopItemSalesDemand();
	else
		f_currentSalesDemand += GetRandomShopItemSalesDemand();

	f_currentSalesDemand = Precision(f_currentSalesDemand);

	if (fabsf(f_currentSalesDemand) >= g_salesPriceRoofPct)
	{
		if (isAttached)
		{
			if (fromPurchase)
				m_attachedItems[pos].sales_demand = g_salesPriceRoofPct * -1;
			else
				m_attachedItems[pos].sales_demand = g_salesPriceRoofPct;
			m_attachedItems[pos].salesRoofHit = true;
		}
		else
		{
			if (fromPurchase)
				m_itemVector[pos].sales_demand = g_salesPriceRoofPct * -1;
			else
				m_itemVector[pos].sales_demand = g_salesPriceRoofPct;
			m_itemVector[pos].salesRoofHit = true;
		}
	}
	else
	{
		if (isAttached)
		{
			m_attachedItems[pos].salesRoofHit = false;
			if (fabsf(f_currentSalesDemand) == 0.0f)
				m_attachedItems[pos].sales_demand = 0.0f;
			else
				m_attachedItems[pos].sales_demand = f_currentSalesDemand;
		}
		else
		{
			m_itemVector[pos].salesRoofHit = false;
			if (fabsf(f_currentSalesDemand) == 0.0f)
				m_itemVector[pos].sales_demand = 0.0f;
			else
				m_itemVector[pos].sales_demand = f_currentSalesDemand;
		}
	}
}

bool CShop::GetPurchaseFloorHit(BYTE pos)
{
	return m_itemVector[pos].purchaseFloorHit;
}

bool CShop::GetSalesRoofHit(BYTE pos, bool isAttached)
{
	if (isAttached)
		return m_attachedItems[pos].salesRoofHit;
	return m_itemVector[pos].salesRoofHit;
}

bool CShop::ItemIsShopItem(DWORD vnum)
{
	DWORD i = 0;
	do
	{
		if (m_itemVector[i].vnum == vnum)
			return true;
		i++;
	} while (i < m_itemVector.size() && i < SHOP_HOST_ITEM_MAX_NUM);

	return false;
}

bool CShop::ItemIsAttachedItem(DWORD vnum)
{
	if (!m_attachedItems.empty())
	{
		DWORD i = 0;
		do
		{
			if (m_attachedItems[i].dwVnum == vnum)
				return true;
			i++;
		} while (i < m_attachedItems.size());
	}
	return false;
}

void CShop::AttachItemToShop(LPITEM item)
{
	if (m_attachedItems.empty() || m_attachedItems.size() <= m_attachedItemsPos)
		m_attachedItems.resize(m_attachedItemsPos + 1);

	const TItemTable* item_table = item->GetProto();
		
	ATTACHED_ITEM& shop_item = m_attachedItems[m_attachedItemsPos];

	shop_item.dwVnum = item->GetVnum();
	shop_item.sales_price = item_table->dwShopBuyPrice;

	m_attachedItemsDefaults.assign(m_attachedItems.begin(), m_attachedItems.end());
	m_attachedItemsPos += 1;
}

#ifdef FULL_YANG
long long CShop::CalcPurchasePrice(const SHOP_ITEM& item, BYTE pos)
#else
long CShop::CalcPurchasePrice(const SHOP_ITEM& item, BYTE pos)
#endif
{
	/*
	* There is a high demand in that item, so the price is reduced (e.g. many players buy that specific item)
	* example:
	* - base price = 500
	* - demand = 25.0 (%)
	* - new price = 500 x ((100 - 25.0) / 100) => 500 x 0.75 => 375
	*
	* There is a low demand in that item, so the price is higer (e.g. many players sell that spoecific item)
	* example:
	* - base price = 500
	* - demand = -25.0 (%)
	* - new price = 500 x ((100 + 25.0) / 100) => 500 x 1.25 => 625
	*/
	if (GetPurchaseFloorHit(pos))
		return item.price;

	if (item.purchase_demand == 0.0f)
		return m_itemVectorDefaults[pos].price;

	if (item.purchase_demand > 0.0f)
	{
#ifdef FULL_YANG
		return static_cast<long long>(m_itemVectorDefaults[pos].price * ((100 - item.purchase_demand) / 100));
#else
		return static_cast<long>(m_itemVectorDefaults[pos].price * ((100 - item.purchase_demand) / 100));
#endif
	}
	else
	{
#ifdef FULL_YANG
		return static_cast<long long>(m_itemVectorDefaults[pos].price * ((100 + fabsf(item.purchase_demand)) / 100));
#else
		return static_cast<long>(m_itemVectorDefaults[pos].price * ((100 + fabsf(item.purchase_demand)) / 100));
#endif
	}
}

#ifdef FULL_YANG
long long CShop::CalcSalesPrice(const SHOP_ITEM& item, BYTE pos)
#else
long CShop::CalcSalesPrice(const SHOP_ITEM& item, BYTE pos)
#endif
{
	if (GetSalesRoofHit(pos, false))
		return item.sales_price;

#ifdef FULL_YANG
	long long baseSalesPrice = m_itemVectorDefaults[pos].sales_price;
#else
	long baseSalesPrice = m_itemVectorDefaults[pos].sales_price;
#endif

	if (item.sales_demand == 0.0f)
		return m_itemVectorDefaults[pos].sales_price;

	if (item.sales_demand > 0.0f)
	{
#ifdef FULL_YANG
		return static_cast<long long>(baseSalesPrice * ((100 - item.sales_demand) / 100));
#else
		return static_cast<long>(baseSalesPrice * ((100 - item.sales_demand) / 100));
#endif
	}
	else
	{
#ifdef FULL_YANG
		return static_cast<long long>(baseSalesPrice * ((100 + fabsf(item.sales_demand)) / 100));
#else
		return static_cast<long>(baseSalesPrice * ((100 + fabsf(item.sales_demand)) / 100));
#endif
	}
}

#ifdef FULL_YANG
long long CShop::CalcAttachedSalesPrice(const ATTACHED_ITEM& item, BYTE pos)
#else
long CShop::CalcAttachedSalesPrice(const ATTACHED_ITEM& item, BYTE pos)
#endif
{
	if (GetSalesRoofHit(pos, true))
		return item.sales_price;

#ifdef FULL_YANG
	long long baseSalesPrice = m_attachedItemsDefaults[pos].sales_price;
#else
	long baseSalesPrice = m_attachedItemsDefaults[pos].sales_price;
#endif

	if (item.sales_demand == 0.0f)
		return m_attachedItemsDefaults[pos].sales_price;

	if (item.sales_demand > 0.0f)
	{
#ifdef FULL_YANG
		return static_cast<long long>(baseSalesPrice * ((100 - item.sales_demand) / 100));
#else
		return static_cast<long>(baseSalesPrice * ((100 - item.sales_demand) / 100));
#endif
	}
	else
	{
#ifdef FULL_YANG
		return static_cast<long long>(baseSalesPrice * ((100 + fabsf(item.sales_demand)) / 100));
#else
		return static_cast<long>(baseSalesPrice * ((100 + fabsf(item.sales_demand)) / 100));
#endif
	}
}
#endif
