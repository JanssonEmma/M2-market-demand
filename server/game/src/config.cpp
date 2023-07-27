//01. Find:
bool g_BlockCharCreation = false;

//01. Add below:
#ifdef ENABLE_ECONOMY_FEATURES
float	g_maxShopItemPurchaseDemand = 1.5f;
float	g_maxShopItemSalesDemand = 3.0f;
float	g_purchasePriceFloorPct = 10.0f;
float	g_salesPriceRoofPct = 20.0f;
long	g_salesPriceRoofAbsolute = 1;
#endif

//02. Find in function __LoadGeneralConfigFile:
		TOKEN("block_char_creation")
		{
			int tmp = 0;

			str_to_number(tmp, value_string);

			if (0 == tmp)
				g_BlockCharCreation = false;
			else
				g_BlockCharCreation = true;

			continue;
		}
		
//02. Add after:
#ifdef ENABLE_ECONOMY_FEATURES
		TOKEN("MAX_DEMAND_PURCH_CHANGE")
		{
			str_to_number(g_maxShopItemPurchaseDemand, value_string);
		}

		TOKEN("MAX_DEMAND_SALES_CHANGE")
		{
			str_to_number(g_maxShopItemSalesDemand, value_string);
		}

		TOKEN("PURCHASE_FLOOR_PCT")
		{
			str_to_number(g_purchasePriceFloorPct, value_string);
		}

		TOKEN("SALES_ROOF_PCT")
		{
			str_to_number(g_salesPriceRoofPct, value_string);
		}

		TOKEN("SALES_ROOF_ABSOLUTE")
		{
			str_to_number(g_salesPriceRoofAbsolute, value_string);
		}
#endif