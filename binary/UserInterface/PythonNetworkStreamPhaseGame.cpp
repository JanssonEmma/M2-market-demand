//01. Find:
		case SHOP_SUBHEADER_GC_SOLD_OUT:
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OnShopError", Py_BuildValue("(s)", "SOLDOUT"));
			break;
			
//01. Add after:
#ifdef ENABLE_ECONOMY_FEATURES
		case SHOP_SUBHEADER_GC_ATTACH_ITEM:
			{
				DWORD dwSize = *(DWORD*)&vecBuffer[0];
				CPythonShop::instance().InitAttachedItemData(dwSize);

				TPacketGCShopAttachItem* pShopAttachItemPacket = (TPacketGCShopAttachItem*)&vecBuffer[4];
				for (BYTE iItemIndex = 0; iItemIndex < dwSize; ++iItemIndex)
				{
					CPythonShop::Instance().SetAttachItemData(iItemIndex, pShopAttachItemPacket->items[iItemIndex]);
				}
			}
			break;
#endif