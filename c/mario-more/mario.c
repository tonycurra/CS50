//Builds a piramid, you choose the height

#include <cs50.h>
#include <stdio.h>

void bricks(int n);

int main(void)
{
    //get pyramid height from user
    int height;
    do
    {
        height = get_int("Height (1-8): ");
    } while (height < 1 || height > 8);

    // for each line --> i = line number, first line 1, last line is "height"
    for (int i=1; i <= height; i++)
    {
        //prints "height - i(line n)" spaces
        for (int j = height - i ; j>0; j--)
        {
            printf(" ");
        }
        //print i times #
        bricks(i);
        //print 2 spaces
        printf("  ");
        //print i times #
        bricks(i);
        //new line
        printf("\n");
    }
}

void bricks(int n) {
    for (int x = n; x > 0; x--)
    {
        printf("#");
    }
}