// File:		$Id: infosys_helper.c,v 1.1 1999/02/23 00:55:26 jon Exp $
// Description:	Network helper class for Infosys clients
//				Shields clients from all networking and parsing
//				of Infosys message data.
// Revisions:	
//				$Log: infosys_helper.c,v $
//				Revision 1.1  1999/02/23 00:55:26  jon
//
//				Revision 1.1  1999/02/03 07:06:18  grahams
//				Initial revision
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stddef.h>
#include <fcntl.h>
#include <memory.h>
#include <errno.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>

// #define DEBUG



char 	*infosys_data_location		=		"/~sunday/infosys.cgi?l=ap";
char	*infosys_header_id			=		"INFOSYS";
char 	*infosys_get_command		=		"GET ";
char 	*infosys_buffer				=		NULL;
char	*infosys_buffer_cursor		=		NULL;

int		infosys_buffer_length		=		65536;
int		infosys_data_length			=		0;


/*
	This function takes a location string in the format of :
	
		site/location
		
	or
		site/location/

*/
void break_location( char *location, char **server, char **directory )
{
	register char *tmp = location;
	
	//Check for null string and empty string
	if( location != NULL && *location != '\0' )
	{
		//Set the server
		if( server != NULL )
			*server = location;
			
		//detach the server from the location
		while( (*tmp) )
		{
			if( (*tmp) == '/' )
			{
				*tmp = '\0';
				tmp++;
				break;
			}
			tmp++;
		}
		
		//set the directory
		if( directory != NULL )
			*directory = tmp;
		
		//Seek to the end of the string
		while( (*tmp) )tmp++;
	
		//remove a trailing slashes if they exist
		while( tmp != location )
		{
			tmp--;
			if( *tmp != '/' )
				break;
			else
				*tmp = '\0';
		}
	}
	else
	{
		*server = location;
		*directory = location;
	}
	
}

/*
	Connects to the infosys server and returns a socket.

	returns:
	< 0 if error.
	otherwise a socket handle;
*/

int infosys_connect( char *server_name, int port )
{
	struct sockaddr_in		server;
	struct hostent			*hp;
	int						sock;


	// create a socket
	sock = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
	if( sock < 0 )
		return(-1);

	// get the ip from the hostname
	if( (hp = gethostbyname( server_name )) == NULL )
		return(-2);

	server.sin_family		= AF_INET;
	server.sin_port			= htons( port );
	memcpy( &server.sin_addr, hp->h_addr_list[0], hp->h_length );

	// connect	
	if( connect( sock, (struct sockaddr *)&server, sizeof(server)) )
		return(-3);
	
	return sock;
}

/*




*/

int infosys_sock_writeline( int sock, char *buffer )
{
	int number; 
	
	if( buffer )
	{
		number = send( sock, buffer, strlen(buffer), 0 );
		if( number > 0)
		{
			if( send( sock, "\n", 1, 0 ) > 0 )
				return number+1;
			else
				return -1;
		}
		return number;
	}
	else
		return 0;
	
}



/*




*/

int infosys_sockread( int sock, char *buffer, int length )
{
	int y=0;
	int left = length - 1;
	int count =0;
	int total = 0;
	
	if( buffer != NULL && sock >= 0 )
	{
		while( left )
		{
			count = recv( sock, &buffer[y], left, 0 );
			if ( count < 1 )
				break;
			total = total + count;
			length = length - count;
		}
	}
	else return -1;

	return total;
}


/*

 Accepts a URL in the following format:
 server/location

 /infosys/ will be appended to the location
 ie:
 		lounge.csh.rit.edu 
 will become 
		lounge.csh.rit.edu/infosys/

*/


int infosys_grab( char *location )
{
	int						sock;
	int						full_length;
	int						ret_value;
	char 					*server;
	char					*directory;
	char					*command;
	
	if( infosys_buffer )
	{
		//Grab our location and server
		break_location( location, &server, &directory );
	
		//Open the connection
		sock =  infosys_connect( server, 80 );
	
		//allocate memory for our command string
		full_length = strlen( infosys_get_command );
		full_length = full_length + strlen( directory );
		full_length = full_length + strlen( infosys_data_location ) + 1;
		command = malloc( full_length );
	
		//prepare our string
		strcpy( command, infosys_get_command );
		strcat(	command, directory );
		strcat( command, infosys_data_location );
		
#ifdef DEBUG
		printf("command = %s\n", command );
#endif

		//send our string
		infosys_sock_writeline( sock, command );
		
		//free the memory
		free( command );
		
		//receive back our file...
		ret_value = infosys_sockread(	sock, 
										infosys_buffer, 
										infosys_buffer_length );
		infosys_buffer[ ret_value ] = '\0';
		
		//close the socket
		shutdown( sock, 2 );
#if defined( __FreeBSD__ ) || defined( linux ) || (__NetBSD__)
		close( sock );
#else
		closesocket( sock );
#endif

		// return
		if( ret_value )
			return 0;
		else return -1;
	}
	else return -1;
}


/*

*/

int infosys_buffer_valid()
{
	if( !strncmp( infosys_header_id, infosys_buffer, 7 ) )
		return 1;
	return 0;
}

/*


*/


void infosys_messagelist_start()
{
	register char *tmp;
	if( infosys_buffer )
	{
		tmp = infosys_buffer;
		if( infosys_buffer_valid() )
		{
			while( *tmp && *tmp != '\n' )
				tmp++;
			if( *tmp == '\n' )
				tmp++;
			infosys_buffer_cursor = tmp;
		}
		else infosys_buffer_cursor = infosys_buffer;
	}
}

int infosys_next_message()
{
	char *tmp;
	
	if( infosys_buffer )
	{
		tmp = infosys_buffer_cursor;
		while( *tmp )
		{
			if( *tmp == '\n' )
				break;
			tmp++;
		}
		if( *tmp == '\n' )
		{
			infosys_buffer_cursor = tmp+1;
			return 1;
		}
	}
	return 0;
}

/*



*/

int infosys_get_message( char *msg, int length )
{
	char *msg_tmp = msg;
	char *tmp = infosys_buffer_cursor;
	int msg_len = 0;
	
	if( tmp && msg_tmp )
	{
		while( *tmp && (msg_len < length) && (*tmp != '|') )
		{
				*msg_tmp = *tmp;
				msg_tmp += 1;
				tmp+= 1;
				msg_len += 1;
		}
		*msg_tmp = '\0';
	}
	return msg_len;

}

/*




*/


int infosys_get_text_message( char *msg, int length )
{
	char *msg_tmp = msg;
	char *tmp = infosys_buffer_cursor;
	int msg_len = 0;
	
	if( tmp && msg_tmp )
	{
		while( *tmp && msg_len < length && *tmp != '|' )
		{
			if( *tmp != '\\' )
			{
				*msg_tmp = *tmp;
				msg_tmp	+= 1;
				tmp		+= 1;
				msg_len += 1;
			}
			else
			{
				tmp++;
				switch( *tmp )
				{
					case 'c':	tmp	+=	3;
								break;
					case 'n':	tmp +=	1; 
								break;
					case 'a':	tmp +=  3;
								break;
					case 't':	tmp +=  3;
								break;
					case '\\':	tmp +=	1;
								*msg_tmp = '\\';
								msg_len += 1;
								break;
					default:
								break;
				}
			}
		}
		*msg_tmp = '\0';
	}
	return msg_len;
}


/*


*/

char *infosys_get_msg_url_ptr()
{
	register char *tmp = infosys_buffer_cursor;	

	if( tmp )
	{
		while( *tmp != '|' && *tmp )
			tmp += 1;
		if( *tmp == '|' )
			tmp += 1;
	}
	return tmp;
}
/*


*/

int infosys_get_url( char *url, int length )
{

	register char *tmp;
	register char *url_tmp = url;
	int len=0;
	
	if( (tmp = infosys_get_msg_url_ptr()) )
	{
		while( *tmp && (*tmp != '\n')  && len < length )
		{
			*url_tmp = *tmp;
			url_tmp += 1;
			tmp 	+= 1;
			len 	+= 1;
		}
		*url_tmp = '\0';
	}
	else return -1;
	
	return len;
}
/*






*/

int infosys_initialize( int buffer_size )
{
	infosys_buffer = malloc( buffer_size );
	if( infosys_buffer != NULL )
	{
		infosys_buffer_length = buffer_size;
		return 1;
	}
	return 0;
}


int infosys_uninitialize()
{
	if( infosys_buffer != NULL )
	{
		free( infosys_buffer );
		infosys_buffer = NULL;
		infosys_buffer_length = 0;
		return 1;
	}
	return 0;
}
