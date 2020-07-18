/* @file morse.h
 * @brief Header file for codec to and from morse code.  
*/

#ifndef MORSE
#define MORSE

#define TREE_DEPTH 4

//that's a mess you need math.h to run this 
#include <math.h>
#define MORSE_AR_LEN(lvl) ((int)(pow(2,(lvl))+1)) //Counting starts form 1

#define LONG_SYMBOL '-'
#define SHORT_SYMBOL '.'
#define SPACE_SYMBOL ' '

typedef struct{
    char* array;
    int level; // @param row in morse tree. Array length is 2^level  + 1. 
}morse_tree_level;

typedef struct
{
    morse_tree_level level[TREE_DEPTH]; 
} morse_tree;


/*
 * @brief Create structure needed for the codec.
 */
morse_tree* morse_init();
void morse_drop(morse_tree* root);

/*
 *  @brief encode and decode strings to and from morse code 
 * 
 *  @param morse_tree structure codec will be refered to.
 *  @param to_* string to refer to when chenging 
 *  @param buffer memory space to write output to 
 * 
 */
void morse_encode(morse_tree* root, char* to_encode, char* buffer);
void morse_decode(morse_tree* root, char* to_decode, char* buffer);

#endif
