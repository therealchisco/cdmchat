#include <stdio.h>
#include <time.h>
 
// Print the current date and time in C
int main(int argc , char * argv[])
{
    // `time_t` is an arithmetic time type
    time_t now;
 
    // Obtain current time
    // `time()` returns the current time of the system as a `time_t` value
    time(&now);
 
    // Convert to local time format and print to stdout
    printf("Current time: %s", ctime(&now));
} 
