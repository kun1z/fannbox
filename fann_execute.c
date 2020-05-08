#include "common.h"
//----------------------------------------------------------------------------------------------------------------------
int main(void)
{
    printf("Starting...\n");

    u64 count;
    struct digit const * const test_data = load_digits(&count, "train-labels.idx1-ubyte.bin", "train-images.idx3-ubyte.bin");

    fann_type input[784], *calc_out;

    struct fann * ann = fann_create_from_file("fannbox.net");
    if (!ann) return 0;

    for (ui i=0;i<count;i++)
    {
        for (ui y=0;y<28;y++)
        {
            for (ui x=0;x<28;x++)
            {
                input[(y * 28) + x] = test_data[i].pixel[y][x];
            }
        }

        calc_out = fann_run(ann, input);

        ui activated = 0;

        for (ui j=0;j<10;j++)
        {
            if (calc_out[j] > 0)
            {
                activated++;
            }
        }

        if (activated > 1)
        {
            printf("More than 1 neuron activated!\n");
        }

        if (calc_out[test_data[i].digit] <= 0)
        {
            printf("Error: %u failed! Supposed to be %u\n", i, test_data[i].digit);

            for (ui j=0;j<10;j++)
            {
                printf("%u: %+.2f\n", j, calc_out[j]);
            }

            printf("\n");

            print_digit(test_data[i].pixel);
        }
    }

    fann_destroy(ann);

    printf("All done!\n");
    fflush(0);
    sleep(-1u);

    return 0;
}
//----------------------------------------------------------------------------------------------------------------------