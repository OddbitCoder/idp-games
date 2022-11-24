/*
 * utils.c
 *
 * Miha Grčar 2022
 *
 * Various utils for Adventure
 *
 */

#include <string.h>
#include "vocab.h"
#include "utils.h"

#ifdef __EMSCRIPTEN__

#include <emscripten.h>

EM_JS(BOOL, check_stdin, (), {
    return Module.cmdAvailable();
});

EM_JS(void, _fflush, (), {
    Module._fflush();
});

#endif

void create_fn(char *name, char *fn) {
    UINT8 len = length(name) - 1;
    if (len > 8) { len = 8; }
    memcpy(fn, name, len);
    memcpy(fn + len, ".SAV", 5);
    to_upper(fn);
}

/*UINT8 *__fread(char *path, UINT8 *buf, UINT16 pos, UINT16 len) {
    return NULL;
}*/

/*BOOL __fwrite(char *path, UINT8 *buf, UINT16 len) {
    return FALSE;
}*/

char *__fgets(char *str, int n, FILE *stream) {
#ifdef __EMSCRIPTEN__
    while (TRUE) {
        if (check_stdin()) {
            break;
        }
        emscripten_sleep(100);
    }
#endif
    fgets(str, n, stream);
    for (char *p = str; *p != '\n' || (*p = '\0'); p++);
    return str;
}

void stdout_flush() {
#ifdef __EMSCRIPTEN__
    _fflush();
#else
    fflush(stdout);
#endif
}

void to_lower(char *str) {
    for (; *str; str++) {
        if (*str >= 'A' && *str <= 'Z') {
            *str += 'a' - 'A';
        }
    }
}

void to_upper(char *str) {
    for (; *str; str++) {
        if (*str >= 'a' && *str <= 'z') {
            *str -= 'a' - 'A';
        }
    }
}

void con_in(char *buffer) {
#ifndef __EMSCRIPTEN__
    printf("? ");
    stdout_flush();
#endif
    __fgets(buffer, BUFFER_SIZE, stdin);
}

void parse_in(char *buffer, char *w1_buf, char *w2_buf, int w1_max_len, int w2_max_len) {
    // word 1
    char *p_buf = buffer, *w_start;
    for (; *p_buf == ' '; p_buf++);
    w_start = p_buf;
    UINT8 len = 0;
    for (; *p_buf != ' ' && *p_buf != '\0'; p_buf++, len++);
    if (len > w1_max_len) {
        memcpy(w1_buf, w_start, w1_max_len);
        w1_buf[w1_max_len] = '\0';
    } else {
        memcpy(w1_buf, w_start, len);
        w1_buf[len] = '\0';
    }
    to_lower(w1_buf);
    // word 2
    w2_buf[0] = '\0';
    if (w2_max_len > 0) {
        for (; *p_buf == ' '; p_buf++); 
        if (*p_buf != '\0') { // is there a word?
            w_start = p_buf;
            len = 0;
            for (; *p_buf != ' ' && *p_buf != '\0'; p_buf++, len++);
            if (len > w2_max_len) {
                memcpy(w2_buf, w_start, w2_max_len);
                w2_buf[w2_max_len] = '\0';
            } else {
                memcpy(w2_buf, w_start, len);
                w2_buf[len] = '\0';
            }
            to_lower(w2_buf);
        }
    }
}