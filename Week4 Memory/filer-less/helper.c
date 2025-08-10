#include "helpers.h"
#include <math.h>
#include <stdlib.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            RGBTRIPLE *p = &image[i][j];
            float avg = (p->rgbtRed + p->rgbtGreen + p->rgbtBlue) / 3.0f;
            int gray = (int) roundf(avg);
            p->rgbtRed = p->rgbtGreen = p->rgbtBlue = (BYTE) gray;
        }
    }
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            RGBTRIPLE orig = image[i][j];

            int sepiaRed = (int) roundf(0.393f * orig.rgbtRed + 0.769f * orig.rgbtGreen + 0.189f * orig.rgbtBlue);
            int sepiaGreen = (int) roundf(0.349f * orig.rgbtRed + 0.686f * orig.rgbtGreen + 0.168f * orig.rgbtBlue);
            int sepiaBlue = (int) roundf(0.272f * orig.rgbtRed + 0.534f * orig.rgbtGreen + 0.131f * orig.rgbtBlue);

            if (sepiaRed > 255) sepiaRed = 255;
            if (sepiaGreen > 255) sepiaGreen = 255;
            if (sepiaBlue > 255) sepiaBlue = 255;

            image[i][j].rgbtRed = (BYTE) sepiaRed;
            image[i][j].rgbtGreen = (BYTE) sepiaGreen;
            image[i][j].rgbtBlue = (BYTE) sepiaBlue;
        }
    }
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            RGBTRIPLE tmp = image[i][j];
            image[i][j] = image[i][width - 1 - j];
            image[i][width - 1 - j] = tmp;
        }
    }
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE(*copy)[width] = malloc(sizeof(RGBTRIPLE) * height * width);
    if (copy == NULL)
    {
        return; // out of memory: do nothing
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int sumR = 0, sumG = 0, sumB = 0;
            int count = 0;

            for (int di = -1; di <= 1; di++)
            {
                for (int dj = -1; dj <= 1; dj++)
                {
                    int ni = i + di;
                    int nj = j + dj;
                    if (ni >= 0 && ni < height && nj >= 0 && nj < width)
                    {
                        sumR += copy[ni][nj].rgbtRed;
                        sumG += copy[ni][nj].rgbtGreen;
                        sumB += copy[ni][nj].rgbtBlue;
                        count++;
                    }
                }
            }

            image[i][j].rgbtRed = (BYTE) roundf((float) sumR / count);
            image[i][j].rgbtGreen = (BYTE) roundf((float) sumG / count);
            image[i][j].rgbtBlue = (BYTE) roundf((float) sumB / count);
        }
    }

    free(copy);
}
