#pragma once
//Defining which app to use 
#define ActiveApp_ResearchProject

//---------- Registered Applications -----------
#ifdef ActiveApp_ResearchProject
#include "projects/Movement/Pathfinding/NavMeshGraph/App_ResearchProject.h"
typedef App_ResearchProject CurrentApp;
#endif

class App_Selector {
public: 
	static IApp* CreateApp() {
		IApp* myApp = new CurrentApp();
		return myApp;
	}
};