// File:		$Id: InfosysApp.h,v 1.1 1999/02/23 00:55:25 jon Exp $
// Description:	The header for InfosysApp.C
// Revisions:	
//				$Log: InfosysApp.h,v $
//				Revision 1.1  1999/02/23 00:55:25  jon
//
//				Revision 1.1  1999/02/03 07:05:48  grahams
//				Initial revision
//

#ifndef __INFOSYSAPP_H__
#define __INFOSYSAPP_H__

#include <Application.h>

#include "InfosysWindow.h"

extern const char *app_signature;

class InfosysApp : public BApplication
{
public:	//Constructors / Deconstructors
						InfosysApp();						
public:	// Other shit 

private: // Data members

	InfosysWindow		*win;	

};

#endif