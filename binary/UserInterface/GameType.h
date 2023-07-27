//01. Find in typedef struct packet_shop_item:
    TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_SLOT_MAX_NUM];

//01. Add after:
#ifdef ENABLE_ECONOMY_FEATURES
	float		purchase_demand;
	float		sales_demand;
#ifdef FULL_YANG
	long long	sales_price;
#else
	DWORD       sales_price;
#endif
	bool		purchaseFloorHit;
	bool		salesRoofHit;
#endif

//02. Add new struct after packet_shop_item:
#ifdef ENABLE_ECONOMY_FEATURES
typedef struct packet_shop_attached_item
{
	DWORD		dwVnum;
#ifdef FULL_YANG
	long long	sales_price;
#else
	long		sales_price;
#endif
	float		sales_demand;
	bool		salesRoofHit;
} TAttachedShopItems;
#endif