#include "utility.h"

int main(int argc, char * argv[]) {
	int opt;
	
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

	return 0;
}

