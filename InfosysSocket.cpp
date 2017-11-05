// File:		$Id: InfosysSocket.cpp,v 1.1 1999/02/23 00:55:25 jon Exp $
// Description:	Some socket handlers for use with Infosys
// Revisions:	
//				$Log: InfosysSocket.cpp,v $
//				Revision 1.1  1999/02/23 00:55:25  jon
//
//				Revision 1.1  1999/02/03 07:05:39  grahams
//				Initial revision
//

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <fcntl.h>
#include <memory.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>

int open_connection( char *server_name, int16 port )
{
	struct sockaddr_in		server;
	struct hostent			*hp;
	int						sock;

//  count is unused, but not removed until I consult Soco
//	int						count;

	sock = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
	if( sock < 0 )
		return(-1);

	if( (hp = gethostbyname( server_name )) == NULL )
		return(-2);

	server.sin_family		= AF_INET;
	server.sin_port			= htons( port );
	memcpy( &server.sin_addr, hp->h_addr_list[0], hp->h_length );
	
	if( connect( sock, (struct sockaddr *)&server, sizeof(server)) )
		return(-3);
	
	return sock;
}

int sock_readline( int fd, char *buf, int len )
{
	int x,y=0;

	while(1)
	{
		x = recv( fd, &buf[y], 1, 0);
		if( x > 0 )
		{
			if( buf[y] == '\n' )
			{
				y++;
				break;
			}
			y++;
			if( y >= len - 1 )
				break;
		}
		else
			break;
	}
	buf[y] = '\0';

	return(y);
}

int file_readline( FILE *fp, char *buf, int len )
{
	int y=0;

	while(1)
	{
		buf[y] = fgetc( fp );
		if( buf[y] == EOF || buf[y] == '\n' )
			break;
		y++;
		if( y >= len - 1 )
			break;
	}
	buf[y] = '\0';

	return(y);
}

int get_infosys_file( char *filename )
{
	int sock;
	int fn;
	int count;
	char buf[512];

	if( (sock = open_connection("jake.csh.rit.edu", 20000)) >= 0 )
	{
			fn = open( filename, O_RDWR | O_CREAT | O_TRUNC | O_TEXT );
			count = sock_readline( sock, buf, 512 );
			while( count > 0 )
			{
				if( fn > 0 )
					write( fn, buf, count );
				count = sock_readline( sock, buf, 512 );
			}
			if( fn > 0 )
				close( fn );
			close( sock );
	}
	else return -1;

	return fn;
}

