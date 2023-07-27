//Add to end of file:
#ifdef ENABLE_ECONOMY_FEATURES
	PyModule_AddIntConstant(poModule, "ENABLE_ECONOMY_FEATURES", true);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_ECONOMY_FEATURES", false);
#endif