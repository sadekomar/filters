#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int average = round((image[i][j].rgbtGreen + image[i][j].rgbtBlue + image[i][j].rgbtRed) / 3.0);
            image[i][j].rgbtBlue = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtRed = average;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            RGBTRIPLE buffer = image[i][j];
            image[i][j] = image[i][width - j - 1];
            image[i][width - j - 1] = buffer;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE tempImage[height][width];
    // t stands for triple
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int sumBlue = 0, sumGreen = 0, sumRed = 0;
            int counter = 0;

            // selecting the nine rows around it
            for (int row = -1; row < 2; row++)
            {
                for (int col = -1; col < 2; col++)
                {
                    if ((((i + row) < 0) || ((j + col) < 0) || ((i + row) == height) || ((j + col) == width)) == 0)
                    {
                        sumBlue += image[i + row][j + col].rgbtBlue;
                        sumGreen += image[i + row][j + col].rgbtGreen;
                        sumRed += image[i + row][j + col].rgbtRed;

                        counter++;
                    }
                }
            }

            int avgBlue = round(sumBlue / (double) counter);
            int avgGreen = round(sumGreen / (double) counter);
            int avgRed = round(sumRed / (double) counter);
            tempImage[i][j].rgbtBlue = avgBlue;
            tempImage[i][j].rgbtGreen = avgGreen;
            tempImage[i][j].rgbtRed = avgRed;
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = tempImage[i][j];
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE tempImage[height][width];

    int Gx[3][3] = {{-1,0,1}, {-2,0,2}, {-1,0,1}};
    int Gy[3][3] = {{-1,-2,-1}, {0,0,0}, {1,2,1}};

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int greenGx, redGx, blueGx, greenGy, redGy, blueGy;
            greenGx = redGx = blueGx = greenGy = redGy = blueGy = 0;

            for (int row = -1; row < 2; row++)
            {
                for (int col = -1; col < 2; col++)
                {
                    if (((i + row) < 0) || ((j + col) < 0) || ((i + row) >= height) || ((j + col) >= width))
                    {
                        // nothing
                    }
                    else
                    {
                        greenGx = greenGx + (image[i + row][j + col].rgbtGreen * Gx[row + 1][col + 1]);
                        redGx = redGx + (image[i + row][j + col].rgbtRed * Gx[row + 1][col + 1]);
                        blueGx = blueGx + (image[i + row][j + col].rgbtBlue * Gx[row + 1][col + 1]);

                        greenGy = greenGy + (image[i + row][j + col].rgbtGreen * Gy[row + 1][col + 1]);
                        redGy = redGy + (image[i + row][j + col].rgbtRed * Gy[row + 1][col + 1]);
                        blueGy = blueGy + (image[i + row][j + col].rgbtBlue * Gy[row + 1][col + 1]);
                    }
                }
            }

            // int newGreen = sqrt(pow(greenGx, 2) + pow(greenGy, 2));
            // int newRed = sqrt(pow(redGx, 2) + pow(redGy, 2));
            // int newBlue = sqrt(pow(blueGx, 2) + pow(blueGy, 2));

            int newGreen = round(sqrt(greenGx * greenGx + greenGy * greenGy));
            int newRed = round(sqrt(redGx * redGx + redGy * redGy));
            int newBlue = round(sqrt(blueGx * blueGx + blueGy * blueGy));

            tempImage[i][j].rgbtGreen = newGreen > 255? 255: newGreen;
            tempImage[i][j].rgbtRed = newRed > 255? 255: newRed;
            tempImage[i][j].rgbtBlue = newBlue > 255? 255: newBlue;
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = tempImage[i][j];
        }
    }

    return;
}
