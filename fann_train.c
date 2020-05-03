#include "common.h"
//----------------------------------------------------------------------------------------------------------------------
int main(void)
{
    const ui num_data = 10000;
    const ui num_input = 16;
    const ui num_output = 1;
    const ui num_layers = 3;
    const ui num_neurons_hidden = 8;
    const r64 desired_error = 0.0001;
    const ui max_epochs = 500000;
    const ui epochs_between_reports = 1000;

    // Create a neural network
    struct fann * ann = fann_create_standard(num_layers, num_input, num_neurons_hidden, num_output);
    fann_set_activation_function_hidden(ann, FANN_SIGMOID_SYMMETRIC);
    fann_set_activation_function_output(ann, FANN_SIGMOID_SYMMETRIC);

    for (int x=0;x<1;x++)
    {
        printf("Starting session %u\n", x);
        // Create training data
        u32 start = tick();
        struct fann_train_data * data = fann_create_train(num_data, num_input, num_output);
        for (ui i=0;i<num_data;i++)
        {
            u16 number = rng_word();
            const ui real_result = in_range(number);

            for (ui j=0;j<16;j++)
            {
                data->input[i][j] = number & 1 ? 1 : -1;
                number >>= 1;
            }

            data->output[i][0] = real_result;
        }
        u32 end = tick();
        printf("Create training data: %ums\n", end - start);

        // Do training
        start = tick();
        fann_train_on_data(ann, data, max_epochs, epochs_between_reports, desired_error);
        end = tick();
        printf("Training data took: %ums\n", end - start);

        fann_destroy_train(data);
    }

    // Saving training data
    u32 start = tick();
    fann_save(ann, "number.net");
    fann_destroy(ann);
    u32 end = tick();
    printf("Saving data took: %ums\n", end - start);

    printf("All done!\n");
    fflush(0);
    sleep(-1u);

    return 0;
}
//----------------------------------------------------------------------------------------------------------------------