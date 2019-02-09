#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include<time.h>
#include <stdlib.h>

int boundedrand(int inputSeed, int max, int min);
pid_t wait(int *stat_loc);
int main() {

    int seed;
    int ranNum;
    int c_ranNum;
    int gc_ranNum;
    pid_t parentPID;
    pid_t pid;
    pid_t childPID[9];
    pid_t grandChildPID[3];
    int status;

    printf("enter the seed for the parent process\n"); //used to determine number of children, waiting times
    scanf("%d", &seed);

    parentPID = getpid();
    printf("My process id is %d\n",parentPID);

    ranNum = boundedrand(seed, 9, 5); //number of children
    for (int i = 0; i<ranNum; i++) {//creating the children
        printf("%d is about to create a child\n",parentPID);
        pid = fork();
        if(pid < 0) {
            printf("error: closing program");
            perror("stop");
        }

        else if (pid == 0) { /*child process*/
            childPID[i] = getpid();
            seed += i;
            printf("I am a new child, my process ID is %d, my seed id %d\n", getpid(), seed);
            c_ranNum = boundedrand(seed, 3, 1); //number of grandchildren
            printf("I am child %d, I will have %d children\n", getpid(), c_ranNum);

            for(int j = 0; j<c_ranNum; j++){//creating the grandchildren
              printf("I am child %d, I am about to create a child\n", getpid());
              pid = fork();
              if(pid < 0){
                printf("\nerror: closing program\n");
                perror("stop\n");
              }

              else if (pid == 0){ /*grandchild process */
                printf("I am grandchild %d, My grandparent is %d, My parent is %d\n", getpid(), parentPID, childPID[i]);
                gc_ranNum = boundedrand(seed, 14, 5); //gc_ranNum = how long the grandchild waits to terminate in seconds
                sleep(gc_ranNum);
                printf("I am grandchild %d, with parent %d, I am about to terminate\n", getpid(), childPID[i]);
                exit(0);
              }
              else {
                printf("I am child %d, I just created a child\n", getpid());
                grandChildPID[j] = pid;
              }
            }//finished creating the grandchildren

            printf("I am the child %d, I have %d children, I am waiting for my children to terminate\n", getpid(), c_ranNum);

            for(int k = 0; k<c_ranNum; k++){
              while((pid = waitpid(grandChildPID[k], &status, WNOHANG)) == 0){}
              printf("I am child %d. My child %d has been waited\n", getpid(), grandChildPID[k]);
            }

            printf("I am child %d, I am about to terminate\n", getpid());
            sleep(5);
            exit(0); /*terminate child*/

        }
        else{
            printf("Parent %d has created a child with process ID %d\n", parentPID, pid);
            childPID[i] = pid;
        }
    }//finished creating children

    for(int n = 0; n<ranNum; n++){
      printf("I am the parent. I am waiting for child %d to terminate\n", childPID[n]);
      while((pid = waitpid(childPID[n], &status, WNOHANG)) == 0){}
      printf("I am process %d. My child %d is dead\n", getpid(), childPID[n]);
    }
    printf("I am the parent, child %d has terminated\n", childPID[ranNum-1]);

    sleep(5);
    return 0;
}

//pass in seed value?
int boundedrand(int inputSeed, int max, int min)
{

    int remainderNum;
    int randomNum;
    srand(inputSeed); //use seed for initizalization
    int bound;

    bound = max - min;
    remainderNum = rand() % bound;
    randomNum = min + remainderNum;
    return(randomNum);

}
