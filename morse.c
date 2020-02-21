#include "morse.h"
#include <stdlib.h>
#include <string.h>


//local functions
static void fill_array(char* array, char* code, int len);
static void make_lvl(morse_tree* root, int lvl, char* code);
static int location_in_table(morse_tree_level* lvl,char c);
static void encode_char(int lvl,int location, char** buffer);

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
    int slice_len = lvl*2;
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

