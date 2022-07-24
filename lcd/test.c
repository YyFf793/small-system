#include <stdio.h>

int main(void)
{
	int i;
	for (i = 0; i < 100; i++) {
		printf("i : %d\n", i);
	}

	int j = 0;

	while (j < 100) {
		printf("j : %d\n", j);
		j++;
	}

	int k = 0;

	do{
		printf("%d\n", k);
		k++;
	}while(k < 100);


	int z;

	scanf("%d", &z);

	if (z == 0) {
		printf("0\n");
	}else if (z == 10) {
		printf("10\n");
	}else {
		printf("100\n");
	}

	return 0;
}
