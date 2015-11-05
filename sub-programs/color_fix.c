#include <stdio.h>
#include <stdlib.h>

void SwapColors(const char* filename);

int main(int argc, const char* argv[])
{
    if(argc < 2)
    {
        fprintf(stderr, "err : no param is given.\n");
        return -1;
    }

    int i;
    for(i = 1; i < argc; ++i)
    {
        printf("convert file : %s\n", argv[i]);
        SwapColors(argv[i]);
    }

    return 0;
}

void SwapColors(const char* filename)
{
    FILE* fp = fopen(filename, "rb+");
    if(fp == NULL)
    {
        fprintf(stderr, "err : fopen failed(filename : %s).\n", filename);
        exit(-1);
    }

    fseek(fp, 0, SEEK_END);
    const int bufsize = ftell(fp) - 26;
    char buf[bufsize];

    fseek(fp, 26, SEEK_SET); // skip header

    fread(buf, sizeof(char), bufsize, fp);
    char* p;
    for(p = buf; p < buf+bufsize; p += 3)
    {
        char tmp = p[0];
        p[0] = p[2];
        p[2] = tmp;
    }

    fseek(fp, 26, SEEK_SET);
    fwrite(buf, sizeof(char), bufsize, fp);

    fclose(fp);
}