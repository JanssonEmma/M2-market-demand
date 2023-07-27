##Find in function def SetShopItem(self, slotIndex):
		price = shop.GetItemPrice(slotIndex)
		
##Add after:
		if app.ENABLE_ECONOMY_FEATURES:
			purchaseDemand = shop.GetItemPurchaseDemand(slotIndex)
			salesDemand = shop.GetItemSalesDemand(slotIndex)
			salesPrice = shop.GetItemSalesPrice(slotIndex)
			
##Find in same function:
		self.AppendPrice(price)
		
##Add after:
		if app.ENABLE_ECONOMY_FEATURES:
			self.AppendSalesPrice(salesPrice)
			self.AppendPurchaseDemand(purchaseDemand)
			self.AppendSalesDemand(salesDemand)
			
##Find:
	def AppendPrice(self, price):	
		self.AppendSpace(5)
		self.AppendTextLine(localeInfo.TOOLTIP_BUYPRICE  % (localeInfo.NumberToMoneyString(price)), self.GetPriceColor(price))
		
##Add after:
	if app.ENABLE_ECONOMY_FEATURES:
		def AppendSalesPrice(self, salesPrice):	
			self.AppendSpace(5)
			self.AppendTextLine(localeInfo.TOOLTIP_SALESPRICE  % (localeInfo.NumberToMoneyString(salesPrice)), self.GetPriceColor(salesPrice))
		def AppendPurchaseDemand(self, purchaseDemand,):
			self.AppendSpace(5)
			self.AppendTextLine(localeInfo.TOOLTIP_BUY_DEMAND  % purchaseDemand)
		def AppendSalesDemand(self, salesDemand):
			self.AppendSpace(5)
			self.AppendTextLine(localeInfo.TOOLTIP_SALES_DEMAND  % salesDemand)