#include <stdio.h>
#include <unistd.h>
#include "doublefann.h"
//----------------------------------------------------------------------------------------------------------------------
int main(void)
{
    const unsigned int num_data = 4;
    const unsigned int num_input = 2;
    const unsigned int num_output = 1;
    const unsigned int num_layers = 3;
    const unsigned int num_neurons_hidden = 3;
    const double desired_error = 0.0001;
    const unsigned int max_epochs = 500000;
    const unsigned int epochs_between_reports = 1000;

    struct fann * ann = fann_create_standard(num_layers, num_input, num_neurons_hidden, num_output);

    fann_set_activation_function_hidden(ann, FANN_SIGMOID_SYMMETRIC);
    fann_set_activation_function_output(ann, FANN_SIGMOID_SYMMETRIC);

    struct fann_train_data * data = fann_create_train(num_data, num_input, num_output);

    data->input[0][0] = -1;
    data->input[0][1] = -1;
    data->output[0][0] = -1;

    data->input[1][0] = -1;
    data->input[1][1] = 1;
    data->output[1][0] = 1;

    data->input[2][0] = 1;
    data->input[2][1] = -1;
    data->output[2][0] = 1;

    data->input[3][0] = 1;
    data->input[3][1] = 1;
    data->output[3][0] = -1;

    printf("Train 1\n");
    fann_train_on_data(ann, data, max_epochs, epochs_between_reports, desired_error);

    data->input[2][0] = -1;
    data->input[2][1] = -1;
    data->output[2][0] = -1;

    data->input[3][0] = -1;
    data->input[3][1] = 1;
    data->output[3][0] = 1;

    data->input[1][0] = 1;
    data->input[1][1] = -1;
    data->output[1][0] = 1;

    data->input[0][0] = 1;
    data->input[0][1] = 1;
    data->output[0][0] = -1;

    printf("Train 2\n");
    fann_train_on_data(ann, data, max_epochs, epochs_between_reports, desired_error / 1000);

	fann_destroy_train(data);

    fann_save(ann, "xor.net");

    fann_destroy(ann);

    printf("All done!\n");
    fflush(0);
    sleep(-1u);

    return 0;
}
//----------------------------------------------------------------------------------------------------------------------