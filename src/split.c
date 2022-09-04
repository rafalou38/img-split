#include <stdio.h>
#include <direct.h>

#define STB_IMAGE_IMPLEMENTATION
#include "lib/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "lib/stb_image_write.h"

#include "./common.h"

int split(char *path, int slices)
{
    char *const baseName = getBaseName(path);

    char filename[strlen(baseName)];
    getFileName(baseName, filename);

    _mkdir(filename);

    int width,
        height, n;
    unsigned char *source = stbi_load(path, &width, &height, &n, 0);
    if (source == NULL)
    {
        printf("Imposible d'ouvrir l'image %s", path);
        return 1;
    }

    int nH = height / slices;

    printf("Source image height: %d, new images height: %d\n", height, nH);

    // unsigned char buffer[width * nH * n];

    unsigned char *buffer = malloc(width * nH * n);

    for (int i = 0; i < slices; i++)
    {
        for (int x = 0; x < width; x++)
        {
            for (int y = 0; y < nH; y++)
            {
                int bufI = (width * y + x) * n;
                int srcI = (width * (nH * i + y) + x) * n;
                for (int comp = 0; comp < n; comp++)
                {
                    buffer[bufI + comp] = source[srcI + comp];
                }
            }
        }

        char outputFileName[100];
        sprintf(outputFileName, "%s/%04d-%04d.jpg", filename, i + 1, slices);
        printf("Writing %s\r", outputFileName);
        stbi_write_jpg(outputFileName, width, nH, n, buffer, 90);
    }

    stbi_image_free(source);
    free(buffer);

    return 0;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Merci de fournir un fichier\n");
        return end(1);
    }

    int slices = 0;
    printf("Nombre de découpes: ");
    scanf("%d", &slices);

    for (int i = 1; i < argc; i++)
    {
        if (split(argv[i], slices))
            return end(1);
    }

    return 0;
}