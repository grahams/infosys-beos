// File:		$Id: InfosysWindow.h,v 1.1 1999/02/23 00:55:26 jon Exp $
// Description:	Header for InfosysWindow.cpp
// Revisions:	
//				$Log: InfosysWindow.h,v $
//				Revision 1.1  1999/02/23 00:55:26  jon
//
//				Revision 1.1  1999/02/03 07:05:48  grahams
//				Initial revision
//


#ifndef __INFOSYS_WINDOW_H__
#define __INFOSYS_WINDOW_H__

#include <Window.h>
#include <Application.h>
#include "InfosysView.h"


class InfosysWindow : public BWindow
{
public: //Constructors / Deconstructors
					InfosysWindow( BRect rect );
					~InfosysWindow();
			
public: // Other shit

	bool			QuitRequested();

private: // data members

	InfosysView		*view;
};


#endif