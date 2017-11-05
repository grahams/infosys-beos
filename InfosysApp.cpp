// File:		$Id: InfosysApp.cpp,v 1.1 1999/02/23 00:55:25 jon Exp $
// Description:	The Application class for BeInfosys
//				
// Revisions:	
//				$Log: InfosysApp.cpp,v $
//				Revision 1.1  1999/02/23 00:55:25  jon
//
//				Revision 1.1  1999/02/03 07:05:39  grahams
//				Initial revision
//

#include <Application.h>
#include <Window.h>

#include "InfosysApp.h"
#include "InfosysView.h"

const char *app_signature = "application/x-vnd.CSH-Infosys";


InfosysApp::InfosysApp()
	: BApplication( app_signature )
{
	win = new InfosysWindow(BRect(0,751, 256, 767) );
	win->Show();
}

