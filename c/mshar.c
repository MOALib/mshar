#include "src/mshar.h"
#include <stdio.h>
#include <stddef.h>
#include <string.h>

int main(int argc, char* argv[]){
    char* pre_script = NULL;
    char* post_script = NULL;
    char** files = NULL;
    FILE* f = NULL;
    char* script = NULL;

    /*
        usage: mshar [pre execution script] [post execution script] file1 file2 file3 file4 file5 file6 file7 file8 file9 ... > archive
        the [pre execution script] and the [post execution script] can be replaced with - for no script
     */

    if(argc < 3){
        fprintf(stderr, "usage: %s [pre execution script] [post execution script] file1 file2 file3 file4 file5 file6 file7 file8 file9 ... > archive\n", argv[0]);
        fprintf(stderr, "Put - for [pre execution script] and [post execution script] to not use a script\n");
        return 1;
    }

    pre_script = argv[1];
    post_script = argv[2];

    if(strcmp(pre_script, "-") == 0){
        pre_script = NULL;
    }
    else{
        size_t fsize;
        f = fopen(pre_script, "rb");
        {
            while(fgetc(f) != EOF){;}
            fsize = ftell(f);
            fseek(f, 0, SEEK_SET);
        }
        pre_script = malloc(fsize + 1);
        fread(pre_script, fsize, 1, f);
        pre_script[fsize] = '\0';
        fclose(f);
    }

    if(strcmp(post_script, "-") == 0){
        post_script = NULL;
    }
    else{
        size_t fsize;
        f = fopen(post_script, "rb");
        {
            while(fgetc(f) != EOF){;}
            fsize = ftell(f);
            fseek(f, 0, SEEK_SET);
        }
        post_script = malloc(fsize + 1);
        fread(post_script, fsize, 1, f);
        post_script[fsize] = '\0';
        fclose(f);
    }

    {
        int i;
        files = malloc(sizeof(char*) * (argc - 3));
        for(i = 3; i < argc; i++){
            files[i - 3] = argv[i];
        }
    }

    script = mkmshar(pre_script, post_script, files, argc - 3, 1);
    if(script == NULL){
        fprintf(stderr, "Error creating script\n");
        free(script);
        return 1;
    }
    else{
        fprintf(stdout, "%s", script);
        free(script);
    }

    return 0;
}
