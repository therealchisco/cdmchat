//Example code: A simple server side code, which echos back the received message.
//Handle multiple socket connections with select and fd_set on Linux
//Originally taken from https://www.geeksforgeeks.org/socket-programming-in-cc-handling-multiple-clients-on-server-without-multi-threading/?ref=rp
//but modified and adapted
#include <stdio.h>
#include <string.h> //strlen, strcpy and strcat
#include <stdlib.h>
#include <errno.h>
#include <unistd.h> //close
#include <arpa/inet.h> //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros, in order to use the select() function [check `man select` for way more information]
#include <time.h> // Library to print time of logging attempt
#include "connections_log.h"
	
#define TRUE 1
#define FALSE 0
// Port in which the server will interact with the clients
#define PORT 8888
// Max amount of clients handled by server
#define MAX_CLIENTS 30
	
int main(int argc , char *argv[])
{
	int opt = TRUE;
	/* similar to a file descriptor, an initialized socket will get an integer number assigned to describe 
	the socket being initialized.
	- server_master_socket: describes the socket used by the server to capture all incoming connection requests
	*/ 
	int server_master_socket , new_socket , client_socket[MAX_CLIENTS]; 

	int addrlen, activity, valread , sd, sd2;
	int max_sd;

	struct sockaddr_in address;
		
	char buffer[1025]; //data buffer of 1K
		
	/* set of socket descriptors
	*/
	fd_set readfds;

	// `time_t` is an arithmetic time type
	time_t timeLoggingAttempt; // variable to store time of logging attempt
	char * timeLoggingString; // character pointer to store character UTC time representation of logging attempt
		
	//Welcome message shown at client when establishing a connection with the server
	const char *message = "Bienvenidos al Payaserver! \r\n";
	
	/* initialise all client_socket[] to 0, as a file descriptor, sockets are identified by an integer, all sockets 
	initialized as 0 are disconnected sockets
	*/
	for (int i = 0; i < MAX_CLIENTS; i++)
	{
		client_socket[i] = 0;
	}
		
	/* create a master socket for the server to check on possible incomming connections
	AF_INET stands for IPv4, SOCK_STREAM stands for the kind of stream data transport as in TCP
	(UDP in the other hand is not a socket stream), and the last parameter is always a 0 for the protocol
	*/
	if( (server_master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0)
	{
		/* if initializing the master socket in the server fails, exit the program and print error to strerr */
		perror("Initializing the server's master socket failed");
		exit(EXIT_FAILURE);
	}
	
	//set master socket to allow multiple connections,
	//this is just a good habit, it will work without this
	if( setsockopt(server_master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,
		sizeof(opt)) < 0 )
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	
	/* define type of socket created, use this information later to bind the socket:
	- AF_INET: IPv4 family
	- INADDR_ANY: bind to Any Incomming IP Address
	- htons( PORT) : translate PORT number to format desired by the library
	*/
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( PORT );
		
	/* Bind is define as 'attaching a socket to a local address' 
	Bind the socket to own IP adress port 8888
	- the second parameter must be a pointer to a sockaddr struct, so 
	*/
	if (bind(server_master_socket, (struct sockaddr *)&address, sizeof(address))<0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	printf("Listening on port %d for any IP addresses.\n", PORT);
		
	//try to specify maximum of 3 pending connections for the master socket
	if (listen(server_master_socket, 3) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}
		
	//accept the incoming connection
	addrlen = sizeof(address);
	puts("Waiting for connections ...");
		
	while(TRUE)
	{
		//clear the socket set
		FD_ZERO(&readfds);
	
		//add master socket to set
		FD_SET(server_master_socket, &readfds);
		max_sd = server_master_socket;
			
		//add child sockets to set
		for (int i = 0 ; i < MAX_CLIENTS ; i++)
		{
			//socket descriptor
			sd = client_socket[i];
				
			//if valid socket descriptor then add to read list
			if(sd > 0)
				FD_SET( sd , &readfds);
				
			//highest file descriptor number, need it for the select function
			if(sd > max_sd)
				max_sd = sd;
		}
	
		//wait for an activity on one of the sockets , timeout is NULL ,
		//so wait indefinitely
		activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);
	
		if ((activity < 0) && (errno!=EINTR))
		{
			printf("select error");
		}
			
		//If something happened on the master socket,
		//then its an incoming connection
		if (FD_ISSET(server_master_socket, &readfds))
		{
			if ((new_socket = accept(server_master_socket,
					(struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
			{
				perror("Error accepting connection with new client");
				exit(EXIT_FAILURE);
			}
			
			//inform user of socket number - used in send and receive commands
			printf("New connection , socket fd is %d , IP is : %s , port : %d\n" , new_socket , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));

			// Obtain current time
			// `time()` returns the current time of the system as a `time_t` value
			time(&timeLoggingAttempt);

			// Convert to UTC time format with ctime() and store as string
			timeLoggingString = ctime(&timeLoggingAttempt);

			char * log_info;
			// allocate enough memory to write data - this was causing a segfault, when trying to use strcat
			log_info = malloc(150); 
			strcpy(log_info,"Connection attempt from IP: "); 
			// store IP in ipAddress
			char * ipAddress = inet_ntoa(address.sin_addr);
			//strcat(ipAddress, "\0");
			strcat(log_info, ipAddress);

			strncat(log_info, " at UTC time: ",15);
			/* IMPORTANT: by reducing the strlen by 1 of the string being concatenated
			we are therefore not copying an extra 'new line' character `\n` therefore
			avoiding to have one extra unnecesarry new-line
			*/
			strncat(log_info,timeLoggingString,strlen(timeLoggingString)-1);
			//strncat(log_info, "\n",1); // Add a newline on the end
		
			// write IP address of new connection in connections log
			updateLog(log_info);			
		     	free(log_info);	
			//send new connection greeting message
			if( send(new_socket, message, strlen(message), 0) != strlen(message) )
			{
				perror("send");
			}
				
			puts("Welcome message sent successfully");
				
			//add new socket to array of sockets
			for (int i = 0; i < MAX_CLIENTS; i++)
			{
				//if position is empty
				if( client_socket[i] == 0 )
				{
					client_socket[i] = new_socket;
					printf("Adding to list of sockets as %d\n" , i);
						
					break;
				}
			}
		}
			
		//else its some IO operation on some other socket
		for (int i = 0; i < MAX_CLIENTS; i++)
		{
			sd = client_socket[i];
				
			if (FD_ISSET( sd , &readfds))
			{
				//Check if it was for closing , and also read the
				//incoming message
				if ((valread = read( sd , buffer, 1024)) == 0)
				{
					//Somebody disconnected , get his details and print
					getpeername(sd , (struct sockaddr*)&address , \
						(socklen_t*)&addrlen);
					printf("Client disconnected at IP %s and port %d \n" ,
						inet_ntoa(address.sin_addr) , ntohs(address.sin_port));
						
					//Close the socket and mark as 0 in list for reuse
					close( sd );
					client_socket[i] = 0;
				}
					
				else
				{
					if(sd==client_socket[0]){
						sd2 = client_socket[1];
					}
					if(sd==client_socket[1]){
						sd2 = client_socket[0];
					}
					//set the string terminating NULL byte on the end
					//of the data read
					buffer[valread] = '\0';
					send(sd2 , buffer , strlen(buffer) , 0 );
				}
			}
		}
	}
		
	return 0;
}


