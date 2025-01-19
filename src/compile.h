#ifndef X__SUBLEQ_COMPILE_H__X
#define X__SUBLEQ_COMPILE_H__X
#include "process.h"

typedef struct {
        unsigned char *bytes;
        unsigned int length;
} SUBLEQ_BYTE_BUFFER;

void subleq_byte_buffer_create(SUBLEQ_BYTE_BUFFER *buffer);
void subleq_byte_buffer_destroy(SUBLEQ_BYTE_BUFFER *buffer);
void subleq_byte_buffer_pushb(SUBLEQ_BYTE_BUFFER *buffer, unsigned char byte);
void subleq_byte_buffer_pushw(SUBLEQ_BYTE_BUFFER *buffer, unsigned short word);
void subleq_byte_buffer_pushs(SUBLEQ_BYTE_BUFFER *buffer, const char *string);

void subleq_compile_tokens(SUBLEQ_BYTE_BUFFER *buffer, const SUBLEQ_TOKENS *tokens);

#endif
