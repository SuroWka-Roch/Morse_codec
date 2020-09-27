#include "morse.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


//local functions
static void fill_array(char* array, char* code, int len);
static void make_lvl(morse_tree* root, int lvl, char* code);
static int location_in_table(morse_tree_level* lvl,char c);
static void encode_char(int lvl,int location, char** buffer);
static char decode_morse_char(morse_tree* root, char* token);
static void decode_word(morse_tree* root, char* to_decode, char** buffer, char* stop_at);


//Work around for dependency on math.h lib 
#ifndef _MATH_H_

double pow(double base, double exponent){
    int result=1;
    for (; exponent>0; exponent--)
    {
        result = result * base;
    }
    return result;
}

#endif

morse_tree* morse_init(){
    morse_tree* root = (morse_tree*) malloc(sizeof(morse_tree));

    make_lvl(root,1,"et");
    make_lvl(root,2,"ianm");
    make_lvl(root,3,"surwdkgo");
    make_lvl(root,4,"hvf*l*pjbxcyzq**");

    return root;
}

void morse_drop(morse_tree* root){
    for(int i=0; i < TREE_DEPTH; i++){
        char* temp_pointer = root->level[i].array;
        if (temp_pointer){
            free(temp_pointer);
        }
    }
}

void morse_encode(morse_tree* root,char* to_encode,char* buffer){
    for(int i =0; i<strlen(to_encode);i++){
        char cur_char = to_encode[i];

        if(cur_char == ' '){
            *buffer = SPACE_SYMBOL;
            buffer++;
            *buffer = SPACE_SYMBOL;
            buffer++;
            continue;
        }
        
        for(int j=0;j<TREE_DEPTH;j++){
            int location = location_in_table(&(root->level[j]), cur_char);
            if(location!=-1){
                encode_char(root->level[j].level, location, &buffer);
                break;
            }
        }
    }
    *buffer = '\0';
}

static void fill_array(char* array, char* code,int len){
    for(int i =0;i<len;i++){
        array[i] = code[i];
    }
}

static void make_lvl(morse_tree* root,int lvl, char* code ){
    morse_tree_level lvl_struct;
    lvl_struct.level = lvl;
    lvl_struct.array = (char*)malloc(sizeof(char)* MORSE_AR_LEN(lvl_struct.level));
    fill_array(lvl_struct.array,
                code,
                MORSE_AR_LEN(lvl_struct.level) );
    root->level[lvl-1] = lvl_struct;
}

static int location_in_table(morse_tree_level* lvl,char c){
    for(int i =0; i<MORSE_AR_LEN(lvl->level);i++){
        if(lvl->array[i] == c){
            return i;
        }
    }
    return -1;
}

static void encode_char(int lvl,int location, char** buffer){
    int slice_len = MORSE_AR_LEN(lvl)-1;
    for(int i=0;i<lvl;i++){
        slice_len=slice_len/2;
        if(location < slice_len){
            **buffer = SHORT_SYMBOL;
        }
        else{
            **buffer = LONG_SYMBOL;
            location = location - slice_len;
        }
        (*buffer)++;
    }
    **buffer = SPACE_SYMBOL;
    (*buffer)++;
}

void morse_decode(morse_tree* root, char* to_decode, char* buffer){
    char* buffer_beginning = buffer;
    char *to_decode_pnt = to_decode;
    char *to_decode_pnt_dragging = to_decode;
    to_decode_pnt = strstr(to_decode_pnt, WORD_SPACE_STR);

    for(;to_decode_pnt != NULL; ){
        decode_word(root, to_decode_pnt_dragging, &buffer, to_decode_pnt);
        *buffer = SPACE_SYMBOL;
        buffer++;
        printf("%c\n",*to_decode_pnt);
        to_decode_pnt += strlen(WORD_SPACE_STR);
        to_decode_pnt_dragging = to_decode_pnt;
        to_decode_pnt = strstr(to_decode_pnt, WORD_SPACE_STR);

    }

    decode_word(root, to_decode_pnt_dragging, &buffer, NULL);

    *buffer = '\0';
    buffer = buffer_beginning;

}

static void decode_word(morse_tree* root, char* to_decode, char** buffer, char* stop_at){
    const char seperator[2] = {SPACE_SYMBOL, '\0'};
    char* tok_r_pt;
    char *token = strtok_r(to_decode, seperator, &tok_r_pt);
    while( token != NULL) {
        **buffer = decode_morse_char(root, token);
        (*buffer)++;
        
        if(!(stop_at==NULL || tok_r_pt<=stop_at))
            break;

        token = strtok_r(NULL, seperator, &tok_r_pt);
    }
}

static char decode_morse_char(morse_tree* root, char* token){
    int lvl = strlen(token)-1;
    int place_in_tree = 0;
    int lvl_len = MORSE_AR_LEN( strlen(token)) - 1;
    for(int i = 0; i<=lvl;i++){
        if(token[i] == LONG_SYMBOL){
            place_in_tree += lvl_len/pow(2,i+1);
        }
    }
    return root->level[lvl].array[place_in_tree];
}

