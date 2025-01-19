#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process.h"

void subleq_tokens_create(SUBLEQ_TOKENS *tokens) {
        tokens->values = malloc(1);
        tokens->length = 0;
}
void subleq_tokens_destroy(SUBLEQ_TOKENS *tokens) {
        free(tokens->values);
        memset(tokens, 0, sizeof(*tokens));
}
void subleq_tokens_append(SUBLEQ_TOKENS *tokens, SUBLEQ_TOKEN value) {
        tokens->length                    += 1;
        tokens->values                     = realloc(tokens->values, tokens->length * sizeof(*tokens->values));
        tokens->values[tokens->length - 1] = value;
}
void subleq_tokens_print(FILE *out, SUBLEQ_TOKENS *tokens) {
        unsigned i;
        fprintf(out, "::-$...A--$...B--$...C-::\n");
        for(i = 0; i < tokens->length; i += 3) {
                fprintf(out, ":: $%04X  $%04X  $%04X ::\n", tokens->values[i], tokens->values[i+1], tokens->values[i+2]);
        }
        fprintf(out, "::=====================::\n");
}

#define STR_PUSH(str, c) do { str = realloc(str, strlen(str)+2); str[strlen(str)+1] = 0; str[strlen(str)] = c; } while(0)
#define STR_ERASE(str) *(str = realloc(str, 1)) = 0
int subleq_process_string(SUBLEQ_TOKENS *tokens, const char *source) {
        char *buffer, *eptr;
        long token, line;
        int i;
        line = 0;
        buffer = calloc(1,1);
        for(i = 0; source[i]; i++) {
                line += source[i] == '\n';
                if(!isspace(source[i]) && source[i] != '#') { STR_PUSH(buffer, source[i]); continue; }
                if(source[i] == '#') {
                        while(source[i] && source[i] != '\n') i++;
                        line++;
                }
                if(!*buffer) continue;
                token = strtol(buffer + (*buffer == '$'), &eptr, *buffer == '$' ? 0x10 : 0x0A);
                if(*eptr) {
                        fprintf(stderr, "\033[1;31mError:\033[m Expected token on line %d, got `%s`.\n", line, buffer);
                        return 0;
                }
                subleq_tokens_append(tokens, token);
                STR_ERASE(buffer);
        }
        if(tokens->length && tokens->length % 3) {
                fprintf(stderr, "\033[1;31mError:\033[m Expected token multiple of 3.\n", line);
                return 0;
        }
        return 1;
}
