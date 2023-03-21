// main_signalProcessing.c

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

void exitprogram(int signo) {
    signal(SIGINT, SIG_DFL);  // returns ^c to default handler
    printf(" Maximum SIGINT uses exceeded! Exiting program.\n");
    done = 1;
    // kill(getpid(), SIGINT);  // kills current process using ^c
}