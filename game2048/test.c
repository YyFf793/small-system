#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void)
{
    
    srandom(time(NULL));

    printf("%ld\n", random()% 10);

    return 0;
}
