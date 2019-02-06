/* myloggerd.c
 * Source file for thread-lab
 * Creates a server to log messages sent from various connections
 * in real time.
 *
 * Student: David Zitek
 */
 
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <pthread.h>
#include "message-lib.h"

// forward declarations
int usage( char name[] );
// a function to be executed by each thread
void * recv_log_msgs( void * arg );

// globals
int log_fd; // opened by main() but accessible by each thread
int client_fd; // created a global file descriptor variable
char message[100]; //buffer to hold message


void * recv_log_msgs( void * arg ){
	
	// loops to receive messages from a connection;
	// when read_msg returns zero, terminate the loop
	// and close the connection
	
	int clientfd = *((int*)arg);
	
	while((read_msg(clientfd, message, 100))> 0){
		write(log_fd, message, strlen(message));

}

	close_connection(clientfd);
	
	return NULL;
}

int usage( char name[] ){
	printf( "Usage:\n" );
	printf( "\t%s <log-file-name> <UDS path>\n", name );
	return 1;
}

int main( int argc, char * argv[] )
{
	if ( argc != 3 )
		return usage( argv[0] );
	
	//opening log file with read/write/append privileges to all users
	log_fd = open(argv[1], O_RDWR | O_APPEND | O_CREAT, 0666); 
	
	//opens a listener for UDS connections
	int listener = permit_connections(argv[2]);
	if(listener == -1)
		return -1;


	pthread_t thread1;
	
	//open
	while(1){
		
		printf("Waiting for connection on UDS path %s\n", argv[2]);

		client_fd = accept_next_connection(listener);
		pthread_create(&thread1, NULL, recv_log_msgs, &client_fd);
		
	}

	close_listener(listener);
	close_connection(log_fd);
			
	// open the log file for appending
	
	// permit message connections
			
	// loop to wait for connection requests;
	// as each connection is accepted,
	// launch a new thread that calls
	// recv_log_msgs(), which receives
	// messages and writes them to the log file			
	// when accept_next_connection returns -1, terminate the loop
	
	// close the listener
	
	// close the log file

	return 0;
}
