#include <stdio.h>
#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <string.h>

int countLetters(string text);
int countWords(string text);
int countSentences(string text);
float colIndex(int letters, int words, int sentences);

int main(void)
{
    string text = get_string("Text: "); //Take an input text
    int letersCount = countLetters(text); // Count the letters
    int wordCount = countWords(text); // Count the words
    int sentCount = countSentences(text); // Count the sentences
    float cIndex = round(colIndex(letersCount, wordCount, sentCount)); //Calculate Coleman-Liau index.
 
    if (cIndex < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (cIndex > 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", (int)cIndex); 
    }
    

}

//Function that calculates the Coleman-Liau Index
float colIndex(int letters, int words, int sentences)
{
    float avgLetters = ((float)letters * 100) / (float)words;
    float avgSent = ((float)sentences * 100) / (float)words;
    float cIntex = 0.0588 * avgLetters - 0.296 * avgSent - 15.8;
    
    return cIntex;
}

// Function that counts letters of a string.
int countLetters(string text)
{
    int numberOfLetters = 0;
    for (int i = 0; i < strlen(text); i++)
    {
        if ((text[i] >= 'a' && text[i] <= 'z') || (text[i] >= 'A' && text[i] <= 'Z'))
        {
            numberOfLetters++;
        }

    }
    return numberOfLetters;

}

//Function that counts the number of words in a text
int countWords(string text)
{
    int numberOfWords = 1;
    for (int i = 0; i < strlen(text); i++)
    {
        char c = text[i];
        
        if (isspace(c))
        {
            numberOfWords++;
        }

    }
    return numberOfWords;
}

//Function that counts sentences
int countSentences(string text)
{
    int numberOfSentences = 0;
    for (int i = 0; i < strlen(text); i++)
    {
        if ((text[i] == '.') || (text[i] == '!') || (text[i] == '?'))
        {
            numberOfSentences++;
        }

    }
    return numberOfSentences;
}