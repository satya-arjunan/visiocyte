#include "visiocyte_application.h"

MainWindow* VisiocyteApplication::mainWindow=0; //reset to be 0. by Hanchuan Peng, 20110705
bool VisiocyteApplication::mainWindowIsActive;

#ifdef _ALLOW_WORKMODE_MENU_
NaMainWindow* VisiocyteApplication::naMainWindow = 0; //reset to be 0. by Hanchuan Peng, 20110705
bool VisiocyteApplication::naMainWindowIsActive;
#endif

VisiocyteApplication::VisiocyteApplication(int & argc, char ** argv) 
    : QApplication(argc, argv)
{
	if (!VisiocyteApplication::mainWindow) //force checking. by Hanchuan Peng, 20110705
		VisiocyteApplication::mainWindow = new MainWindow();

    VisiocyteApplication::mainWindowIsActive = false;
    
#ifdef _ALLOW_WORKMODE_MENU_
    VisiocyteApplication::naMainWindowIsActive = false;
#endif
}


VisiocyteApplication::~VisiocyteApplication() {
    // Assuming Qt will delete mainWindow, neuronAnnotatorMainWindow
}



