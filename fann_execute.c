#include "common.h"
//----------------------------------------------------------------------------------------------------------------------
int main(void)
{
    printf("Starting...\n");

    fann_type * calc_out;
    fann_type input[16];

    struct fann * ann = fann_create_from_file("fannbox.net");
    if (!ann) return 0;

    u64 errors = 0;
    const ui count = 65536;

    for (ui i=0;i<count;i++)
    {
        u16 number = i;
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
            printf("%u was wrong (%.8f) real result: %d\n", i, calc_out[0], real_result ? 1 : -1);
            errors++;
        }
    }

    printf("error rate: %.8f%%\n", (errors * 100.) / count);

    fann_destroy(ann);

    printf("All done!\n");
    fflush(0);
    sleep(-1u);

    return 0;
}
//----------------------------------------------------------------------------------------------------------------------