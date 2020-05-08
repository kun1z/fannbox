#include "common.h"
//----------------------------------------------------------------------------------------------------------------------
si main(void)
{
    u64 count;
    struct digit const * const test_data = load_digits(&count, "t10k-labels.idx1-ubyte.bin", "t10k-images.idx3-ubyte.bin");

    const ui num_layers = 4;
    const ui num_input = 576; // 24 x 24
    const ui num_neurons_hidden1 = 49;
    const ui num_neurons_hidden2 = 49;
    const ui num_output = 10;

    const ui num_data = count;
    const ui max_epochs = 100000;
    const ui epochs_between_reports = 10;

    // Create a neural network
    struct fann * const ann = fann_create_standard(num_layers, num_input, num_neurons_hidden1, num_neurons_hidden2, num_output);

    fann_set_activation_function_hidden(ann, FANN_SIGMOID_SYMMETRIC);
    fann_set_activation_function_output(ann, FANN_SIGMOID_SYMMETRIC);

    fann_set_train_stop_function(ann, FANN_STOPFUNC_BIT);
    fann_set_bit_fail_limit(ann, 0.01f);

    fann_randomize_weights(ann, -1.0, 1.0);

    for (ui x=0;x<1;x++)
    {
        printf("Starting session %u\n", x);

        // Create training data
        struct fann_train_data * data = fann_create_train(num_data, num_input, num_output);

        u32 start = tick();
        for (ui i=0;i<num_data;i++)
        {
            for (ui y=2;y<24;y++)
            {
                for (ui x=2;x<24;x++)
                {
                    data->input[i][(y * 24) + x] = test_data[i].pixel[y][x]; // clip edges since they're mostly empty
                }
            }

            for (ui j=0;j<num_output;j++)
            {
                data->output[i][j] = (j == test_data[i].digit) ? 1 : -1;
            }
        }
        u32 end = tick();

        printf("Created training data took %ums\n", end - start);

        // Do training
        start = tick();
        fann_train_on_data(ann, data, max_epochs, epochs_between_reports, 0);
        end = tick();

        printf("Training data took %ums\n\n", end - start);

        fann_destroy_train(data);
    }

    // Saving training data
    u32 start = tick();
    fann_save(ann, "fannbox.net");
    u32 end = tick();

    fann_destroy(ann);

    printf("Saving data took: %ums\n", end - start);

    printf("All done!\n");
    fflush(0);
    sleep(-1u);

    return EXIT_SUCCESS;
}
//----------------------------------------------------------------------------------------------------------------------