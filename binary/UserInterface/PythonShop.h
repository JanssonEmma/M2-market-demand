//01. Find:
		BOOL GetItemData(BYTE tabIdx, DWORD dwSlotPos, const TShopItemData ** c_ppItemData);
		
//01. Add after:
#ifdef ENABLE_ECONOMY_FEATURES
		void	InitAttachedItemData(DWORD attachedItemDataSize);
		void	SetAttachItemData(BYTE tabIdx, const TAttachedShopItems& c_rShopAttachedItemData);
		int		GetAttachedItemIndex(DWORD dwVnum);
		DWORD	GetAttachItemDataSalesPrice(int index);
#endif

//02. Find:
		ShopTab m_aShoptabs[SHOP_TAB_COUNT_MAX];
		
//02. Add after:
#ifdef ENABLE_ECONOMY_FEATURES
		struct ShopAttachedItem
		{
			TAttachedShopItems	items[SHOP_HOST_ITEM_MAX_NUM];
		};
		std::vector<TAttachedShopItems>	m_attachedShopItems;
#endif