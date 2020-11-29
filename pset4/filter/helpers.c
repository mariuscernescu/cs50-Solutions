#include "helpers.h"
#include <stdio.h>
#include <math.h>

void calculate(int height, int width, int i, int k, RGBTRIPLE image[height][width], RGBTRIPLE image2[height][width]);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    //Loop through the image. Every pixel from every row
    for (int i = 0; i < height; i++) // Every row
    {
        for (int k = 0; k < width; k++) // Every pixel in every row.
        {
            //Make the pixel gray
            image[i][k].rgbtBlue = round(((float)image[i][k].rgbtBlue + (float)image[i][k].rgbtGreen + (float)image[i][k].rgbtRed) / 3);
            image[i][k].rgbtGreen = image[i][k].rgbtBlue;
            image[i][k].rgbtRed = image[i][k].rgbtBlue;
        }
    }

}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    //Calculate each new color value using sepia forumula
    for (int i = 0; i < height; i++)
    {
        for (int k = 0; k < width; k++)
        {
            int red = image[i][k].rgbtRed;
            int green = image[i][k].rgbtGreen;
            int blue = image[i][k].rgbtBlue;

            int sepiaRed, sepiaGreen, sepiaBlue;

            sepiaRed = round((0.393 * (double)red) + (0.769 * (double)green) + (0.189 * (double)blue));
            if (sepiaRed > 255)
            {
                image[i][k].rgbtRed = 255;
            }
            else if (sepiaRed <= 0)
            {
                image[i][k].rgbtRed = 0;
            }
            else
            {
                image[i][k].rgbtRed = sepiaRed;
            }

            sepiaGreen = round((0.349 * (double)red) + (0.686 * (double)green) + (0.168 * (double)blue));
            if (sepiaGreen > 255)
            {
                image[i][k].rgbtGreen = 255;
            }
            else if (sepiaGreen <= 0)
            {
                image[i][k].rgbtGreen = 0;
            }
            else
            {
                image[i][k].rgbtGreen = sepiaGreen;
            }

            sepiaBlue = round((0.272 * (double)red) + (0.534 * (double)green) + (0.131 * (double)blue));
            if (sepiaBlue > 255)
            {
                image[i][k].rgbtBlue = 255;
            }
            else if (sepiaBlue <= 0)
            {
                image[i][k].rgbtBlue = 0;
            }
            else
            {
                image[i][k].rgbtBlue = sepiaBlue;
            }
        }
    }

}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        int first = 0;
        int last = width - 1;
        while (first < last)
        {
            RGBTRIPLE swap;
            swap = image[i][first];
            image[i][first] = image[i][last];
            image[i][last] = swap;

            first++;
            last--;
        }

    }
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE image2[height][width]; // Where we store the new values
    for (int i = 0; i < height; i++)
    {
        for (int k = 0; k < width; k++)
        {
            //for every pixel calculate the average for RGB values of sounding pixels.
            calculate(height, width, i, k, image, image2);
        }
    }
    for (int i = 0; i < height; i++)
    {
        for (int k = 0; k < width; k++)
        {
            image[i][k] = image2[i][k];
        }
    }
}

void calculate(int height, int width, int i, int k, RGBTRIPLE image[height][width], RGBTRIPLE image2[height][width])
{
    int valueRed = 0;
    int valueGreen = 0;
    int valueBlue = 0;
    int  cycle = 0;
    for (int m = i - 1; m < i + 2; m++)
    {
        for (int n = k - 1; n < k + 2; n++)
        {
            if (m >= 0 && m <= height - 1 && n >= 0 && n <= width - 1)
            {
                valueRed += image[m][n].rgbtRed;
                valueGreen += image[m][n].rgbtGreen;
                valueBlue += image[m][n].rgbtBlue;
                cycle++;
            }
        }
    }
    float one = (float)valueRed / (float)cycle;
    float two = (float)valueGreen / (float)cycle;
    float three = (float)valueBlue / (float)cycle;

    image2[i][k].rgbtRed = round(one);
    image2[i][k].rgbtGreen = round(two);
    image2[i][k].rgbtBlue = round(three);

}