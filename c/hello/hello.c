#include <stdio.h>
#include <cs50.h>

int main(void)
{
    //user inputs name
    string name = get_string("what's your name? \n");

    //prints "hello, name!"
    printf("hello, %s!\n", name);
}