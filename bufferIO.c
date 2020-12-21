#include "bufferIO.h"

void load_buffer(char *text[MAX_LINE_NO], int *act_line, int *act_char,
        int argc, char **argv)
{
    int i = 0;

    /* load file to buffer */
    FILE *fp;

    /* verify args - if none specified, create/edit saved_buffer.txt */
    char file[MAX_FN_SIZE] = "saved_buffer.txt";
    if (argc > 1) {
        strcpy(file, argv[1]);
    } else {
        fp = fopen(file, "w+");
        if (fp == NULL) {
            printf("Failed to open buffer\n");
            exit(1);
        }
        text[i] = calloc (MAX_LINE_LEN, 1);
        return;
    }

    fp = fopen(file, "r+");
    char buffer[MAX_LINE_LEN];
    if (fp == NULL) {
        /* if file doesn't exist, create it */
        fp = fopen(file, "w+");
        if (fp == NULL) {
            printf("Failed to open buffer\n");
            exit(1);
        }
        text[i] = calloc (MAX_LINE_LEN, 1);
        return;
    }

    /* populate lines */
    while((fgets (buffer, MAX_LINE_LEN, fp))!= NULL) {
        text[i] = calloc (MAX_LINE_LEN, 1);
        for (*act_char = 0; *act_char < MAX_LINE_LEN; (*act_char)++)
            if (buffer[*act_char] == '\n') {
                buffer[*act_char] = '\0';
                break;
            } else if (buffer[*act_char] == '\0') {
                break;
            }
        (*act_line)++;
        *act_char = 0;
        strcpy(text[i], buffer);
        i++;
    }

    /* allocate at least one line if the file is empty */
    text[i] = i == 0 ? calloc (MAX_LINE_LEN, 1) : text[i];

    /* move cursor to the correct position */
    *act_line = *act_line > 0 ? *act_line - 1 : 0;
    for (i = 0; i < MAX_LINE_LEN; i++)
        if (text[*act_line][i] == '\0')
            break;
    *act_char = i;

    fclose(fp);
}

void save_buffer(char *text[MAX_LINE_NO], int argc, char **argv)
{
    int i;

    /* verify args */
    char file[MAX_FN_SIZE] = "saved_buffer.txt";
    if (argc > 1)
        strcpy(file, argv[1]);

    /* save buffer to file */
    FILE *fp = fopen(file, "w+");
    if (fp == NULL) {
        printf("Failed to open buffer\n");
        free_text(text);
        exit(1);
    }
    for (i = 0; i < MAX_LINE_NO; i++)
        if (text[i] != NULL) {
            strcat(text[i], "\n");
            fprintf(fp, text[i]);
        } else {
            break;
        }
    fclose(fp);
    free_text(text);
}
