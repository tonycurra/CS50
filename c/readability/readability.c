// input text, output grade level
#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

int main(void)
{
    // get text from user
    string text = get_string("Text: ");

    //count letters in text, send to function after main
    int l = count_letters(text);

    //count words in text
    int w = count_words(text);

    //count sentences
    int s = count_sentences(text);

    //average number of letters per 100 words in the text
    float L = (100 * (float)l / (float)w);

    //average number of sentences per 100 words in the text
    float S = (100 * (float)s / (float)w);

    int index = round(0.0588 * L - 0.296 * S - 15.8);
    //printf("index: %i\n", index);

    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index > 15)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", index);
    }
}

// count letters (uppercase and lowercase)
int count_letters(string text)
{
    int counter = 0;
    int l = strlen(text);
    for (int i = 0; i < l; i++)
    {
        int c = text[i];
        if (isalpha(c))
        {
            counter++;
        }
    }
    return counter;
}

//count words
int count_words(string text)
{
    // return number of words
    int words = 1;
    int l = strlen(text);
    for (int i = 0; i < l; i++)
    {
        int c = text[i];
        if (isspace(c))
        {
            words++;
        }
    }
    return words;
}

//count sentences in the text
int count_sentences(string text)
{
    //return number of sentences from text
    int sentences = 0;
    int l = strlen(text);
    for (int i = 0; i < l; i++)
    {
        char c = text[i];
        if (c == '.' || c == '!' || c == '?')
        {
            sentences++;
        }
    }
    return sentences;
}