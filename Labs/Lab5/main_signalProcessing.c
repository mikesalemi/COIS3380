// main_signalProcessing.c
// Michael Salemi
// 0751614
// 03/18/2022
// Description: 
//      This program changes the action of the "^C" and "^\" Unix signals
//      to carry out different functions. "^C" increments a counter each 
//      time it is pressed for a maximum of 5 times and then reverts back
//      to it's original action and exits the program if given again. 
//      "^\" is changed to creating a fork and passing the custom SIGUSR1
//      signal to its parent where it will then terminate the program. This
//      is done by setting the global variable "done" to 1 which is used as 
//      flag for the program while loop.

#include <signal.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int counter = 0;  // global counter variable
int done = 0;     // flag for program loop

void signalOptions(int signo);  // main signal handler
void exitprogram(int signo);    // signal handler for exceeding use of ^c

int main(int argc, char const *argv[]) {
    struct sigaction sig;      // holds direction to signal handlers
    const int MAX_SIGINT = 5;  // max uses of ^c before program termination
    // char stuff[100];

    while (!done) {
        printf("Waiting for signal...\n");
        sig.sa_handler = &signalOptions;  // establish signalOptions as primary signal handler

        sigaction(SIGINT, &sig, NULL);   // sighandler for ^c
        sigaction(SIGQUIT, &sig, NULL);  // sighandler for sigquit
        sigaction(SIGUSR1, &sig, NULL);  // signalhandler for sigusr1

        if (counter == MAX_SIGINT) {       // re-establish handler if used more than max times
            signal(SIGINT, &exitprogram);  // redirect signal handler
        }

        pause();      // wait for new signal
    } 

    return 0;
}
// void signalOptions(int signo)
// Parameters: 
//      int signo:  this parameter catches the signal that is given while in the main function
// Description:
//      This function provides an alternative action to the SIGINT, SIGQUIT, and SIGUSR1 actions
//      that are caught while the main function pauses and waits. After a set number of SIGINT
//      uses, the program terminates using the default SIGINT action. When SIGQUIT is given,
//      the program will also terminate. 
void signalOptions(int signo) {
    if (signo == SIGINT) {                               // user typed ^c
        printf(" Free SIGINT uses: %d/5\n", ++counter);  // num of times ^c was used
    } else if (signo == SIGQUIT) {                       // if user types sigquit

        if (!fork()) {  // child
            printf(" I am the child and I am sending SIGUSR1 to parent!\n");
            kill(getppid(), SIGUSR1);                           // sends SIGUSR1 to parent
            kill(getpid(), SIGKILL);                            // kills child process
            printf("ERROR: Child not terminated!\n");           // should never be printed
        } else {                                                // parent
            wait(NULL);                                         // wait for child process to be terminated
            printf("You pressed ctrl-c %d times.\n", counter);  // count of ^c used, only displayed with sigexit
            printf("Program terminating\n");                        // confirmation of child process termination
        }
    } else if (signo == SIGUSR1) {  // parent process receives
        done = 1;
        printf("Received sigusr1 from child\n");  // confirmation of receiving sigusr1
    } else {
        printf("ERROR: received invalid signal %d\n", signo);  // invalid signal
    }
}

// void exitProgram(int signo);
// Parameters:
//      int signo:  this paremater catches the signal that is given while in the main function
// Description:
//      When called, this function alerts the user that they have exceeded the maximum number
//      of SIGINT uses and then resets the action of SIGINT and then terminates the program by 
//      killing the program using SIGINT and the main program's process ID. 
void exitprogram(int signo) {
    signal(SIGINT, SIG_DFL);  // returns ^c to default handler
    printf(" Maximum SIGINT uses exceeded!(+5) Exiting program.\n");
    kill(getpid(), SIGINT);  // kills current process using ^c
}