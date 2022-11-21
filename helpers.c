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
    int Gy[3][3] = {{-1,-2-1}, {0,0,0}, {1,2,1}};
    
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int sumBlueGx = 0, sumGreenGx = 0, sumRedGx = 0, sumBlueGy = 0, sumGreenGy = 0, sumRedGy = 0;
            int rowCounter = 0, colCounter = 0;

            // Selecting the nine pixels and taking a weighted sum.
            for (int row = -1; row < 2; row++)
            {
                for (int col = -1; col < 2; col++)
                {
                    if ((((i + row) < 0) | ((j + col) < 0) | ((i + row) == height) | ((j + col) == width)) == 0)
                    {   
                        // Convolution & Sum of Gx
                        int blueGx = image[i + row][j + col].rgbtBlue * Gx[rowCounter][colCounter];
                        int greenGx = image[i + row][j + col].rgbtGreen * Gx[rowCounter][colCounter];
                        int redGx = image[i + row][j + col].rgbtRed * Gx[rowCounter][colCounter];

                        sumBlueGx += blueGx;
                        sumGreenGx += greenGx;
                        sumRedGx += redGx;

                        // Convolution & Sum of Gy
                        int blueGy = image[i + row][j + col].rgbtBlue * Gy[rowCounter][colCounter];
                        int greenGy = image[i + row][j + col].rgbtGreen * Gy[rowCounter][colCounter];
                        int redGy = image[i + row][j + col].rgbtRed * Gy[rowCounter][colCounter];

                        sumBlueGy += blueGy;
                        sumGreenGy += greenGy;
                        sumRedGy += redGy;
                    }
                    colCounter++;
                }
                rowCounter++;
            }

            double sumBlue = sqrt( pow((double)sumBlueGx, 2) + pow((double)sumBlueGy, 2) );
            double sumGreen = sqrt( pow((double)sumGreenGx, 2) + pow((double)sumGreenGy, 2) );
            double sumRed = sqrt( pow((double)sumRedGx, 2) + pow((double)sumRedGy, 2) );

            int sumIntBlue = (int)sumBlue % 256;
            int sumIntGreen = (int)sumGreen % 256;
            int sumIntRed = (int)sumRed % 256;

            image[i][j].rgbtBlue = sumBlue;
            image[i][j].rgbtGreen = sumGreen;
            image[i][j].rgbtRed = sumRed;
            
        }
    }
    return;
}
