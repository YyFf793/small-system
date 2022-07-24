#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    system("pwd");

    system("madplay 1.mp3 &");

    sleep(2);

    system("kill -19 madplay");

    return 0;
}
