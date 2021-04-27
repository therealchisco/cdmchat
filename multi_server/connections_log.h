/* Code originally taken from Hacking - The Art of Exploitation*/

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <fcntl.h> // this header defines the flags for the access mode of the open() function
#include <sys/stat.h> /* this header defines the flags for the File Permissions of a 
newly created file with the open() function, when using the O_CREAT flag*/

#define LOG_FILE "./log_attempted_connections" // path for connection log file

void fatal(char *); // A function for fatal errors

void *ec_malloc(unsigned int); // An error-checked malloc() wrapper

void updateLog(char *argv) { 
	int fd; // file descriptor for notes file
	char *buffer;
	buffer = (char *) ec_malloc(100); 

	strcpy(buffer, argv); // Copy into buffer.
	//printf("[DEBUG] buffer @ %p: \'%s\'\n", buffer, buffer); 
	
	strncat(buffer, "\n", 2); // Add a newline at the end.
	/* Opening file
	 Flags:
	 * O_WRONGLY flag for write-only access
	 * O_CREAT to create a file if it does not exist
	 * O_APPEND to append the contents to the end of the file
	 File Permission flags: If a file is created, receive this properties at creation
	 * S_IRUSR - Read access to User
	 * S_IWUSR - Write access to User
	 (Check Hacking The Art of Exploitation pg. 87 for more info on this topic)
	*/
	fd = open(LOG_FILE, O_WRONLY|O_CREAT|O_APPEND, S_IRUSR|S_IWUSR); 
	if(fd == -1) /* if opening file */
		fatal("in main() while opening file"); 
	printf("[DEBUG] file descriptor is %d\n", fd);
	/* Writing data 
	 use strlen() to calculate the length of a string
	 write() function needs to know which file descriptor to use, what string to store, and how long the string is
	 how many bytes to write
	*/
	if(write(fd,buffer, strlen(buffer)) == -1) 
		fatal("in main() while writing buffer to file.");
	// Closing file 
	if(close(fd) == -1) 
		fatal("in main() while closing file.");

	printf("Note has been saved.\n");
	free(buffer); 
}

// A function to display an error message and then exit 
void fatal(char *message) {
	char error_message[100];
	strcpy(error_message, "[!!] Fatal Error "); 
	strncat(error_message, message, 83); 
	// perror outputs the error message to stderr
	perror(error_message);
	// exit with an error
	exit(-1);
}
// An error-checked malloc() wrapper function 
void *ec_malloc(unsigned int size) {
	void *ptr;
	ptr = malloc(size); 
	if(ptr == NULL)
		fatal("in ec_malloc() on memory allocation"); 
	return ptr;
}
