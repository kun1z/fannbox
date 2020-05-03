#include "common.h"
//----------------------------------------------------------------------------------------------------------------------
int main(void)
{
    fann_type * calc_out;
    fann_type input[16];

    struct fann * ann = fann_create_from_file("number.net");
    if (!ann) return 0;

    u64 errors = 0;
    const ui count = 50000;

    for (ui i=0;i<count;i++)
    {
        u16 number = rng_word();
        const ui real_result = in_range(number);

        for (ui j=0;j<16;j++)
        {
            input[j] = number & 1 ? 1 : -1;
            number >>= 1;
        }

        calc_out = fann_run(ann, input);
        const ui nn_answer = calc_out[0] > 0 ? 1 : 0;

        if (real_result != nn_answer)
        {
            errors++;
        }
    }

    printf("error rate: %.4f%%\n", (errors * 100.) / count);

    fann_destroy(ann);

    printf("All done!\n");
    fflush(0);
    sleep(-1u);

    return 0;
}
//----------------------------------------------------------------------------------------------------------------------