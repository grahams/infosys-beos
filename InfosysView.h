// File:		$Id: InfosysView.h,v 1.1 1999/02/23 00:55:25 jon Exp $
// Description:	Header for InfosysView.cpp
// Revisions:	
//				$Log: InfosysView.h,v $
//				Revision 1.1  1999/02/23 00:55:25  jon
//
//				Revision 1.1  1999/02/03 07:05:48  grahams
//				Initial revision
//

#ifndef __INFOSYSVIEW_H__
#define __INFOSYSVIEW_H__

#include <View.h>
#include <String.h>
#include <Bitmap.h>
#include <Font.h>
#include <OS.h>
#include <stdio.h>
#include <NetPositive.h>
#include <Application.h>

#include "InfosysDefs.h"

class _EXPORT InfosysView;

class InfosysView : public BView
{
public: // Constructor / Deconstructor
								InfosysView( BRect rect );
								InfosysView( BMessage * );
								~InfosysView();
								
public:
		status_t				Archive( BMessage *data, bool deep ) const;
		InfosysView 			*Instantiate( BMessage *data );
		void					Draw( BRect rect );
		void					Pulse( void );
		void					MouseDown( BPoint point );
		
protected:
		BBitmap *				Create_Text_Bitmap( char *buf, const char *str );
		
protected:
		int32					Infosys_Grabber_Func( void );
		static int32			Grabber_Thread(void *);
		
		
private:
		BString					mlocation;

		BBitmap					*bmp;
		BString					string;
		BFont					font;
		
		int						animation;
		int						frame;
		int						last_frame;
		
		bool					invalid;
		
		char 					url[512];	
		int 					url_length;
		
		thread_id				infosys_grabber;
};

#endif
