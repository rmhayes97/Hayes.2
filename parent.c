#include "utility.h"

#define BUF_SIZE 1024
#define SHM_KEY 25

struct shmseg {
	int cnt;
	int complete;
	char buf[BUF_SIZE];
};
int fill_buffer(char * bufptr, int size);

int main(int argc, char * argv[]) {
	int opt;
	
	if(argc < 3) {
		fprintf(stderr, "not enough arguments\n");
		perror("parent: Error:");
		printf("Example: ./parent ss n\n");
		printf("Where ss is the number of second before termination of the process (default 100, max 200) and n is the number of processes (max 20).\n");
		exit(0);
	}
	else if(argc > 3) {
		fprintf(stderr, "too many arguments\n");
                perror("parent: Error:");
                printf("Example: ./parent ss n\n");
                printf("Where ss is the number of second before termination of the process (default 100, max 200) and n is the number of processes (max 20).\n");
                exit(0);
	}	


	while((opt = getopt(argc, argv, "t:")) != -1 ) {
		switch(opt){
			case 't':
				printf("option t\n");
				break;
			default:
				printf("Error: Invalid option\n");
				exit(-1);
		}
	}
	printf("out of loop\n");	 

//Shared memory functions found from: https://www.tutorialspoint.com/inter_process_communication/inter_process_communication_shared_memory.htm

	int shmid, numtimes;
	struct shmseg *shmp;
	char *bufptr;
	int spaceavailable;
	shmid = shmget(SHM_KEY, sizeof(struct shmseg), 0644|IPC_CREAT);
	if(shmid == -1) {
		perror("Shared memory");
		return 1;
	}

	//Attatch to the segment to get a pointer to it.
	shmp = shmat(shmid, NULL, 0);
	if(shmp == (void *) -1) {
		perror("Shared memory attatch");
		return 1;
	}

	//Transfer blocks of data from buffer to shared memory
	bufptr = shmp->buf;
	spaceavailable = BUF_SIZE;
	for(numtimes = 0; numtimes < 5; numtimes++) {
		shmp->cnt = fill_buffer(bufptr, spaceavailable);
		shmp->complete = 0;
		printf("Writing Process: Shared Memory Write: Wrote %d bytes\n", shmp->cnt);
		bufptr = shmp->buf;
		spaceavailable = BUF_SIZE;
		sleep(3);
	}
	printf("Writing Process: Wrote %d times\n", numtimes);
	shmp->complete = 1;
	
	if(shmdt(shmp) == -1) {
		perror("shmdt");
		return 1;
	}

	if(shmctl(shmid, IPC_RMID, 0) == -1) {
		perror("shmctl");
		return 1;
	}

	printf("Writing Process: Complete\n");
	return 0;
}

int fill_buffer(char * bufptr, int size) {
	static char ch = 'A';
	int filled_count;

	//printf("size is %d\n", size);
	memset(bufptr, ch, size - 1);
	bufptr[size-1] = '\0';
	if(ch > 122)
		ch = 65;
	if( (ch >= 65) && (ch <= 122) ) {
		if( (ch >= 91) && (ch <= 96) ) {
			ch = 65;
		}
	}
	filled_count = strlen(bufptr);
	
	//printf("buffer count is: %d\n", filled_count);
	//printf("buffer filled is:%s\n", bufptr);
	ch++;
	return filled_count;
} 

