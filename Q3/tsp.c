#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int m[17][17] ;
pid_t pid[17];
int pid_d[17];
int path[17] ;
int used[17] ;
int length = 0 ;
int min = -1 ;
int min_path[17];
int cost[17];
FILE *f;
FILE *fp;
int t;
int _min = 9999;
char filename[17][17];

void _travel(int idx) {
	int i ;

	if (idx == 17) {
		if (min == -1 || min > length) {
			min = length ;

			//printf("%d (", length) ;
			for (i = 0 ; i < 17 ; i++){
				//printf("%d ", path[i]) ;
				min_path[i] = path[i];
			}
			//printf("\n");
		//	printf("%d)\n", path[16]) ;
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

void handler(int signum){
	int i=0;
	if(signum == SIGINT){

		 fprintf(f,"%d (", min);
                        for(i=0; i<16;i++)
                                fprintf(f, "%d ", min_path[i]);
                        fprintf(f,"%d)\n", min_path[16]);

	//printf("%d(", min);
	//for(i=0;i<17;i++)
	//	printf("%d  ", min_path[i]);
	//printf(")\n");



	for(i=0; i<17;i++){
	// printf("%d ",pid[i]);

	kill(pid[i],SIGINT);
	}

	//printf("hi %d \n",min);

		for(i=0; i<17;i++){
			//sprintf(str,"%d.sol",pid[i]);
			fp = fopen(filename[i],"r");
			printf("%s\n",filename[i]);
			fscanf(fp,"%d",&t);
			cost[i] = t;
			printf("%d\n",t);
		}
	 //printf("bye %d \n",min);

		for(i=0; i<17;i++){
			if(cost[i]<min){
				min = cost[i];
			}
		}
	printf("min : %d \n",_min);

	exit(0);
	}
}

void travel(int start) {
	path[0] = start ;
	used[start] = 1 ;
	_travel(1) ;
	used[start] = 0 ;
}

int main() {
	int i, j;
	int status;

	pid_t retpid;
	 fp = fopen("gr17.tsp", "r");
	//f = fopen("<PID>.sol","a");
	char str[17];
	signal(SIGINT,handler);

	for (i = 0 ; i < 17 ; i++) {
		for (j = 0 ; j < 17 ; j++) {
			fscanf(fp, "%d", &t) ;
			m[i][j] = t ;
		}
	}
	fclose(fp) ;
	//printf("hi");
	for (i = 0  ; i < 17 ; i++){
		if((pid[i] = fork())==0){
		pid_d[i] = getpid();
		sprintf(str,"%d.sol",getpid());
                f = fopen(str,"w");
		strcpy(str,filename[i]);
		 travel(i);

                        fprintf(f,"%d (", min);
                        for(j=0; j<16;j++)
                                fprintf(f, "%d ", min_path[j]);
                        fprintf(f,"%d)\n", min_path[16]);

                        exit(0);

		}

                        //fprintf(f, "%d (",min);


	}
	 //signal(SIGINT,handler);

	i=0;

	//signal(SIGINT, handler);
	while((waitpid(pid[i++], &status, 0))>0);
	//signal(SIGINT,handler);

}

