#ifndef X__SUBLEQ_PROCESS_H__X
#define X__SUBLEQ_PROCESS_H__X
#include <stdio.h>

typedef unsigned int SUBLEQ_TOKEN;
typedef struct {
        SUBLEQ_TOKEN *values;
        unsigned int length;
} SUBLEQ_TOKENS;

void subleq_tokens_create(SUBLEQ_TOKENS *tokens);
void subleq_tokens_destroy(SUBLEQ_TOKENS *tokens);
void subleq_tokens_append(SUBLEQ_TOKENS *tokens, SUBLEQ_TOKEN value);
void subleq_tokens_print(FILE *out, SUBLEQ_TOKENS *tokens);

int subleq_process_string(SUBLEQ_TOKENS *tokens, const char *source);

#endif
