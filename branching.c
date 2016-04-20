#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<stdio.h>

int _numChild = 0;

int branch(char *fileName){
   pid_t parent = getpid();
   _numChild++;
   fork();
   if(getpid() != parent){
      execlp(fileName, (char *)NULL);
      perror("Error in branch: exect failure");
      return(-1);
   }
   return 0;
}

int zombieKiller(){
   int exitState, count = 0, child;
   do{
      child = waitpid(0, &exitState, WNOHANG);
      if (child < 0){
	 perror("Error in zombieKiller: wait_pid failure");
	 return(-1);
      }
      _numChild--;
      count++;
   }while(child);
   return (count);
}

