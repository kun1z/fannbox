#include "common.h"
//----------------------------------------------------------------------------------------------------------------------
struct digit
{
    r64 pixel[28][28];
    u8 digit;
};
struct digit * load_digits(u64 * const, s8 const * const, s8 const * const);
//----------------------------------------------------------------------------------------------------------------------
si main(void)
{
    u64 count;
    struct digit const * const test_data = load_digits(&count, "train-labels.idx1-ubyte.bin", "train-images.idx3-ubyte.bin");

    const ui num_layers = 4;
    const ui num_input = 784;
    const ui num_neurons_hidden1 = 15;
    const ui num_neurons_hidden2 = 15;
    const ui num_output = 10;

    const ui num_data = count;
    const ui max_epochs = 100000;
    const ui epochs_between_reports = 1;
    const r64 desired_error = 0.0001;

    // Create a neural network
    struct fann * const ann = fann_create_standard(num_layers, num_input, num_neurons_hidden1, num_neurons_hidden2, num_output);

	fann_set_activation_function_hidden(ann, FANN_SIGMOID_SYMMETRIC);
	fann_set_activation_function_output(ann, FANN_SIGMOID_SYMMETRIC);

	fann_randomize_weights(ann, -1.0, 1.0);

    for (ui x=0;x<1;x++)
    {
        printf("Starting session %u\n", x);

        // Create training data
        struct fann_train_data * data = fann_create_train(num_data, num_input, num_output);

        u32 start = tick();
        for (ui i=0;i<num_data;i++)
        {
            for (ui y=0;y<28;y++)
            {
                for (ui x=0;x<28;x++)
                {
                    data->input[i][(y * 28) + x] = test_data[i].pixel[y][x];
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
        fann_train_on_data(ann, data, max_epochs, epochs_between_reports, desired_error);
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
struct digit * load_digits(u64 * const count, s8 const * const filename_labels, s8 const * const filename_images)
{
    FILE * const fl = fopen(filename_labels, "rb");
    if (!fl)
    {
        printf("file %s not found!\n", filename_labels);
        exit(EXIT_FAILURE);
    }

    u32 magic;
    fread(&magic, 1, 4, fl);
    magic = bswap_32(magic);
    if (magic != 2049)
    {
        printf("file %s is invalid!\n", filename_labels);
        exit(EXIT_FAILURE);
    }

    FILE * const fi = fopen(filename_images, "rb");
    if (!fi)
    {
        printf("file %s not found!\n", filename_images);
        exit(EXIT_FAILURE);
    }

    fread(&magic, 1, 4, fi);
    magic = bswap_32(magic);
    if (magic != 2051)
    {
        printf("file %s is invalid!\n", filename_images);
        exit(EXIT_FAILURE);
    }

    u32 count1, count2;
    fread(&count1, 1, 4, fl);
    fread(&count2, 1, 4, fi);
    count1 = bswap_32(count1);
    count2 = bswap_32(count2);
    if (count1 != count2 || !count1 || !count2)
    {
        printf("files %s and %s contain invalid counts!\n", filename_labels, filename_images);
        exit(EXIT_FAILURE);
    }

    u32 row, col;
    fread(&row, 1, 4, fi);
    fread(&col, 1, 4, fi);
    row = bswap_32(row);
    col = bswap_32(col);
    if (row != col || row != 28)
    {
        printf("files %s and %s contain invalid rows and columns!\n", filename_labels, filename_images);
        exit(EXIT_FAILURE);
    }

    *count = count1;

    struct digit * const data = malloc(sizeof(struct digit) * *count);
    if (!data)
    {
        printf("malloc() failed!\n");
        exit(EXIT_FAILURE);
    }

    printf("Loading %lu samples from %s and %s...\n", *count, filename_labels, filename_images);

    const u32 start = tick();
    for (u64 i=0;i<*count;i++)
    {
        for (ui y=0;y<28;y++)
        {
            for (ui x=0;x<28;x++)
            {
                u8 pixel;
                fread(&pixel, 1, 1, fi);
                const r64 sigmoid = pixel / 255.;
                data[i].pixel[y][x] = (sigmoid + sigmoid) - 1; // [-1, +1]
            }
        }

        fread(&data[i].digit, 1, 1, fl);
    }
    const u32 end = tick();

    printf("Completed in %ums\n", end - start);

    return data;
}
//----------------------------------------------------------------------------------------------------------------------