// File:		$Id: InfosysWindow.cpp,v 1.2 1999/02/24 01:28:07 grahams Exp $
// Description:	Handles all window functions and instantiates the view,
// Revisions:	
//				$Log: InfosysWindow.cpp,v $
//				Revision 1.2  1999/02/24 01:28:07  grahams
//
//				Revision 1.1  1999/02/03 07:05:39  grahams
//				Initial revision
//

#include <Window.h>
#include <Application.h>
#include <View.h>
#include <Dragger.h>

#include "InfosysWindow.h"
#include "InfosysView.h"

InfosysWindow::InfosysWindow( BRect rect )
	: BWindow( rect, "Infosys", B_NO_BORDER_WINDOW_LOOK, B_FLOATING_ALL_WINDOW_FEEL, B_NOT_RESIZABLE | B_WILL_ACCEPT_FIRST_CLICK, B_ALL_WORKSPACES )
{
	view = new InfosysView( BRect( 0,0, 256, 16 ) );
	AddChild( view );
	SetPulseRate(50000);
}

InfosysWindow::~InfosysWindow()
{
	RemoveChild( view );
	delete view;
}

bool
InfosysWindow::QuitRequested()
{
	be_app->PostMessage( B_QUIT_REQUESTED );
	return true;
}
