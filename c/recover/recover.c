#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    //check if input correct file
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }

    //open file to read
    FILE *raw_file = fopen(argv[1], "r");

    // check if opens correctly
    if (!raw_file)
    {
        printf("Couldn't open file. Usage: ./recover IMAGE\n");
        return 1;
    }

    //block size
    const int BLOCK_SIZE = 512;
    BYTE buffer[BLOCK_SIZE];

    //jpeg file #000
    int jpeg_no = 0;

    //declare file to save img
    FILE *img = NULL;

    //filename is the name of the jpeg file name
    char filename[8];

    //loop for every block in raw_file till the end of the file
    while (fread(&buffer, 1, BLOCK_SIZE, raw_file) == BLOCK_SIZE)
    {
        //if jpeg create new jpeg file
        if (buffer[0] == 0xff &&
            buffer[1] == 0xd8 &&
            buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0)
        {
            //new jpeg found, close previous one
            if (jpeg_no > 0)
            {
                fclose(img);
            }

            //create name of jpeg in order ###.jpeg
            sprintf(filename, "%03i.jpg", jpeg_no);

            //open and write file
            img = fopen(filename, "w"); //open jpeg file
            fwrite(&buffer, 1, BLOCK_SIZE, img);

            //counter of jpeg files for next one
            jpeg_no++;
        }

        //if not new jpeg file, keep writing in opened file
        else if (jpeg_no > 0)
        {
            fwrite(&buffer, 1, BLOCK_SIZE, img);
        }
    }

    //close all files
    fclose(raw_file);
    fclose(img);
    return 0;
}