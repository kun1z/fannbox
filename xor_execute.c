#include <stdio.h>
#include <unistd.h>
#include "doublefann.h"
//----------------------------------------------------------------------------------------------------------------------
int main(void)
{
    fann_type * calc_out;
    fann_type input[2];

    struct fann * ann = fann_create_from_file("xor.net");
    if (!ann) return 0;

    input[0] = -1;
    input[1] = 1;
    calc_out = fann_run(ann, input);

    printf("xor test (%f,%f) -> %f\n", input[0], input[1], calc_out[0]);

    srand(1234);

    for (int i=0;i<5000000;i++)
    {
        int x = rand() & 1;
        int y = rand() & 1;
        int answer = x ^ y;
        input[0] = x ? 1 : -1;
        input[1] = y ? 1 : -1;

        calc_out = fann_run(ann, input);
        int nn_answer = *calc_out > 0 ? 1 : 0;

        if (answer != nn_answer)
        {
            printf("i: %d -- x: %d, y: %d, a: %d, nn: %d\n", i, x, y, answer, nn_answer);
        }
    }

    fann_destroy(ann);

    printf("All done!\n");
    fflush(0);
    sleep(-1u);

    return 0;
}
//----------------------------------------------------------------------------------------------------------------------