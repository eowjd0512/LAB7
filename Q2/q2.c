#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#define N 200

int main(int argc, char *argv[]){
pid_t pid[N] = {0,};
int status;
//pid_t pid;
int i=0;
printf("Start\n");
for(i=1; i<argc; i++){
//pid = fork();

if((pid[i]=fork()) ==0)
{
	printf("%s \n", argv[i]);
	execl("/bin/gzip","gzip","-d", argv[i],NULL);
	//printf("%s \n", argv[i]);
}

}
i=0;
while(waitpid(pid[i++],&status,0)>0);
exit(0);

printf("Stop");
return 0;

}
