//01. Find:
class CShop
{
	public:
		typedef struct shop_item
		
//01. Add new fields after "int		itemid;"
#ifdef ENABLE_ECONOMY_FEATURES
			float	purchase_demand;
			float	sales_demand;
#ifdef FULL_YANG
			long long	sales_price;
#else
			long	sales_price;
#endif
			bool	purchaseFloorHit;
			bool	salesRoofHit;
#endif

//01. Init fields inside the constructor:
			shop_item()
			{
				vnum = 0;
				price = 0;
				count = 0;
				itemid = 0;
				pkItem = NULL;
#ifdef ENABLE_ECONOMY_FEATURES
				purchase_demand = 0.0f;
				sales_demand = 0.0f;
				sales_price = 0;
				purchaseFloorHit = false;
				salesRoofHit = false;
#endif
			}
			
//01. Add new struct after shop_item:
#ifdef ENABLE_ECONOMY_FEATURES
		typedef struct attached_item
		{
			DWORD		dwVnum;
#ifdef FULL_YANG
			long long	sales_price;
#else
			long		sales_price;
#endif
			float		sales_demand;
			bool		salesRoofHit;

			attached_item()
			{
				dwVnum = 0;
				sales_price = 0;
				sales_demand = 0.0f;
				salesRoofHit = false;
			}
		} ATTACHED_ITEM;
#endif

//02. Find:
		void	BroadcastUpdateItem(BYTE pos);
		
//02. Add function declarations before:
#ifdef ENABLE_ECONOMY_FEATURES
		float	GetRandomShopItemPurchaseDemand();
		float	GetRandomShopItemSalesDemand();
		float	Precision(float f, int places = 2);
		void	RecalcPurchasePrice(BYTE pos);
		void	RecalcSalesPrice(BYTE pos, bool isAttached);
		void	BroadcastUpdateItemNew(BYTE pos);
		void	BroadcastAttachedItems();
		void	SetCurrentPurchaseFloor(BYTE pos, bool fromPurchase);
		void	SetCurrentSalesRoof(BYTE pos, bool fromPurchase, bool isAttached);
		bool	GetPurchaseFloorHit(BYTE pos);
		bool	GetSalesRoofHit(BYTE pos, bool isAttached);
		bool	ItemIsShopItem(DWORD vnum);
		std::vector<SHOP_ITEM> GetShopItems() { return m_itemVector; }
		bool	ItemIsAttachedItem(DWORD vnum);
		std::vector<ATTACHED_ITEM> GetAttachedItems() { return m_attachedItems; }
		void	AttachItemToShop(LPITEM item);
#ifdef FULL_YANG
		long long	CalcPurchasePrice(const SHOP_ITEM& item, BYTE pos);
		long long	CalcSalesPrice(const SHOP_ITEM& item, BYTE pos);
		long long	CalcAttachedSalesPrice(const ATTACHED_ITEM& item, BYTE pos);
#else
		long	CalcPurchasePrice(const SHOP_ITEM& item, BYTE pos);
		long	CalcSalesPrice(const SHOP_ITEM& item, BYTE pos);
		long	CalcAttachedSalesPrice(const ATTACHED_ITEM& item, BYTE pos);
#endif
#endif

//03. Find: 
		std::vector<SHOP_ITEM>		m_itemVector;
		
//03. Add after:
#ifdef ENABLE_ECONOMY_FEATURES
		std::vector<SHOP_ITEM>		m_itemVectorDefaults;
		std::vector<ATTACHED_ITEM>	m_attachedItems;
		std::vector<ATTACHED_ITEM>	m_attachedItemsDefaults;
		int							m_attachedItemsPos;
#endif