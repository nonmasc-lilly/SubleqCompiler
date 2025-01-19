#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process.h"
#include "compile.h"

void subleq_byte_buffer_create(SUBLEQ_BYTE_BUFFER *buffer) {
        buffer->bytes = malloc(1);
        buffer->length = 0;
}
void subleq_byte_buffer_destroy(SUBLEQ_BYTE_BUFFER *buffer) {
        free(buffer->bytes);
        memset(buffer, 0, sizeof(*buffer));
}
void subleq_byte_buffer_pushb(SUBLEQ_BYTE_BUFFER *buffer, unsigned char byte) {
        buffer->bytes = realloc(buffer->bytes, ++buffer->length);
        buffer->bytes[buffer->length - 1] = byte;
}
void subleq_byte_buffer_pushw(SUBLEQ_BYTE_BUFFER *buffer, unsigned short word) {
        subleq_byte_buffer_pushb(buffer, word & 0xFF);
        subleq_byte_buffer_pushb(buffer, (word >> 0x08) & 0xFF);
}
void subleq_byte_buffer_pushs(SUBLEQ_BYTE_BUFFER *buffer, const char *source) {
        int i;
        for(i = 0; source[i]; i++) subleq_byte_buffer_pushb(buffer, source[i]);
}

void subleq_compile_tokens(SUBLEQ_BYTE_BUFFER *buffer, const SUBLEQ_TOKENS *tokens) {
        unsigned int i;
        char *string;

        subleq_byte_buffer_pushs(buffer, "format elf64 executable\n");
        subleq_byte_buffer_pushs(buffer, "entry _start\n");
        subleq_byte_buffer_pushs(buffer, "segment readable executable\n");
        subleq_byte_buffer_pushs(buffer, "_start:\n");
        subleq_byte_buffer_pushs(buffer, "    sub rsp, 0x10000\n");
        subleq_byte_buffer_pushs(buffer, "    mov rsi, program\n");
        subleq_byte_buffer_pushs(buffer, "    mov rdi, rsp\n");
        subleq_byte_buffer_pushs(buffer, "    mov rcx, program_len\n");
        subleq_byte_buffer_pushs(buffer, "    rep movsb\n");
        subleq_byte_buffer_pushs(buffer, "\n");
        subleq_byte_buffer_pushs(buffer, "    xor rsi, rsi\n");
        subleq_byte_buffer_pushs(buffer, ".loop:\n");
        subleq_byte_buffer_pushs(buffer, "    mov rbp, rsi\n");
        subleq_byte_buffer_pushs(buffer, "    shl rbp, 0x01\n");
        subleq_byte_buffer_pushs(buffer, "    add rbp, rsp\n");
        subleq_byte_buffer_pushs(buffer, "\n");
        subleq_byte_buffer_pushs(buffer, "    cmp word [rbp+0x0002], 0xFFFF\n");
        subleq_byte_buffer_pushs(buffer, "    je .putc\n");
        subleq_byte_buffer_pushs(buffer, "\n");
        subleq_byte_buffer_pushs(buffer, "    mov ax, word [rbp+0x0002]\n");
        subleq_byte_buffer_pushs(buffer, "    sub word [rbp], ax\n");
        subleq_byte_buffer_pushs(buffer, "    cmp word [rbp], 0x00\n");
        subleq_byte_buffer_pushs(buffer, "    jle .leq\n");
        subleq_byte_buffer_pushs(buffer, "    add si, 0x06\n");
        subleq_byte_buffer_pushs(buffer, "    jmp .fin\n");
        subleq_byte_buffer_pushs(buffer, ".leq:\n");
        subleq_byte_buffer_pushs(buffer, "    cmp word [rbp+0x0004], 0xFFFF\n");
        subleq_byte_buffer_pushs(buffer, "    je .eloop\n");
        subleq_byte_buffer_pushs(buffer, "    mov si, word [rbp+0x0004]\n");
        subleq_byte_buffer_pushs(buffer, ".fin:\n");
        subleq_byte_buffer_pushs(buffer, "    jmp .loop\n");
        subleq_byte_buffer_pushs(buffer, ".putc:\n");
        subleq_byte_buffer_pushs(buffer, "    xor esi, esi\n");
        subleq_byte_buffer_pushs(buffer, "    mov si, word [rbp]\n");
        subleq_byte_buffer_pushs(buffer, "    shl rsi, 0x01\n");
        subleq_byte_buffer_pushs(buffer, "    add rsi, rsp\n");
        subleq_byte_buffer_pushs(buffer, "    mov eax, 0x01\n");
        subleq_byte_buffer_pushs(buffer, "    mov edi, eax\n");
        subleq_byte_buffer_pushs(buffer, "    mov edx, eax\n");
        subleq_byte_buffer_pushs(buffer, "    syscall\n");
        subleq_byte_buffer_pushs(buffer, "    xor esi, esi\n");
        subleq_byte_buffer_pushs(buffer, "    mov si, word [rbp+0x04]\n");
        subleq_byte_buffer_pushs(buffer, "    jmp .loop\n");
        subleq_byte_buffer_pushs(buffer, ".eloop:\n");
        subleq_byte_buffer_pushs(buffer, "    mov rax, 0x3C\n");
        subleq_byte_buffer_pushs(buffer, "    mov rdi, 0x00\n");
        subleq_byte_buffer_pushs(buffer, "    syscall\n");
        subleq_byte_buffer_pushs(buffer, "\n");
        subleq_byte_buffer_pushs(buffer, "segment readable\n");
        subleq_byte_buffer_pushs(buffer, "program:\n");

        string = malloc(strlen("dw 0x0000, 0x0000, 0x0000\n")+1);
        for(i = 0; i < tokens->length; i+=3) {
                sprintf(string, "dw 0x%04X, 0x%04X, 0x%04X\n", tokens->values[i], tokens->values[i+1], tokens->values[i+2]);
                subleq_byte_buffer_pushs(buffer, string);
        }
        free(string);

        subleq_byte_buffer_pushs(buffer, "program_len = $-program\n");
}
