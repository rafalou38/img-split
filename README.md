# img split

This open source tool written in pure c can be used to split tall images in parts that can the be regrouped together in one single image.

## Usage

### From the commandline:

```
./split ./image.jpg|png|...
./join c:/path/to/folder/containing/images
```

### From the explorer:

**SPLIT:** select the files you want to split and drop them on the executable split.exe

**JOIN:** select the folders containing the images you want to join and drop them on the executable join.exe

## Additional notes

- When splitting
  - files are output in a folder with the name of the input file
  - You **can** provide multiple images at the same time
- When joining
  - output file has the name of the folder + `.joined.jpg`
  - You **can** provide multiple folders at the same time
  - Files are ordered by alphabetical order
  - If all the files don't have the same width all the images will be resized to the smallest one.

## Compiling

If you don't trust the official builds you can build it by yourself:

Run `build.bat`

```bat
gcc -Wall -Wextra -o split.exe ./src/split.c -lm
gcc -Wall -Wextra -o join.exe ./src/join.c -lm
```

> The code was developed on windows with **MinGW**, so you may not be able to compile for linux.

## Credit

The code uses the following libraries ([src/lib/](src/lib/)):

- [stb_image](https://github.com/nothings/stb/blob/master/stb_image_write.h)
- [stb_image_write](https://github.com/nothings/stb/blob/master/stb_image_write.h)
- [stb_image_resize](https://github.com/nothings/stb/blob/master/stb_image_write.h)
