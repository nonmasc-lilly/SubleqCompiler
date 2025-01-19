#include <stdio.h>
#include <stdlib.h>
#include "process.h"
#include "compile.h"

void help_menu(const char *argv0, int err_code) {
        fprintf(stderr, "\033[1;33mUsage:\033[m %s <infile> <ofile> [options]\n", argv0);
        fprintf(stderr, "Options:\n"
                        "\t-d :: Allow for debug text.\n"
                        "\t-h :: Print this menu and quit.\n"
        );
        exit(err_code);
}

int main(int argc, char **argv) {
        int i, status;
        char debug = 0;
        SUBLEQ_TOKENS tokens;
        SUBLEQ_BYTE_BUFFER buffer;
        struct {
                FILE *pointer;
                char *content;
                int   length;
        } file;
        if(argc < 3) help_menu(argv[0], 1);
        for(i = 3; i < argc; i++) {
                if(!strcmp(argv[i], "-d")) debug = 1;
                if(!strcmp(argv[i], "-h")) help_menu(argv[0], 0);
        }
        file.pointer = fopen(argv[1], "r");
        if(!file.pointer) {
                fprintf(stderr, "\033[1;31mError:\033[m Could not open `%s` for reading.\n", argv[1]);
                help_menu(argv[0], 1);
        }
        fseek(file.pointer, 0L, SEEK_END);
        file.length  = ftell(file.pointer);
        file.content = calloc(1, file.length+1);
        fseek(file.pointer, 0L, SEEK_SET);
        fread(file.content, 1, file.length, file.pointer);
        fclose(file.pointer);

        if(debug)
                fprintf(stderr, "\033[1;90mDebug (file `%s` read):\033[0;90m\n%s\n\033[m", argv[1], file.content);

        subleq_tokens_create(&tokens);
        status = subleq_process_string(&tokens, file.content);
        if(!status) {
                fprintf(stderr, "Exitted with error during string processing.\n");
                subleq_tokens_destroy(&tokens);
                free(file.content);
                exit(1);
        }
        if(debug) {
                fprintf(stderr, "\033[1;90mDebug (program string processed):\033[0;90m\n");
                subleq_tokens_print(stderr, &tokens);
                fprintf(stderr, "\n\033[m");
        }

        subleq_byte_buffer_create(&buffer);
        subleq_compile_tokens(&buffer, &tokens);
        if(debug) {
                subleq_byte_buffer_pushb(&buffer, 0x00);
                fprintf(stderr, "\033[1;90mDebug (program compiled):\033[0;90m\n%s\n\033[m", (const char*)buffer.bytes);
                buffer.bytes = realloc(buffer.bytes, --buffer.length);
        }

        file.pointer = fopen(argv[2], "w");
        if(!file.pointer) {
                subleq_byte_buffer_destroy(&buffer);
                subleq_tokens_destroy(&tokens);
                free(file.content);
                fprintf(stderr, "\033[1;31mError:\033[m Could not open `%s` for writing.\n", argv[2]);
                help_menu(argv[0], 1);
        }
        fwrite(buffer.bytes, 1, buffer.length, file.pointer);

        subleq_byte_buffer_destroy(&buffer);
        subleq_tokens_destroy(&tokens);
        free(file.content);
        exit(0);
}
