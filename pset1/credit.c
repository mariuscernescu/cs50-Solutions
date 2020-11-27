#include <stdio.h>
#include <cs50.h>

int main(void)

{
    long card = get_long("Number: "); //Get CC nr.
    long cardUntouched = card;


    int otherDigit = 0;
    int beforeLastDigit = 0;
    int sum = 0;
    int sumSecond = 0;
    //Check the checksum to validate if card is a valid cc nr.
    while (card > 10)
    {
        beforeLastDigit = (card % 10); // Get the last digit.
        card = (card - beforeLastDigit) / 10; // Remove the last digit from the card.

        otherDigit += beforeLastDigit; // Get the digits starting with the last one added together.

        beforeLastDigit = (card % 10);
        card = (card - beforeLastDigit) / 10; // Get the second to last digit

        sumSecond = beforeLastDigit * 2; //Add digit * 2 to see later if it is < 1;0

        if (beforeLastDigit < 10 && sumSecond < 10) // Check if digit * 2 > 10.
        {
            sum += beforeLastDigit * 2;
        }
        else
        {
            int sumE = sumSecond;
            sumE = (sumSecond % 10);
            int sumX = (sumSecond - sumE) / 10;  // If sum > 10 then get digits one by one for ex. if sum = 13 => 1+3.
            sum = sum + sumX + sumE;
        }

    }


    if (card < 10 && card > 0)
    {
        otherDigit += card;
        card = card - card;
    }


    int total = sum + otherDigit;
    if (total % 10 == 0) // Check is is a valid CC nr.
    {
        long int cardcopy  = cardUntouched;
        long int cardcopytwo = cardUntouched;
        int i;

        for (i = 0; cardcopy > 0; i++) // find the number of digits
        {

            cardcopy = (cardcopy - (cardcopy % 10)) / 10; // Remove the last digit from the card.

        }
        while (cardcopytwo > 99)
        {
            cardcopytwo = (cardcopytwo - (cardcopytwo % 10)) / 10; // Get the first 2 digits in cardcopy variable

        }


        if (i == 15 && (cardcopytwo == 34 || cardcopytwo == 37)) // i = nr of digits cardcopy = first 2 digits
        {
            printf("AMEX\n");
        }
        else if (i == 16 && (cardcopytwo <= 55 && cardcopytwo >= 51)) // i = nr of digits cardcopy = first 2 digits
        {
            printf("MASTERCARD\n");
        }
        else if ((i == 16 || i == 13) && (((cardcopytwo - (cardcopytwo % 10)) / 10) == 4)) // i = nr of digits cardcopy = first 2 digits
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