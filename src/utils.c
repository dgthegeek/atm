#include "header.h"

void flushBuffer()
{
    int c;
    while ((c = fgetc(stdin)) != EOF && c != '\n')
    {
        // Reading characters until newline or end-of-file is encountered
    }
}

void flushInputBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
    }
}

short Atoi(char text[255])
{
    size_t i;
    for (i = 0; i < strlen(text); i++)
    {
        int r = text[i];
        if ((r < 48 || r > 57))
        {
            return -1;
        }
    }
    return 1;
}

void Scanner(int *a)
{
    char text[255];
invalid:
    scanf("%[^\n]s", text);
    short b = Atoi(text);
    if (b == -1)
    {
        printf("Please Enter a valid Int\n");
        flushInputBuffer();
        goto invalid;
    }
    *a = atoi(text);
    if (*a > INT_MAX)
    {
        printf("Please Enter a valid Int\n");
        flushInputBuffer();
        goto invalid;
    }
}