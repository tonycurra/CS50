#include "helpers.h"
#include <math.h>


// *********************change ROUND(), approssimates wrong, what other function? check previous *********************
//*****************************************************************************


// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    //get value for every pixel, then save the new value for RGB
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // rgb value average = () red + green + blue ) / 3
            // round average to nearest integer
            int k = round((float)(image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3);
            //set all rgb values of that pixel to the average to get greyscale
            image[i][j].rgbtBlue = k;
            image[i][j].rgbtGreen = k;
            image[i][j].rgbtRed = k;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // calc value for rgb sepia (formula on filter file cs50)
            int sR = (int)round(.393 * image[i][j].rgbtRed + .769 * image[i][j].rgbtGreen + .189 * image[i][j].rgbtBlue);
            int sG = (int)round(.349 * image[i][j].rgbtRed + .686 * image[i][j].rgbtGreen + .168 * image[i][j].rgbtBlue);
            int sB = (int)round(.272 * image[i][j].rgbtRed + .534 * image[i][j].rgbtGreen + .131 * image[i][j].rgbtBlue);

            //if >255 set to 255
            if (sR > 255)
            {
                sR = 255;
            }
            if (sG > 255)
            {
                sG = 255;
            }
            if (sB > 255)
            {
                sB = 255;
            }

            //new values in image
            image[i][j].rgbtBlue = sB;
            image[i][j].rgbtGreen = sG;
            image[i][j].rgbtRed = sR;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    //mirror
    for (int i = 0; i < height; i++)
    {
        // create array and fill with rgb for the line
        int b[width];
        int g[width];
        int r[width];
        //k for the line to fill array
        for (int k = 0; k < width ; k++)
        {
            b[k] = image[i][k].rgbtBlue;
            g[k] = image[i][k].rgbtGreen;
            r[k] = image[i][k].rgbtRed;
        }

        for (int j = 0; j < width; j++)
        {
            // create int to store value
            //exchange in the image
            image[i][j].rgbtBlue = b[width - j - 1];
            image[i][j].rgbtGreen = g[width - j - 1];
            image[i][j].rgbtRed = r[width - j - 1];
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    //create another array to store new data for rgb without screwing the image
    int br[height][width];
    int bg[height][width];
    int bb[height][width];

    // this loop does not touch borders
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //top border
            if (i == 0 && j != 0 && j != (width - 1))
            {
                bb[i][j] = round((float)(image[i][j - 1].rgbtBlue + image[i][j].rgbtBlue + image[i][j + 1].rgbtBlue +
                                         image[i + 1][j - 1].rgbtBlue + image[i + 1][j].rgbtBlue + image[i + 1][j + 1].rgbtBlue) / 6);

                bg[i][j] = round((float)(image[i][j - 1].rgbtGreen + image[i][j].rgbtGreen + image[i][j + 1].rgbtGreen +
                                         image[i + 1][j - 1].rgbtGreen + image[i + 1][j].rgbtGreen + image[i + 1][j + 1].rgbtGreen) / 6);

                br[i][j] = round((float)(image[i][j - 1].rgbtRed + image[i][j].rgbtRed + image[i][j + 1].rgbtRed +
                                         image[i + 1][j - 1].rgbtRed + image[i + 1][j].rgbtRed + image[i + 1][j + 1].rgbtRed) / 6);
            }
            //bottom border
            else if (i == (height - 1) && j != 0 && j != (width - 1))
            {
                bb[i][j] = round((float)(image[i - 1][j - 1].rgbtBlue + image[i - 1][j].rgbtBlue + image[i - 1][j + 1].rgbtBlue +
                                         image[i][j - 1].rgbtBlue + image[i][j].rgbtBlue + image[i][j + 1].rgbtBlue) / 6);

                bg[i][j] = round((float)(image[i - 1][j - 1].rgbtGreen + image[i - 1][j].rgbtGreen + image[i - 1][j + 1].rgbtGreen +
                                         image[i][j - 1].rgbtGreen + image[i][j].rgbtGreen + image[i][j + 1].rgbtGreen) / 6);

                br[i][j] = round((float)(image[i - 1][j - 1].rgbtRed + image[i - 1][j].rgbtRed + image[i - 1][j + 1].rgbtRed +
                                         image[i][j - 1].rgbtRed + image[i][j].rgbtRed + image[i][j + 1].rgbtRed) / 6);
            }
            //left border
            else if (j == 0)
            {
                //left top corner
                if (i == 0)
                {
                    bb[i][j] = round((float)(image[i][j].rgbtBlue + image[i][j + 1].rgbtBlue +
                                             image[i + 1][j].rgbtBlue + image[i + 1][j + 1].rgbtBlue) / 4);

                    bg[i][j] = round((float)(image[i][j].rgbtGreen + image[i][j + 1].rgbtGreen +
                                             image[i + 1][j].rgbtGreen + image[i + 1][j + 1].rgbtGreen) / 4);

                    br[i][j] = round((float)(image[i][j].rgbtRed + image[i][j + 1].rgbtRed +
                                             image[i + 1][j].rgbtRed + image[i + 1][j + 1].rgbtRed) / 4);
                }
                //left bottom corner
                else if (i == (height - 1))
                {
                    bb[i][j] = round((float)(image[i - 1][j].rgbtBlue + image[i - 1][j + 1].rgbtBlue +
                                             image[i][j].rgbtBlue + image[i][j + 1].rgbtBlue) / 4);

                    bg[i][j] = round((float)(image[i - 1][j].rgbtGreen + image[i - 1][j + 1].rgbtGreen +
                                             image[i][j].rgbtGreen + image[i][j + 1].rgbtGreen) / 4);

                    br[i][j] = round((float)(image[i - 1][j].rgbtRed + image[i - 1][j + 1].rgbtRed +
                                             image[i][j].rgbtRed + image[i][j + 1].rgbtRed) / 4);
                }
                else
                {
                    bb[i][j] = round((float)(image[i - 1][j].rgbtBlue + image[i - 1][j + 1].rgbtBlue + image[i][j].rgbtBlue +
                                             image[i][j + 1].rgbtBlue + image[i + 1][j].rgbtBlue + image[i + 1][j + 1].rgbtBlue) / 6);
                    bg[i][j] = round((float)(image[i - 1][j].rgbtGreen + image[i - 1][j + 1].rgbtGreen + image[i][j].rgbtGreen +
                                             image[i][j + 1].rgbtGreen + image[i + 1][j].rgbtGreen + image[i + 1][j + 1].rgbtGreen) / 6);
                    br[i][j] = round((float)(image[i - 1][j].rgbtRed + image[i - 1][j + 1].rgbtRed + image[i][j].rgbtRed +
                                             image[i][j + 1].rgbtRed + image[i + 1][j].rgbtRed + image[i + 1][j + 1].rgbtRed) / 6);
                }
            }
            //right border
            else if (j == (width - 1))
            {
                //right top corner
                if (i == 0)
                {
                    bb[i][j] = round((float)(image[i][j - 1].rgbtBlue + image[i][j].rgbtBlue +
                                             image[i + 1][j - 1].rgbtBlue + image[i + 1][j].rgbtBlue) / 4);

                    bg[i][j] = round((float)(image[i][j - 1].rgbtGreen + image[i][j].rgbtGreen +
                                             image[i + 1][j - 1].rgbtGreen + image[i + 1][j].rgbtGreen) / 4);

                    br[i][j] = round((float)(image[i][j - 1].rgbtRed + image[i][j].rgbtRed +
                                             image[i + 1][j - 1].rgbtRed + image[i + 1][j].rgbtRed) / 4);
                }
                //right bottom corner
                else if (i == (height - 1))
                {
                    bb[i][j] = round((float)(image[i - 1][j - 1].rgbtBlue + image[i - 1][j].rgbtBlue +
                                             image[i][j - 1].rgbtBlue + image[i][j].rgbtBlue) / 4);

                    bg[i][j] = round((float)(image[i - 1][j - 1].rgbtGreen + image[i - 1][j].rgbtGreen +
                                             image[i][j - 1].rgbtGreen + image[i][j].rgbtGreen) / 4);

                    br[i][j] = round((float)(image[i - 1][j - 1].rgbtRed + image[i - 1][j].rgbtRed +
                                             image[i][j - 1].rgbtRed + image[i][j].rgbtRed) / 4);
                }
                else
                {
                    bb[i][j] = round((float)(image[i - 1][j - 1].rgbtBlue + image[i - 1][j].rgbtBlue + image[i][j - 1].rgbtBlue +
                                             image[i][j].rgbtBlue + image[i + 1][j - 1].rgbtBlue + image[i + 1][j].rgbtBlue) / 6);
                    bg[i][j] = round((float)(image[i - 1][j - 1].rgbtGreen + image[i - 1][j].rgbtGreen + image[i][j - 1].rgbtGreen +
                                             image[i][j].rgbtGreen + image[i + 1][j - 1].rgbtGreen + image[i + 1][j].rgbtGreen) / 6);
                    br[i][j] = round((float)(image[i - 1][j - 1].rgbtRed + image[i - 1][j].rgbtRed + image[i][j - 1].rgbtRed +
                                             image[i][j].rgbtRed + image[i + 1][j - 1].rgbtRed + image[i + 1][j].rgbtRed) / 6);
                }
            }
            //any other pixel, blur average pixel 3x3, include itself
            else
            {
                bb[i][j] = round((float)(image[i - 1][j - 1].rgbtBlue + image[i - 1][j].rgbtBlue + image[i - 1][j + 1].rgbtBlue +
                                         image[i][j - 1].rgbtBlue + image[i][j].rgbtBlue + image[i][j + 1].rgbtBlue +
                                         image[i + 1][j - 1].rgbtBlue + image[i + 1][j].rgbtBlue + image[i + 1][j + 1].rgbtBlue) / 9);

                bg[i][j] = round((float)(image[i - 1][j - 1].rgbtGreen + image[i - 1][j].rgbtGreen + image[i - 1][j + 1].rgbtGreen +
                                         image[i][j - 1].rgbtGreen + image[i][j].rgbtGreen + image[i][j + 1].rgbtGreen +
                                         image[i + 1][j - 1].rgbtGreen + image[i + 1][j].rgbtGreen + image[i + 1][j + 1].rgbtGreen) / 9);

                br[i][j] = round((float)(image[i - 1][j - 1].rgbtRed + image[i - 1][j].rgbtRed + image[i - 1][j + 1].rgbtRed +
                                         image[i][j - 1].rgbtRed + image[i][j].rgbtRed + image[i][j + 1].rgbtRed +
                                         image[i + 1][j - 1].rgbtRed + image[i + 1][j].rgbtRed + image[i + 1][j + 1].rgbtRed) / 9);
            }
        }
    }

    //set image == new array with blur rgb
    for (int p = 0; p < height; p++)
    {
        for (int q = 0; q < width; q++)
        {
            image[p][q].rgbtBlue = bb[p][q];
            image[p][q].rgbtGreen = bg[p][q];
            image[p][q].rgbtRed = br[p][q];
        }
    }
    return;
}
