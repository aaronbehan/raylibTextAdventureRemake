#include <stdio.h>
#include <string.h>

void returnFeedback(char *input, char *output)
{
    // char feedback[300];  // I INTEND TO POPULATE THIS ARRAY WITH TEXT TO APPEND TO OUTPUT ARRAY

    strcat(output, input);
    // if (!strcmp(input, "hello world")) strcat(pointer, "hello world!!!!");  // EXCLAMATION MARK NECESSARY DUE TO STRANGE PROPERTIES OF strcmp()
    printf("input = %s", input);
    printf("output = %s", output);
    
}   