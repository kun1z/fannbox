#include "common.h"
//----------------------------------------------------------------------------------------------------------------------
si main(void)
{
    fann_disable_seed_rand();

    const ui threads = 4;

    ui count;
    struct digit const * const test_data = load_digits(&count, "t10k-labels.idx1-ubyte.bin", "t10k-images.idx3-ubyte.bin");

    const ui num_layers = 5;
    const ui num_input = 576; // 24 x 24 pixels
    const ui num_neurons_hidden1 = 144;
    const ui num_neurons_hidden2 = 144;
    const ui num_neurons_hidden3 = 144;
    const ui num_output = 10;

    const ui num_data = count;
    const ui max_epochs = 100000;
    const ui epochs_between_reports = 10;
    const r64 desired_error = 0.0001;

    // Create a neural network
    struct fann * const ann = fann_create_standard(num_layers, num_input, num_neurons_hidden1, num_neurons_hidden2, num_neurons_hidden3, num_output);

    fann_set_activation_function_hidden(ann, FANN_SIGMOID_SYMMETRIC);
    fann_set_activation_function_output(ann, FANN_SIGMOID_SYMMETRIC);

    // Create training data
    struct fann_train_data * data = fann_create_train(num_data, num_input, num_output);

    u64 start = tick();
    for (ui i=0;i<num_data;i++)
    {
        for (ui y=0;y<24;y++)
        {
            for (ui x=0;x<24;x++)
            {
                data->input[i][(y * 24) + x] = test_data[i].pixel[y+2][x+2]; // clip edges since they're mostly empty
            }
        }

        for (ui j=0;j<num_output;j++)
        {
            data->output[i][j] = (j == test_data[i].digit) ? 1 : -1;
        }
    }
    u64 end = tick();

    printf("Created training data took %lums\n", end - start);

    // Do training
    start = tick();

    printf("Max epochs %8d. Desired error: %.10f.\n", max_epochs, desired_error);

    for (ui i=1;i<=max_epochs;i++)
    {
        const r64 error = threads > 1 ? fann_train_epoch_irpropm_parallel(ann, data, threads) : fann_train_epoch(ann, data);

        if (!(i % epochs_between_reports) || i == max_epochs || i == 1)
        {
            printf("Epochs     %8d. Current error: %.10f.\n", i, error);
        }

        if (error < desired_error)
        {
            printf("Epochs     %8d. Current error: %.10f.\n", i, error);
            break;
        }
    }

    end = tick();

    printf("Training data took %.2f secs\n\n", (end - start) / 1000.);

    fann_destroy_train(data);

    // Saving training data
    start = tick();
    fann_save(ann, "fannbox.net");
    end = tick();

    fann_destroy(ann);

    printf("Saving data took: %lums\n", end - start);

    printf("All done!\n");
    fflush(0);
    sleep(-1u);

    return EXIT_SUCCESS;
}
//----------------------------------------------------------------------------------------------------------------------