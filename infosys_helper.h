// File:		$Id: infosys_helper.h,v 1.1 1999/02/23 00:55:26 jon Exp $
// Description:	Header for infosys_helper.c
// Revisions:	
//				$Log: infosys_helper.h,v $
//				Revision 1.1  1999/02/23 00:55:26  jon
//
//				Revision 1.1  1999/02/03 07:05:48  grahams
//				Initial revision
//


#ifndef __INFOSYS_HELPER_H__
#define __INFOSYS_HELPER_H__

extern "C" int infosys_initialize( int );
extern "C" int infosys_uninitialize();
extern "C" int infosys_grab( char *location ); 
extern "C" int infosys_buffer_valid();
extern "C" int infosys_messagelist_start();
extern "C" int infosys_next_message();
extern "C" int infosys_get_text_message( char *msg, int length );
extern "C" int infosys_get_url( char *msg, int length );

#endif
