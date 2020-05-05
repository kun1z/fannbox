#include "common.h"
//----------------------------------------------------------------------------------------------------------------------
int main(void)
{
    printf("Starting...\n");

    fann_type * calc_out;
    fann_type input[10];

    struct fann * ann = fann_create_from_file("fannbox.net");
    if (!ann) return 0;

    u64 errors = 0;
    const ui count = 10000;

    for (ui i=0;i<count;i++)
    {
        for (ui j=0;j<10;j++)
        {
            input[j] = -1;
        }

        calc_out = fann_run(ann, input);

        if (calc_out[0] > 0)
        {
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