#include <stdio.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>

#define STB_IMAGE_IMPLEMENTATION
#include "lib/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "lib/stb_image_write.h"

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "lib/stb_image_resize.h"

#include "./common.h"

#define MAX_JOIN_FILES 100
#define JOIN_COMP 3

#define VERBOSE 0

#define OUT_EXT ".joined.jpg"

int getFilesFromFolder(char *path, char *files[MAX_JOIN_FILES])
{
    DIR *d;
    struct dirent *dir;
    d = opendir(path);
    if (!d)
        return 0;

    int i = 0;
    while ((dir = readdir(d)) != NULL)
    {
        // printf("%s\n", dir->d_name);
        if (i < 100)
        {
            // files[i] = (dir->d_name);
            files[i] = (char *)malloc(strlen(dir->d_name) + 1);
            // sprintf(files[i], "%s\\%s", path, dir->d_name);
            sprintf(files[i], "%s", dir->d_name);
            // strncpy(files[i], dir->d_name, strlen(dir->d_name) + 1);
#if VERBOSE
            printf("Discovered file: %s\n", files[i]);
#endif
            i++;
        }
        else
        {
            printf("Tu as atteint le maximum de fichier a join (%d), merci de recompiler avec une valeur de MAX_JOIN_FILES plus élevée", MAX_JOIN_FILES);
        }
    }
    closedir(d);

    sort(files, i);

    return i;
}

typedef struct
{
    char *filePath;
    int width;
    int height;
    bool resize;
} SourceImage;

void join(char *basePath, char *files[MAX_JOIN_FILES], int count)
{

    int targetWidth = 0;
    SourceImage sources[count];
    int sourceCnt = 0;

    // Load image infos
    for (int i = 0; i < count; i++)
    {
        int w, h, comp;

        char *path = malloc(strlen(basePath) + strlen(files[i]) + 1);
        sprintf(path, "%s\\%s", basePath, files[i]);

        if (!stbi_info(path, &w, &h, &comp))
        {
            printf("ERREUR: Impossible de charger l'image %s\n", files[i]);
            continue;
        };

        if (targetWidth == 0)
            targetWidth = w;
        else if (w < targetWidth)
            targetWidth = w;

        sources[sourceCnt].filePath = path;
        sources[sourceCnt].width = w;
        sources[sourceCnt].height = h;
        sources[sourceCnt].resize = false;
#if VERBOSE
        printf("Load info of %s: w=%d h=%d\n", files[i], w, h);
#endif
        sourceCnt++;
    }

    int targetHeight = 0;
    // Calculate resize
    for (int i = 0; i < sourceCnt; i++)
    {
        if (sources[i].width > targetWidth)
        {
            int nw = targetWidth;
            int nh = sources[i].height * nw / sources[i].width;

            printf("Image %s should be resized, %dx%d > %dx%d\n", sources[i].filePath, sources[i].width, sources[i].height, nw, nh);

            sources[i].width = nw;
            sources[i].height = nh;
            sources[i].resize = true;
        }

        targetHeight += sources[i].height;
    }

    unsigned char *buffer = malloc(targetWidth * targetHeight * JOIN_COMP);
    int currentHeight = 0;
    for (int i = 0; i < sourceCnt; i++)
    {
        SourceImage *src = &sources[i];
        int w, h, comp;
        unsigned char *data = stbi_load(src->filePath, &w, &h, &comp, JOIN_COMP);

        if (data != NULL)
        {
            if (sources[i].resize)
            {
                unsigned char *resized = malloc(sources[i].width * sources[i].height * JOIN_COMP);
                stbir_resize_uint8(data, w, h, 0, resized, src->width, src->height, 0, JOIN_COMP);
                free(data);
                data = resized;

                printf("Resized %s\n", src->filePath);
            }

            for (int x = 0; x < sources[i].width; x++)
            {
                for (int y = 0; y < sources[i].height; y++)
                {
                    int srcI = (src->width * y + x) * JOIN_COMP;
                    int bufI = (targetWidth * (currentHeight + y) + x) * JOIN_COMP;
                    for (int comp = 0; comp < JOIN_COMP; comp++)
                    {
                        buffer[bufI + comp] = data[srcI + comp];
                    }
                }
            }
            currentHeight += src->height;
            printf("Joined %s, current Height=%d/%d\r", src->filePath, currentHeight, targetHeight);
        }
        else
        {
            printf("%s - erreur, image ignorée\n", files[i]);
        }
    }

    printf("\n\n");

    char *baseName = getBaseName(basePath);

    char *outFileName = malloc(strlen(baseName) + strlen(OUT_EXT) + 1);
    // sprintf(outFileName, "%s%s", baseName, OUT_EXT);
    outFileName = strcat(baseName, OUT_EXT);
    stbi_write_jpg(outFileName, targetWidth, targetHeight, JOIN_COMP, buffer, 90);

    printf("Wrote %s\n", outFileName);

    free(buffer);
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Merci de fournir un dossier\n");
        return end(1);
    }

    for (int i = 1; i < argc; i++)
    {
        struct stat path_stat;
        stat(argv[i], &path_stat);
        if (!S_ISDIR(path_stat.st_mode))
        {
            printf("%s n'est pas un dossier!\n", argv[i]);
            continue;
        }

        char *files[MAX_JOIN_FILES];
        // getFilesFromFolder(argv[i], files);
        int count = getFilesFromFolder(argv[i], files);
        join(argv[i], files, count);
    }

    return end(0);
}