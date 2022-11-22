#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int average = round((image[i][j].rgbtGreen + image[i][j].rgbtBlue + image[i][j].rgbtRed) / 3);
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
// t stands for triple
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int sumBlue = 0, sumGreen = 0, sumRed = 0;
            int avgBlue, avgGreen, avgRed;

            // selecting the nine rows around it
            for (int row = -1; row < 2; row++)
            {
                for (int col = -1; col < 2; col++)
                {
                    if ((((i + row) < 0) | ((j + col) < 0) | ((i + row) == height) | ((j + col) == width)) == 0)
                    {
                        sumBlue += image[i + row][j + col].rgbtBlue;
                        sumGreen += image[i + row][j + col].rgbtGreen;
                        sumRed += image[i + row][j + col].rgbtRed;
                    }
                }
            }

            avgBlue = round(sumBlue / 9);
            avgGreen = round(sumGreen / 9);
            avgRed = round(sumRed / 9);
            image[i][j].rgbtBlue = avgBlue;
            image[i][j].rgbtGreen = avgGreen;
            image[i][j].rgbtRed = avgRed;
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    int Gx[3][3] = {{-1,0,1}, {-2,0,2}, {-1,0,1}};
    int Gy[3][3] = {{-1,-2,-1}, {0,0,0}, {1,2,1}};

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int kRow = 0, kCol = 0;
            int sumGreenGx, sumRedGx, sumBlueGx, sumGreenGy, sumRedGy, sumBlueGy;
            sumGreenGx = sumRedGx = sumBlueGx = sumGreenGy = sumRedGy = sumBlueGy = 0;
            
            for (int row = -1; row < 2; row++)
            {
                for (int col = -1; col < 2; col++)
                {
                    int greenGx = image[i + row][j + col].rgbtGreen * Gx[kRow][kCol];
                    int redGx = image[i + row][j + col].rgbtRed * Gx[kRow][kCol];
                    int blueGx = image[i + row][j + col].rgbtBlue * Gx[kRow][kCol];

                    int greenGy = image[i + row][j + col].rgbtGreen * Gy[kRow][kCol];
                    int redGy = image[i + row][j + col].rgbtRed * Gy[kRow][kCol];
                    int blueGy = image[i + row][j + col].rgbtBlue * Gy[kRow][kCol];

                    sumGreenGx += greenGx;
                    sumRedGx += redGx;
                    sumBlueGx += blueGx;

                    sumGreenGy += greenGy;
                    sumRedGy += redGy;
                    sumBlueGy += blueGy;

                    kCol++;
                }
                kCol = 0;
                kRow++;
            }

            int newGreen = sqrt(pow(sumGreenGx, 2) + pow(sumGreenGy, 2));
            int newRed = sqrt(pow(sumRedGx, 2) + pow(sumRedGy, 2));
            int newBlue = sqrt(pow(sumBlueGx, 2) + pow(sumBlueGy, 2));
            
            image[i][j].rgbtGreen = newGreen > 255? 255: newGreen;
            image[i][j].rgbtRed = newRed > 255? 255: newRed;
            image[i][j].rgbtBlue = newBlue > 255? 255: newBlue;
        }
    }
    return;
}