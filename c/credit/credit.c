#include <cs50.h>
#include <stdio.h>

int main(void)
{
    //prompt card number
    long number = get_long("Number: ");

    //check sum
    int sum = 0;
    //x = number, to take off digits
    long x = number;
    //sum every other 2 digits from the second to last
    do
    {
        //take out the last digit
        x /= 10;
        // multiply the second to last digit (now last) *2
        int num = (x % 10) * 2;
        //sum num digits 1 by one
        do
        {
            sum += (num % 10);
            num /= 10;
        }
        while (num >= 1);
        //take out the digit just multiplied
        x /= 10;
    }
    while (x >= 1);

    //sum += digits not multiplied
    //y to take off digits
    long y = number;
    do
    {
        sum += (y % 10);
        y /= 100;
    }
    while (y >= 1);

    //card lenght. number of digits, divide by 10 until answer is 0, counter
    int lenght = 0;
    for (long i = number; i >= 1; i /= 10)
    {
        lenght++;
    }

    //Check sum
    if (sum % 10 == 0)
    {
        //check AMEX
        if (lenght == 15 && ((number / 10000000000000) == 34 || (number / 10000000000000) == 37))
        {
            printf("AMEX\n");
        }
        //check MASTERCARD
        else if (lenght == 16 && ((number / 100000000000000) >= 50 && (number / 100000000000000) <= 55))
        {
            printf("MASTERCARD\n");
        }
        //check VISA
        else if ((lenght == 16 || lenght == 13) && ((number / 1000000000000000) == 4 || (number / 1000000000000) == 4))
        {
            printf("VISA\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }
}