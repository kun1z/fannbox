#include "common.h"
//----------------------------------------------------------------------------------------------------------------------
u32 tick(void)
{
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    return (now.tv_sec * 1000UL) + (now.tv_nsec / 1000000UL);
}
//----------------------------------------------------------------------------------------------------------------------
struct digit * load_digits(ui * const count, s8 const * const filename_labels, s8 const * const filename_images)
{
    *count = 0;

    FILE * const fl = fopen(filename_labels, "rb");
    if (!fl)
    {
        printf("file %s not found!\n", filename_labels);
        exit(EXIT_FAILURE);
    }

    u32 magic;
    fread(&magic, 1, 4, fl);
    magic = bswap_32(magic);
    if (magic != 2049)
    {
        printf("file %s is invalid!\n", filename_labels);
        exit(EXIT_FAILURE);
    }

    FILE * const fi = fopen(filename_images, "rb");
    if (!fi)
    {
        printf("file %s not found!\n", filename_images);
        exit(EXIT_FAILURE);
    }

    fread(&magic, 1, 4, fi);
    magic = bswap_32(magic);
    if (magic != 2051)
    {
        printf("file %s is invalid!\n", filename_images);
        exit(EXIT_FAILURE);
    }

    u32 count1, count2;
    fread(&count1, 1, 4, fl);
    fread(&count2, 1, 4, fi);
    count1 = bswap_32(count1);
    count2 = bswap_32(count2);
    if (count1 != count2 || !count1 || !count2)
    {
        printf("files %s and %s contain invalid counts!\n", filename_labels, filename_images);
        exit(EXIT_FAILURE);
    }

    u32 row, col;
    fread(&row, 1, 4, fi);
    fread(&col, 1, 4, fi);
    row = bswap_32(row);
    col = bswap_32(col);
    if (row != col || row != 28)
    {
        printf("files %s and %s contain invalid rows and columns!\n", filename_labels, filename_images);
        exit(EXIT_FAILURE);
    }

    struct digit * const data = malloc(count1 * sizeof(struct digit));
    if (!data)
    {
        printf("malloc() failed!\n");
        exit(EXIT_FAILURE);
    }

    printf("Loading %u samples from %s and %s...\n", count1, filename_labels, filename_images);

    const u32 start = tick();

    for (ui i=0;i<count1;i++)
    {
        for (ui y=0;y<28;y++)
        {
            for (ui x=0;x<28;x++)
            {
                u8 pixel;
                fread(&pixel, 1, 1, fi);
                data[i].pixel[y][x] = (pixel / 127.5) - 1; // [-1, +1]
            }
        }

        u8 digit;
        fread(&digit, 1, 1, fl);
        data[i].digit = digit;
    }

    const u32 end = tick();

    printf("Completed in %ums\n", end - start);

    fclose(fl);
    fclose(fi);

    *count = count1;

    return data;
}
//----------------------------------------------------------------------------------------------------------------------
void print_digit(const r32 digit[28][28])
{
    for (ui y=0;y<24;y++)
    {
        for (ui x=0;x<24;x++)
        {
            putc(digit[y+2][x+2] > 0 ? '#' : '-', stdout);
        }

        putc('\n', stdout);
    }

    putc('\n', stdout);
}
//----------------------------------------------------------------------------------------------------------------------