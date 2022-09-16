// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cs50.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// numbers of words in dictionary
int words_number = 0;

// TODO: Choose number of buckets in hash table
const unsigned int N = 677;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    //cursor to move in hash table, points towards hash table

    node *cursor = table[hash(word)];

    //if word is in dictionary return true
    while (cursor != NULL)
    {
        //if word in dictionary, return true
        if (strcasecmp(word, cursor->word) == 0)
        {
            return true;
        }
        //else, check next word
        cursor = cursor->next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // numbers from [0 to (26 * 26) = 676]; from 0 to 675; N = 676
    // add 1 last number (word[276]) is for any other case that does not apply (in case there's any found)
    //aa, ab, ac, ..., az, ba, bb, bc, ..., bz, ca, cb, ..., zy, zz
    // 0,  1,  2, ..., 25, 26, 27, 28, ..., 51, 52, 53, ..., 674, 675

    // a is "0" in ascii decimal,
    //considering lowercase letter - 97 ( a = 0, z = 25)
    //formula used as per above: (26 * (first letter)) + (second letter);
    // e.g. aa = (26 * 0) + 0 = 0; bc = (26 * 1) + 2 = 28; zy = (26 * 25) + 24 = 674

    if (strlen(word) > 1)
    {
        //if first or second letter is not in range 'a' to 'z'
        if (word[0] < 0 || word[0] > 25 || word[1] < 0 || word[1] > 25)
        {
            return 276;
        }
        else
        {
            return ((word[0] - 97) * 26 + (word[1] - 97));
        }
    }
    //any other case found
    else
    {
        return 276;
    }
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    //open dictionary
    FILE *file = fopen(dictionary, "r");
    //check if dictionary was opened correctly
    if (file == NULL)
    {
        printf("Could not open %s", dictionary);
        return false;
    }

    //read words from dictionary one at a time and put them in hash table
    char *word = malloc(LENGTH + 1);
    if (word == NULL)
    {
        return false;
    }

    while (fscanf(file, "%s", word) != EOF)
    {
        //create new node to store the word
        node *new = malloc(sizeof(node));
        if (new == NULL) // to check if there's memory. If Not terminate.
        {
            return false;
        }

        //copy word in the node
        strcpy(new->word, word);

        //hash word
        int t = hash(word);

        //check if already words in table[t]
        if (table[t] == NULL)
        {
            new->next = NULL;
        }
        else
        {
            new->next = table[t];
        }

        //update table pointer to new word
        table[t] = new;

        // update number of words loaded
        words_number++;
    }
    free(word);
    fclose(file);

    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return words_number;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        while (table[i] != NULL)
        {
            node *cursor = table[i]->next;
            free(table[i]);
            table[i] = cursor;
        }
    }
    return true;
}
