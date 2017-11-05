#ifndef __INFOSYSSOCKET_H__
#define __INFOSYSSOCKET_H__


extern int open_connection( char *server_name, int16 port );
extern int sock_readline( int fd, char *buf, int len );
extern int file_readline( FILE *fp, char *buf, int len );
extern int get_infosys_file( char *filename );

#endif