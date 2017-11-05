// File:		$Id: InfosysView.cpp,v 1.2 1999/02/24 01:23:19 grahams Exp $
// Description:	The 'meat' of Infosys, this is the View, which handles all drawing
//				rendering of Bitmaps, movement, etc...
// Revisions:	
//				$Log: InfosysView.cpp,v $
//				Revision 1.2  1999/02/24 01:23:19  grahams
//
//				Revision 1.1  1999/02/03 07:05:39  grahams
//				Initial revision
//

#include <View.h>
#include <Bitmap.h>
#include <Font.h>
#include <String.h>
#include <Dragger.h>
#include <Alert.h>
#include <Roster.h>

#include <stdio.h>
#include <stdlib.h>

#include "InfosysView.h"
#include "infosys_helper.h"
#include "InfosysSocket.h"

extern const char *app_signature;


InfosysView::InfosysView( BRect rect )
	: BView( rect, "InfosysView", B_FOLLOW_LEFT | B_FOLLOW_TOP , B_WILL_DRAW | B_PULSE_NEEDED ){
	SetViewColor(B_TRANSPARENT_32_BIT); // Eliminates Flickering
	//SetViewColor( B_TRANSPARENT_COLOR );
	invalid = true;
	string = "Welcome to Infosys!";
	bmp = NULL;
	font = *be_bold_font;
	font.SetSize( 16 );
	frame = 0;
	mlocation = "www.csh.rit.edu/";

	BRect rect(Bounds());
	rect.OffsetTo( B_ORIGIN );
	rect.top = rect.bottom -7;
	rect.left = rect.right -7;

	infosys_initialize( 65536 );

	infosys_grabber = spawn_thread( Grabber_Thread, 
                                    "Infosys Grabber",
									B_NORMAL_PRIORITY, this );
	resume_thread( infosys_grabber );
}


InfosysView::InfosysView( BMessage *msg )
	: BView( msg )
{
	SetViewColor( B_TRANSPARENT_COLOR );
	invalid = true;
	string = "Hello!";
	bmp = NULL;
	font = *be_bold_font;
	font.SetSize( 16 );
	frame = 0;
	
	mlocation = "lounge.csh.rit.edu/";

	infosys_initialize( 65536 );

	infosys_grabber = spawn_thread( Grabber_Thread, "Infosys Grabber",
									B_NORMAL_PRIORITY, this );
	resume_thread( infosys_grabber );
}


status_t
InfosysView::Archive( BMessage *data, bool deep ) const
{
	uint32 winresizingmode = ResizingMode();
	((BView*)this)->BView::SetResizingMode( B_FOLLOW_LEFT | B_FOLLOW_TOP );
	BView::Archive(data, deep);
	((BView*)this)->SetResizingMode(winresizingmode);
	
	//Pack our name 
	data->AddString( "add_on", app_signature);

	
	return B_NO_ERROR;
}


InfosysView *
InfosysView::Instantiate( BMessage *data )
{
	(new BAlert("lkj","validating instantiation", "go"))->Go();
	if( !validate_instantiation( data, "InfosysView" ) )
	{
		(new BAlert("lkj","returning null from instantiate", "ok"))->Go();
		return NULL;
	}
	return new InfosysView( data );
}


InfosysView::~InfosysView()
{
	kill_thread( infosys_grabber );
	infosys_uninitialize();
}


void
InfosysView::Draw( BRect rect )
{
	BView *tmp_view;
	BBitmap *tmp_bmp;
	
	if( !invalid )
	{
		tmp_bmp = new BBitmap( Bounds(), B_RGBA32, true, false );
		tmp_view = new BView( tmp_bmp->Bounds(), "tmpview", B_FOLLOW_NONE, 0 );
		tmp_bmp->AddChild( tmp_view );
		
		if( tmp_bmp->Lock() )
		{
			tmp_view->SetHighColor( 0,0,0, 255 );
			tmp_view->FillRect( tmp_view->Bounds() );
			
			tmp_view->DrawBitmap( bmp, BPoint( tmp_view->Bounds().Width() - (frame*4), 0 ) );

			tmp_bmp->Unlock();
		}
			
		tmp_bmp->RemoveChild( tmp_view );
		delete tmp_view;
			
		if( Window()->Lock() )
		{
			DrawBitmap( tmp_bmp, BPoint(0,0) );
			Window()->Unlock();			
		}	
		delete tmp_bmp;
	}
	else
	{
		SetHighColor( 0,0,0,255 );
		FillRect( Bounds() );
	}
}


void
InfosysView::Pulse( void )
{
	char buf[512];
	char str[512];
		
	if( invalid )
	{

		if( infosys_buffer_valid()  )
		{
			if( infosys_get_text_message( str, 512 ) < 1 )
			{
				infosys_messagelist_start();
			}
			else
			{
				if( !strncmp( ".", str, 1 ) )
				{
					infosys_messagelist_start();
					return;
				}
				string = str;
				url_length = infosys_get_url( url, 512 );

				infosys_next_message();
			}
		}
		else
			string = "Retrieving Data. Please Wait...";
		
				
		if( bmp != NULL )
			delete bmp;
		
		bmp = Create_Text_Bitmap( buf, string.String() );
			
		frame = 0;
		invalid = false;
	}
	else
	{
		frame++;
		if( frame > last_frame )
			invalid = true;
		Invalidate();
	}
}


int32	
InfosysView::Infosys_Grabber_Func( void )
{
    char *tempstring;

    tempstring = (char *) malloc( sizeof( (char) * mlocation.String() ) );

	while(1)
	{
        strcpy( tempstring, mlocation.String() );

		if ( infosys_grab( tempstring ) ) {
    		infosys_messagelist_start();
        }
		snooze( 5 * 60 * 1000 * 1000);
	}

    free( tempstring );
}


int32 
InfosysView::Grabber_Thread( void *arg )
{
	InfosysView		*obj = (InfosysView *)arg;
	return ( obj->Infosys_Grabber_Func() );
}


BBitmap *
InfosysView::Create_Text_Bitmap( char *buf, const char *str )
{
	BBitmap 		*bmp;
	BView 			*tmp;
	float 			width;

	width = font.StringWidth( str );
	bmp = new BBitmap( BRect(0,0,width, Bounds().Height()), B_RGBA32, 
							true, false );
	tmp = new BView( bmp->Bounds(), "tmpview", B_FOLLOW_NONE, 0 );
	bmp->AddChild( tmp );
	
	if( bmp->Lock() )
	{	
		tmp->SetHighColor( 0,0,0,255 );
		tmp->FillRect( tmp->Bounds() );
		
		tmp->SetHighColor( 255, 0, 0, 255 );
		tmp->SetLowColor( 0,0,0, 255 );
		tmp->SetFont( &font );
		tmp->DrawString(str, BPoint(0, tmp->Bounds().Height()));
		bmp->Unlock();
	}
		
	bmp->RemoveChild( tmp );
	delete tmp;
	
	last_frame = (int)( Bounds().Width() + width )/4;

	
	return bmp;
}


void
InfosysView::MouseDown(BPoint point)
{
	BMessage *open_url = new BMessage( B_NETPOSITIVE_OPEN_URL );
 
	if ( url_length > 0 ) {
		open_url->AddString( "be:url", url );
		be_roster->Launch( "application/x-vnd.Be-NPOS", open_url, NULL );
	}
}

