//01. Find: 
typedef struct packet_shop_start
{
	struct packet_shop_item		items[SHOP_HOST_ITEM_MAX_NUM];
} TPacketGCShopStart;

//01. Add after:
#ifdef ENABLE_ECONOMY_FEATURES
typedef struct packet_shop_attach_item
{
	struct packet_shop_attached_item	items[SHOP_HOST_ITEM_MAX_NUM];
} TPacketGCShopAttachItem;
#endif

//02. Find enum EPacketShopSubHeaders
//02. Add new enum value after SHOP_SUBHEADER_GC_NOT_ENOUGH_MONEY_EX:
#ifdef ENABLE_ECONOMY_FEATURES
	SHOP_SUBHEADER_GC_ATTACH_ITEM,
#endif