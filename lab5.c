/* =================================================================== *
*                          Signal Processing                           *
*                                                                      *
*  Name: lab5.c                                                        *
*                                                                      *
*  Written by: Brianna Drew - March 2020                               *
*                                                                      *
*  Purpose: (1) Handle the SIGINT signal by incrementing a counter     *
*           when the program finally completes and output the number   *
*           of times the user depressed CTRL-C. (2) Handle the         *
*           SIGQUIT signal by creating a new process (fork).           *
*                                                                      *
*  Subroutines/Libraries Required:                                     *
*      See include statements.                                         *
* ==================================================================== */

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

pid_t PID1,MainPID;		// process ID

void sig_int()
{
/**********Ctrl-C handler routine***********/
// Keep track of how many times Ctrl-C is pressed

static int ctrlcount = 0;
  ctrlcount ++;
    if (ctrlcount < 5)
       {
          switch (ctrlcount)
             {
                case 1:		// 1st time the user presses CTRL-C
                   printf("\nThis is the \033[0;31mfirst\033[0m time that you pressed CTRL-C\n");
                   break;
                case 2:		// 2nd time the user presses CTRL-C
                   printf("\nThis is the \033[0;32msecond\033[0m time that you pressed CTRL-C\n");
                   break;
                case 3:		// 3rd time the user presses CTRL-C
                   printf("\nThis is the \033[0;33mthird\033[0m time that you pressed CTRL-C\n");
                   break;
                case 4:		// 4th time the user presses CTRL-C
                   printf("\nThis is the \033[0;34mfourth\033[0m time that you pressed CTRL-C\n");
                   break;
             }
          printf("Waiting for another signal....\n\n");
       }
    else if (ctrlcount == 5)		// 5th time the user presses CTRL-C
       {
          //Terminate program gracefully when 5 CTRL-C key presses completed

          printf(" \nThis is the \033[0;35mfifth\033[0m time that you pressed CTRL-C\n");
          printf("SIGINT Maximum of 5 reached. Next SIGINT will terminate the program\n");
          printf("Waiting for a signal....\n\n");
       }
    else		// User presses CTRL-C more than 5 times
          printf("\n\033[1;32mProgram terminating....\033[0m\n\n");
          if (ctrlcount ==6 )
             exit(0);
//signal(SIGINT, SIG_DFL);

}

void sig_quit()
{
   // Handle SIGQUIT Signal
   printf(" received SIGQUIT\n");

     if ((PID1 = fork()) == -1)
         {     /* Start a child process.      */
           perror("fork error");
           exit(EXIT_FAILURE);
         }
     else if (PID1 == 0)
         {            /* This is the child.          */
           //Send user defined signal SIGUSR1 to parent and then close process gracefully
           printf("\nI am the Child PID:%d and I am sending a signal to my Parent PID:%d\n\n",getpid(),MainPID);
           int ret = kill(getpid(),SIGUSR1);
           exit(0);
         }
     else
         {  /* This is the parent.         */
            exit(EXIT_SUCCESS);
         }
}

void sig_usr1()
{
           //Indicate that the signal SIGUSR1 has been received from the Child and then ext program gracefully

           printf("I am the Parent. Signal received from the Child....\n");
           printf("\n\033[1;32mProgram terminating....\033[0m\n\n");
}


//**************************************************************************
//**************************************************************************

int main(void)
{
    int status;

    system("clear");
    MainPID=getpid();
    //Handle all signals and call appropriate subroutines
    if (signal(SIGUSR1, sig_usr1) == SIG_ERR)
        printf("\ncan't catch SIGUSR1\n");
    else if (signal(SIGINT, sig_int) == SIG_ERR)
        printf("\ncan't catch SIGINT\n");
    else if (signal(SIGQUIT, sig_quit) == SIG_ERR)
        printf("\ncan't catch SIGQUIT\n");
        // A long long wait so that we can easily issue a signal to this process
    else
       printf("\nWaiting for a signal....\n\n");
    while(1)
       sleep(1);
    return 0;
}
