#include "common.h"
//----------------------------------------------------------------------------------------------------------------------
int main(void)
{
    printf("Starting...\n");

    struct fann * ann = fann_create_from_file("fannbox.net");
    if (!ann) return 0;

    ui count;
    struct digit const * const test_data = load_digits(&count, "train-labels.idx1-ubyte.bin", "train-images.idx3-ubyte.bin");

    ui error_count = 0;

    for (ui i=0;i<count;i++)
    {
        fann_type input[576];

        for (ui y=0;y<24;y++)
        {
            for (ui x=0;x<24;x++)
            {
                input[(y * 24) + x] = test_data[i].pixel[y+2][x+2];
            }
        }

        fann_type * const calc_out = fann_run(ann, input);

        r32 max = -1;
        ui nn_digit;

        for (ui j=0;j<10;j++)
        {
            if (calc_out[j] > max)
            {
                max = calc_out[j];
                nn_digit = j;
            }
        }

        if (test_data[i].digit != nn_digit)
        {
            /*printf("Error: %u failed! Supposed to be %u, was %u\n", i, test_data[i].digit, nn_digit);

            for (ui j=0;j<10;j++)
            {
                printf("%u: %+.4f\n", j, calc_out[j]);
            }
            printf("\n");

            print_digit(test_data[i].pixel);

            sleep(10);*/

            error_count++;
        }
    }

    fann_destroy(ann);

    printf("count: %u\n", count);
    printf("error_count: %u\n", error_count);
    printf("Final Error: %.4f%%\n", (error_count * 100.) / count);

    printf("All done!\n");
    fflush(0);
    sleep(-1u);

    return 0;
}
//----------------------------------------------------------------------------------------------------------------------