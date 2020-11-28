#include <stdio.h>
#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

bool checkArg(int argc, string argv[]);
string encipherText(string text, int key);

int main(int argc, string argv[])
{

    int key;
    string text;
    bool check = checkArg(argc, argv); //Function that checks if the in-line argument is valid
    if (check == false)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }
    else
    {
        key = atoi(argv[1]); //Transforming the key from a string to a int
    }

    text = get_string("Text: "); //Get text to encipher from the user
    string encipheredText = encipherText(text, key); // Encipher the text using this function
    printf("ciphertext: %s\n", encipheredText);


}
//Function used to encipher text
string encipherText(string text, int key)
{
    string textClone = text;
    for (int i = 0; i < strlen(text); i++)
    {
        if (isalpha(text[i]))
        {
            if (isupper(text[i]) == 0) //dealing with lowercase chars
            {
                textClone[i] = (((text[i] - 97) + key) % 26) + 97;
            }
            else if (islower(text[i]) == 0) //dealing with uppercase chars
            {

                textClone[i] = (((text[i] - 65) + key) % 26) + 65;
            }
        }
        else
        {
            textClone[i] = text[i];
        }


    }
    return textClone;
}



//Function that checks if the in-line argument is valid
bool checkArg(int argc, string argv[])
{
    if (argc == 2)
    {
        int nondigit = 0;
        for (int i = 0; i < strlen(argv[1]); i++)
        {
            if ((isdigit(argv[1][i])) == 0)
            {
                nondigit++;
            }
        }
        if (nondigit > 0)
        {
            return false;
        }
        else
        {
            return true;
        }

    }
    else
    {
        return false;

    }

}
