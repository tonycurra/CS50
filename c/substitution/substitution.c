#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

bool alpha(string key);
bool valid_key(string key);

int main(int argc, string argv[])
{
    //if no key, or more than 1
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    //get the KEY
    string key = argv[1];
    int n = strlen(key);
    //transform key all uppercase
    for (int i = 0; i < n; i++)
    {
        key[i] = toupper(key[i]);
    }

    //check if valid key: 1 argument, 26 characters
    if (argc == 2 && n == 26 && valid_key(key))
    {
        //get plaintext to cipher
        string plaintext = get_string("plaintext: ");

        //print ciphertext
        printf("ciphertext: ");
        //convert letters 1 by 1
        int m = strlen(plaintext);
        for (int i = 0; i < m; i++)
        {
            char k;
            char x = plaintext[i];
            //if alphabetical
            if (isalpha(x))
            {
                //if character uppercase in plaintext
                if (isupper(x))
                {
                    k = (char)key[x - 65];
                    printf("%c", k);
                }
                //if character in plaintext lowercase
                else
                {
                    char y = toupper(x);
                    k = (char)key[y - 65];
                    printf("%c", tolower(k));
                }
            }
            else
            {
                printf("%c", (plaintext[i]));
            }
        }
        printf("\n");
        return 0;
    }
    //else if key different than 26 char
    else
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }
}

//check if alphabetical
bool alpha(string key)
{
    for (int i = 0; i < 26; i++)
    {
        if (!isalpha(key[i]))
        {
            return false;
        }

    }
    return true;
}

//check if letters not repeated, return true if 26 unique letters
bool valid_key(string key)
{
    if (!alpha(key))
    {
        return false;
    }
    int n = 26;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (i != j && key[i] == key[j])
            {
                return false;
            }
        }
    }
    return true;
}