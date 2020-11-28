#include <stdio.h>
#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

bool checkArg(int argc, string argv[]);
bool checkArgLen(string argv[]);
string cipher(string plainText, string key);

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./caesar KEY\n");  // Check if there is only one argument
        return 1; 
    }
    bool check = checkArg(argc, argv); //Function that checks if the in-line argument is valid
    if (strlen(argv[1]) != 26) 
    {
        printf("Key must contain 26 characters.\n"); // Check if the key = 26 char
        return 1;
    }
    else if (check == false)
    {
        printf("Usage: ./caesar KEY\n");
        return 1;
    }
    //Once the key is valid let's get the plain text
    string key = argv[1]; //Save the key in the variable "key"
    string plainText = get_string("plaintext: "); // Get the plain text from the user
    string cipherText = cipher(plainText, key);  //Cipher the text
    printf("ciphertext: %s\n", cipherText); //Return the ciphered text
}



//Function that enciphers the text
string cipher(string plainText, string key)
{
    string cipherText = plainText;
    for (int i = 0; i < strlen(plainText); i++)
    {
        for (int j = 0; j < strlen(key); j++)
        {
            if (islower(key[j])) // Make sure that all the characters of the key are uppercase, if not make them uppercase.
            {
                key[j] = key[j] - 32; 
            }
            
        }
        if (islower(plainText[i])) //if a charcater is lowercase
        {                          
            int position = plainText[i] - 97;   
            cipherText[i] = key[position] + 32; //Because our key is all uppercase at this point, we need to add 32
        }
        else if (isupper(plainText[i])) //if a charcater is uppercase
        {                               
            int position = plainText[i] - 65;
            cipherText[i] = key[position];
        }
    }
    return cipherText;
}


//Function that checks if the in-line argument is valid
bool checkArg(int argc, string argv[])
{
    if (argc == 2)
    {
        int notAlhpa = 0;
        int duplicat = 0;
        for (int i = 0; i < 26; i++)
        {
            if ((isalpha(argv[1][i])) == 0)
            {
                notAlhpa++;
            }
            for (int j = i + 1; j < (26 - i) ; j++)
            {
                if (argv[1][i] == argv[1][j])
                {
                    duplicat++;
                }
            }
        }
        if (notAlhpa > 0 || duplicat > 0)
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