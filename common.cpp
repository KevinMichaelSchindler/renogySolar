
#include <errno.h>
#include <malloc.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/poll.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "common.hpp"

int mprintf( char** pp, const char* fmt, ... ) {
	char* n = (char*)malloc(0);
	va_list args;

	va_start( args, fmt );
	int nl = vsnprintf( n, 0, fmt, args );
	va_end( args );

	int ol = 0;
	if ( pp[0] != NULL ) ol = strlen( pp[0] );

	pp[0] = (char*)realloc( pp[0], ol + nl + 1024 );
	va_start( args, fmt );
	int rc = vsprintf( pp[0] + ol, fmt, args );
	va_end( args );
	return rc;
}

int tcpAccept( int server ) {
	struct sockaddr_in sai;
	socklen_t sai_len = sizeof(sai);
	memset( &sai, 0, sizeof(sai) );
	return accept( server, (struct sockaddr*)&sai, &sai_len );
}

int createTCPServerSocket( unsigned short port ) {
	int server_fd;

	server_fd = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
	if ( server_fd < 0 ) {
		fprintf( stderr, "Failed to create server socket [%s]\n", strerror(errno) );
	} else {
		int opt = 1;
		if ( setsockopt( server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int) ) < 0 ) {
			fprintf( stderr, "Failed to set server socket reuseaddr [%s]\n", strerror(errno) );
			close( server_fd );
			server_fd = -1;
		} else {
			struct sockaddr_in	sai;
			memset( &sai, 0, sizeof(sai) );
			sai.sin_family = AF_INET;
			sai.sin_port = htons( port );
			sai.sin_addr.s_addr = htonl( INADDR_ANY );
			if ( bind( server_fd, (const sockaddr*)&sai, sizeof(sai) ) < 0 ) {
				fprintf( stderr, "Failed to bind server socket [%s]\n", strerror(errno) );
				close( server_fd );
				server_fd = -1;
			} else if ( listen( server_fd, 1 ) < 0 ) {
				fprintf( stderr, "Server wont listen [%s]\n", strerror(errno) );
				close( server_fd );
				server_fd = -1;
			} else {
				/* cool */
			}
		}
	}
	return server_fd;
}

void waitForTCPHangup( int fd ) {
	struct pollfd pfd;
	pfd.fd = fd;
	pfd.events = POLLIN | POLLHUP;
	for (;;) {
		pfd.revents = 0;

		int ret = poll( &pfd, 1, -1 );
	
		if ( pfd.revents & POLLIN ) {
			char buffer[1024];
			ret = read( fd, buffer, sizeof(buffer) );
			if ( ret == 0 ) break;
		}
		if ( pfd.revents & POLLHUP ) {
			break;
		}	
	}
}

int connectTCP( const char* ip, unsigned short port ) {
	int fd = -1;

	fd = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
	if ( fd < 0 ) {
		fprintf( stderr, "Failed to create server socket [%s]\n", strerror(errno) );
	} else {
		struct sockaddr_in	sai;
		memset( &sai, 0, sizeof(sai) );
		sai.sin_family = AF_INET;
		sai.sin_port = htons( port );
		sai.sin_addr.s_addr = inet_addr(ip);
		if ( connect( fd, (const sockaddr*)&sai, sizeof(sai) ) < 0 ) {
			fprintf( stderr, "Failed to connect [%s]\n", strerror(errno) );
			close( fd );
			fd = -1;
		}
	}
	return fd;
}


