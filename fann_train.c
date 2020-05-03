#include "common.h"
//----------------------------------------------------------------------------------------------------------------------
si main(void)
{
    const ui num_layers = 4;
    const ui num_input = 16;
    const ui num_neurons_hidden1 = 16;
    const ui num_neurons_hidden2 = 16;
    const ui num_output = 1;

    const ui num_data = 25000;
    const ui max_epochs = 100000;
    const ui epochs_between_reports = 1000;

    // Create a neural network
    struct fann * const ann = fann_create_standard(num_layers, num_input, num_neurons_hidden1, num_neurons_hidden2, num_output);

    fann_set_activation_steepness_hidden(ann, 1.0);
	fann_set_activation_steepness_output(ann, 1.0);

	fann_set_activation_function_hidden(ann, FANN_SIGMOID_SYMMETRIC);
	fann_set_activation_function_output(ann, FANN_SIGMOID_SYMMETRIC);

	fann_set_train_stop_function(ann, FANN_STOPFUNC_BIT);
	fann_set_bit_fail_limit(ann, 0.01);

	fann_set_training_algorithm(ann, FANN_TRAIN_RPROP);

	fann_randomize_weights(ann, -1.0, 1.0);

    for (ui x=0;x<100;x++)
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

            data->output[i][0] = real_result ? 1 : -1;
        }
        
        

        u32 end = tick();
        printf("Create training data: %ums\n", end - start);

        // Do training
        start = tick();
        fann_train_on_data(ann, data, max_epochs, epochs_between_reports, 0);
        end = tick();
        printf("Training data took: %ums\n\n", end - start);

        fann_destroy_train(data);
    }

    // Saving training data
    u32 start = tick();
    fann_save(ann, "fannbox.net");
    fann_destroy(ann);
    u32 end = tick();
    printf("Saving data took: %ums\n", end - start);

    printf("All done!\n");
    fflush(0);
    sleep(-1u);

    return 0;
}
//----------------------------------------------------------------------------------------------------------------------