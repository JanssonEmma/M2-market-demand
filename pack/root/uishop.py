##If existing, replace or remove this nonsense:
			if item.Is1GoldItem():
				itemPrice = itemCount / itemPrice / 5
			else:
				itemPrice = itemPrice * itemCount / 5
				
## ->
			if not app.ENABLE_ECONOMY_FEATURES:
				if item.Is1GoldItem():
					itemPrice = itemCount / itemPrice / 5
				else:
					itemPrice = itemPrice * itemCount / 5
