#include <stdio.h>

//声明
int max(int, int);


int min(int x, int y)
{
    return 0;
}

int main(void)
{
    int value = 0xefadca;

    int num1 = (value >> 0) & 0xff;
    int num2 = (value >> 8) & 0xff;
    int num3 = (value >> 16) & 0xff;

    printf("num1 :%#x\n", num1);
    printf("num2 :%#x\n", num2);
    printf("num3 :%#x\n", num3);

	int x = 0xac, y = 0xcd, z = 0xae;
	
	// int value 0xaccdae;
	
	value = z;
	value |= (y << 8);
	value |= (x << 16);

    printf("value :%#x\n", value);

    int a = 10, b = 20, c;

    //调用
    c = max(a, b);

    printf("c : %d\n", c);

    return 0;
}

//实现
int max(int x, int y)
{
    if (x > y) {
        return x;
    }else if (x < y) {
        return y;
    }

    return 0;
}

