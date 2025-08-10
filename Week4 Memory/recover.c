#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // Check proper usage
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }

    // Open forensic image
    FILE *inptr = fopen(argv[1], "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", argv[1]);
        return 1;
    }

    const int BLOCK = 512;
    BYTE buffer[BLOCK];
    FILE *img = NULL;
    int file_count = 0;
    char filename[8]; // "###.jpg" + '\0'
    size_t bytesRead;

    // Read until no more bytes
    while ((bytesRead = fread(buffer, 1, BLOCK, inptr)) > 0)
    {
        // Check for JPEG signature in first 4 bytes (if available)
        bool is_jpeg = bytesRead >= 4 &&
                       buffer[0] == 0xff &&
                       buffer[1] == 0xd8 &&
                       buffer[2] == 0xff &&
                       (buffer[3] & 0xf0) == 0xe0;

        // If start of a new JPEG
        if (is_jpeg)
        {
            // Close previous file if open
            if (img != NULL)
            {
                fclose(img);
            }

            // Create new filename and open for writing
            snprintf(filename, sizeof(filename), "%03d.jpg", file_count);
            img = fopen(filename, "w");
            if (img == NULL)
            {
                fprintf(stderr, "Could not create %s.\n", filename);
                fclose(inptr);
                return 1;
            }
            file_count++;
        }

        // If currently writing a JPEG, write the block (may be partial last block)
        if (img != NULL)
        {
            fwrite(buffer, 1, bytesRead, img);
        }
    }

    // Clean up
    if (img != NULL)
    {
        fclose(img);
    }
    fclose(inptr);

    return 0;
}
