/*
 * Copyright (C) 2017 Sidhin S Thomas. All rights reserved.
 */

#include "stmp_utility.h"

#ifndef LOG_OUTPUT
#define LOG_OUTPUT stdout
#endif

#ifndef DEBUG_LOG_OUTPUT
#define DEBUG_LOG_OUTPUT stdout
#endif

char const FILE_ERROR_MESSAGE[] = "Unable to open the file.\n"
        "Please check the filename and ensure read permission.\n";


int get_all_words(char *const source, char buffer[][WORD_SIZE], int buffer_size){
    int j = 0;

    buffer[0][0] = '\0';
    int start;
    for (start = 0; source[start] == ' ' ; ++start);
    int k = 0;
    int comment_flag = 0;
    for (int i = start; source[i] != '\0' ; ++i) {

        if (j > buffer_size){
            fprintf(stderr, "Buffer overflow");
            return -1;
        }

        if(source[i]=='#'||source[i]==';'){
            /* skip over line comments */
            comment_flag = 1;
            j--;
            break;
        }
        /* word finished, switching to next placeholder */
        if(source[i] == ' '){
            buffer[j][k] = '\0';

            while(source[++i]==' ');
            if(source[i] == '\0')
                break;
            i--;

            j++;
            buffer[j][0] = '\0';
            k = 0;

        }
        else if (source[i]==','){
            buffer[j][k] = '\0';

            while(source[++i]==' ');
            if(source[i] == '\0')
                break;
            i--;

            j++;
            buffer[j][0] = ',';
            buffer[j][1] = '\0';
            j++;
            buffer[j][0] = '\0';
            k = 0;
        }
        else {
            buffer[j][k] = source[i];
            k++;
        }
    }
    if(comment_flag==0)
        buffer[j][k]='\0';
    return j+1;
}

void print_string_array(void *lines, const int size, const int line_size) {
    char (*src)[line_size] = lines;
    printf("Number of words: %d\nList of words:\n", size);
    for (int i = 0; i < size; ++i) {
        printf("%d) %s\n",i,src[i]);
    }
}

int get_all_lines(char *const path, char buffer[][LINE_SIZE], const int buffer_size){

    FILE *fp = fopen(path,"r");
    if (fp == NULL){
        perror("stmp utility.\n");
        fprintf(stderr, FILE_ERROR_MESSAGE);
        return -1;
    }

    char *string_buffer = (char *) malloc(sizeof(char)*LINE_SIZE);
    if (string_buffer == NULL){
        perror("stmp utility.\n");
        return -1;
    }

    int buff_index = 0;
    while (fgets(string_buffer,LINE_SIZE,fp)!= NULL){

#ifdef DEBUG
        fprintf(DEBUG_LOG_OUTPUT,"Retrieving line: %s",string_buffer);
#endif
        for (int i = 0; string_buffer[i]!='\0'; ++i) {
            if (string_buffer[i] == '\n'){
                string_buffer[i] = '\0';
            }
        }
        strcpy(buffer[buff_index], string_buffer);
        buff_index++;
        if (buff_index >= buffer_size){
            fprintf(stderr,"Buffer overflow.\n");
            return -1;
        }
    }
    fprintf(DEBUG_LOG_OUTPUT,"\nCount: %d\n",buff_index);
    return buff_index ;
}

int startsWith(const char *str, const char *pre)
{
    size_t lenpre = strlen(pre), lenstr = strlen(str);
    return lenstr < lenpre ? 0 : strncmp(pre, str, lenpre) == 0;
}
