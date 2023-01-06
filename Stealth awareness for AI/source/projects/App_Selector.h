#pragma once
//Defining which app to use 
#define ActiveApp_NavMeshGraph

//---------- Registered Applications -----------
#ifdef ActiveApp_NavMeshGraph
#include "projects/Movement/Pathfinding/NavMeshGraph/App_NavMeshGraph.h"
typedef App_NavMeshGraph CurrentApp;
#endif

class App_Selector {
public: 
	static IApp* CreateApp() {
		IApp* myApp = new CurrentApp();
		return myApp;
	}
};