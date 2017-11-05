// File:		$Id: Infosys.cpp,v 1.1 1999/02/23 00:55:24 jon Exp $
// Description:	The main routine for BeInfosys
// Revisions:	
//				$Log: Infosys.cpp,v $
//				Revision 1.1  1999/02/23 00:55:24  jon
//
//				Revision 1.1  1999/02/03 07:05:39  grahams
//				Initial revision
//

#include <Application.h>

#include "InfosysApp.h"


int main( int argc, char **argv )
{
	InfosysApp		app;
	
	app.Run();
	
	return(0);
}