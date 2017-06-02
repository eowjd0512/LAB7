#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
int m[17][17] ;
int path[17] ;
int used[17] ;
int length = 0 ;
int min = -1 ;
FILE * f;
char filename[17][17];
pid_t pid[17];

void handler(int signum)
{
	int shortest = 99999;
	int i;
	int eachlen[17] = {0,};
	FILE * eachfile[17];
	char result[100];

        if (signum == SIGINT) {
		//Open all file
		for(i = 0 ; i<17 ; i++){
			eachfile[i] = fopen(filename[i],"r");
			kill(pid[i], SIGKILL);
		}


		for(i = 0 ; i < 17 ; i++)
			fscanf(eachfile[i],"%d",&eachlen[i]);

		printf("\n");
		//Print each min len
		for(i = 0 ; i< 17 ; i++)
			printf("%d \n",eachlen[i]);
		printf("\n");


		//Find shortest path
		for(i = 0; i<17 ; i++){
			if(shortest > eachlen[i])
				shortest = eachlen[i];
		}
		for(i = 0 ; i<17; i++)
			if(shortest == eachlen[i])
				break;
		//TODO: Initialize the file's cursor : DONE
		fseek( eachfile[i], 0L, SEEK_SET );
		fgets(result,sizeof(result),eachfile[i]);
		printf("%s",result);
		//Close all file pointer
		for(i = 0 ; i<17 ; i++){
                        fclose(eachfile[i]);
                }

                exit(0) ;
        }
}


void _travel(int idx) {
	int i ;
	pid_t pid;
	char filename[10];
	if (idx == 17) {
		if (min == -1 || min > length) {
			pid = getpid();
			sprintf(filename , "%d.sol",pid);
			f = fopen(filename,"w");
			min = length ;
			fprintf(f,"%d (", length) ;
                	for (i = 0 ; i < 16 ; i++)
                      		fprintf(f,"%d ", path[i]) ;
                	fprintf(f,"%d)\n", path[16]) ;
			fclose(f);
		}

	}
	else {
		for (i = 0 ; i < 17 ; i++) {
			if (used[i] == 0) {
				path[idx] = i ;
				used[i] = 1 ;
				length += m[path[idx-1]][i] ;
				_travel(idx+1) ;
				length -= m[path[idx-1]][i] ;
				used[i] = 0 ;
			}
		}
	}
}

void travel(int start) {
	path[0] = start ;
	used[start] = 1 ;
	_travel(1) ;
	used[start] = 0 ;
}

int main() {
	int i, j, t ;
	int status;
	FILE * fp = fopen("gr17.tsp", "r") ;
	for (i = 0 ; i < 17 ; i++) {
		for (j = 0 ; j < 17 ; j++) {
			fscanf(fp, "%d", &t) ;
			m[i][j] = t ;
		}
	}

	fclose(fp) ;


	for (i = 0  ; i < 17 ; i++){

		if( (pid[i] = fork()) == 0) {
			sprintf(filename[i],"%d.sol",pid[i]);
			f = fopen(filename[i] , "w");
			travel(i) ;
			exit(0);
		}
	}
	for(i = 0 ; i<17 ; i++)
		sprintf(filename[i],"%d.sol",pid[i]);

	signal(SIGINT, handler) ;
	i = 0;
	while ((waitpid(pid[i++], &status, 0)) > 0);
	handler(SIGINT);

}