// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dictionary.h"
#include <stdint.h>
#include <strings.h>

unsigned int RSHash(const char* str, unsigned int length);



// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

bool destroy(node* head);

// Number of buckets in hash table
const unsigned int N = 100000;

// Hash table
node *table[N];

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // Hash word to obtain a hash value
   int value = hash(word);
    //Access linked list at the index in the hash table
    node *cursor = table[value];
    //Traverse linked list, looking for the word(strcasecmp)
    while (cursor)
    {
        char * word1 = cursor->word;
        if (strcasecmp(word1, word) == 0)
        {
            return true;
        }
        cursor = cursor->next;

    }
    return false;
}

//-----------------------------------------------------------------------------

//JS Hash Function
//A bitwise hash function written by Justin Sobel
//https://www.partow.net/programming/hashfunctions/#ConsistentHashing


unsigned int RSHash(const char* str, unsigned int length)
{
    int str1 = 0;
    int str2 = 0;
    for (int k = 0; k < strlen(str); k++)
    {
        if (str[k] < 97)
        {
            str2 = str[k] + 32;
        }
        else
        {
            str2 = str[k];
        }
        str1 = str1 + str2;
    }
   unsigned int hash = 1315423911;
   unsigned int i    = 0;

   for (i = 0; i < length; ++str1, ++i)
   {
      hash ^= ((hash << 5) + (str1) + (hash >> 2));
   }

   return hash;
}
//----------------------------------------------------------------------------------------------------

// Hashes word to a number
unsigned int hash(const char *word)
{
    unsigned int hash = RSHash(word, LENGTH);
    // printf("%i\n", hash);

    // printf("string: %s hash: %i\n", word, (hash % 10000));
    return hash % 100000;


}

int sizeOfDict = 0;
FILE *file;

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    char word[LENGTH+1];
    //Open dictionary file
    file = fopen(dictionary, "r");
    if(!file)
    {
        printf("file not opening ");
        return false;
    }
    //Read strings from file one at a time

    while(fscanf(file, "%s", word) != EOF)
    {
        //Create a new node for each word using malloc
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            return false;
            printf("malloc error");
        }
        strcpy(n->word, word);
        n->next = NULL;

        //Hash word to obtain a hash value
        int index = hash(word);


        //Insert node into hash table at that location
        if(!table[index]) // If is the first node.
        {
            table[index] = n;
        }
        else
        {
            n->next = table[index]->next;
            table[index]->next = n;
        }

        sizeOfDict++;
    }
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{

    return sizeOfDict;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    fclose(file);
    for (int i = 0; i < N; i++)
    {
        if (!table[i])
        {
            //Skip
        }
        else
        {
           destroy(table[i]);
        }

    }
    return true;
}

bool destroy(node* head)
{
    if (head->next)
    {
        destroy(head->next);
    }

    free(head);
    return true;
}