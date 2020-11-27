#include <stdio.h>
#include <cs50.h>

int main(void)
{
    int height;
    do
    {
        height = get_int("Height: "); //Ask for the height of the piramide
    
    }
    while (height < 1 || height > 8); //Check if the value is between 1 and 8
    
    for (int i = 0; i < height; i++)
    {
        
        for (int k = 1; k < height - i ; k++)
        {
            printf(" "); //Print spaces
        }
        for (int j = 0; j <= i; j++)
        {
            
            printf("#"); //Print #
        }
        printf("  ");
        for (int j = 0; j <= i; j++)
        {
            
            printf("#"); //Print #
        }
        
        printf("\n"); //New line
    }
    
    
 
  
    
} 