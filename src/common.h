char *getBaseName(char *path)
{
    char *copy = path + strlen(path);
    for (; copy > path; copy--)
    {
        if ((*copy == '\\') || (*copy == '/'))
        {
            copy++;
            break;
        }
    }
    return copy;
}

void getFileName(char *base_name, char *out)
{
    int i = 0;
    for (; base_name[i] != 0 && base_name[i] != '.'; i++)
        out[i] = base_name[i];

    out[i] = 0;
}

int end(int code)
{
    printf("[PRESS ENTER TO CLOSE]");
    char c;
    scanf("%c", &c);
    return code;
}

int cmpstr(void const *a, void const *b)
{
    char const *aa = (char const *)a;
    char const *bb = (char const *)b;

    return strcmp(aa, bb);
}

void sort(char **arr, int size)
{
    int last = size - 1;
    while (last > 0)
    {
        for (int i = 0; i < last; i++)
        {
            if (!strcmp(arr[i], arr[i + 1]))
            {
                char *tmp = arr[i];
                arr[i] = arr[i + 1];
                arr[i + 1] = tmp;
            }
        }
        last--;
    }
}