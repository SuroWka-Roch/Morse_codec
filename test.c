#include <math.h>
#include "morse.h"
#include <stdio.h>
#include <stdlib.h>

int main(){
    char* test_string = "abcdefghijklmnoprstuwxyz";
    char* buffer = malloc(200 * sizeof(char));
    char* buffer2 = malloc(200 * sizeof(char));

    morse_tree* root = morse_init();
    morse_encode(root, test_string, buffer);
    printf("%s\n", buffer);
    morse_decode(root, buffer, buffer2);
    printf("%s\n",buffer2);
    morse_drop(root);
}