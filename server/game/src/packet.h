//01. Find:
enum EPacketShopSubHeaders

//01. Add new enum value after SHOP_SUBHEADER_GC_NOT_ENOUGH_MONEY_EX
#ifdef ENABLE_ECONOMY_FEATURES
	SHOP_SUBHEADER_GC_ATTACH_ITEM,
#endif

//02: Find in struct packet_shop_item:
	TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_MAX_NUM];
	
//02. Add after:
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

//03. Find:
typedef struct packet_shop_start
{
	DWORD   owner_vid;
	struct packet_shop_item	items[SHOP_HOST_ITEM_MAX_NUM];
} TPacketGCShopStart;

//03. Add after:
#ifdef ENABLE_ECONOMY_FEATURES
struct packet_shop_attached_item
{
	DWORD		dwVnum;
#ifdef FULL_YANG
	long long	sales_price;
#else
	DWORD       sales_price;
#endif
	float		sales_demand;
	bool		salesRoofHit;
};

typedef struct packet_shop_attach_item
{
	DWORD   dwSize;
	struct packet_shop_attached_item	items[SHOP_HOST_ITEM_MAX_NUM];
} TPacketGCShopAttachItem;
#endif