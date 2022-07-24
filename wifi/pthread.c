#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

int i = 0;

void *pthread1 (void * arg)
{
	
	while (1) {
		i = i-2;
		
		printf("pthread : %d\n", i);
		sleep(1);
		if (i == -10) {
			break;
		}
	}
	
	pthread_exit(NULL);
}

int main(void)
{
	
	
	pthread_t tid;
	
	pthread_create(&tid, NULL, pthread1, NULL);
	
	while (1) {
		i++;
		printf("main : %d\n", i);
		sleep(1);
		if (i == 50) {
			break;
		}
	}
	
	pthread_join(tid, NULL);
	
	printf("break : %d\n", i);
	
	return 0;
}