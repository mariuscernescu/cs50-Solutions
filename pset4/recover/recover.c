#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int jpgCounter = 0;
    char name[8] = "";
    FILE *img;

    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }


    // Open the memory card
    FILE *card = fopen(argv[1], "r"); // Open in to a buffer[];
    if (!card)
    {
        return 1;
    }

    int returnx = 1;
    //Repeat until end of card:
    while (returnx == 1)
    {
        //Create the buffer
        unsigned char bytes[512];
        //Read 512 bytes into a buffer

        returnx = fread(bytes, 512, 1, card); // To read data-pointer/ size: 512bytes/ inptr : card.raw
        if (returnx != 1)
        {
            return 0;
        }

        // fread returnes the number of items of size 512 were read


        //If start of new JPEG
        if (bytes[0] == 0xff && bytes[1] == 0xd8 && bytes[2] == 0xff && (bytes[3] & 0xf0) == 0xe0)
        {

            //If first JPEG
            if (jpgCounter == 0)
            {

                //Create a new JPEG file.
                sprintf(name, "%03i.jpg", jpgCounter); // -> 001.jpeg
                img = fopen(name, "w");         // -> Open the new Jpeg file to write
                fwrite(bytes, 512, 1, img);               //->  fwrite(data, 512, 1, 001.jpeg)
                jpgCounter++;
            }
            else //Else not first JPEG
            {
                //Close the old JPEG
                fclose(img);


                //Open a new one
                sprintf(name, "%03i.jpg", jpgCounter);
                img = fopen(name, "w");

                //Write the 512bytes
                fwrite(bytes, 512, 1, img);
                jpgCounter++;
            }
        }
        else
        {
            //If already found JPEG
            if (jpgCounter > 0)
            {
                //Write on the file that is open
                fwrite(bytes, 512, 1, img);
            }
        }
    }
    //Close any reamaining files
    fclose(card);
    fclose(img);
}