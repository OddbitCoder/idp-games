#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "utils.h"

// timer

#define BCD2BIN(val) (((val) & 15) + ((val) >> 4) * 10)

__sfr __at 0xA0 CTC_TENTHS_CS;
__sfr __at 0xA1 CTC_HUNDREDS;
__sfr __at 0xA2 CTC_SECONDS;

uint16_t timer() {
    uint8_t s = CTC_SECONDS;
    uint8_t s100 = CTC_HUNDREDS;
    uint8_t cs10 = CTC_TENTHS_CS;
    uint8_t s100_check = CTC_HUNDREDS;
    uint8_t s_check = CTC_SECONDS;
    uint16_t ms;
    if (s100 == s100_check) {
        ms = BCD2BIN(s100) * 10 + BCD2BIN(cs10 >> 4);
    } else {
        ms = BCD2BIN(s100_check) * 10;
    }
    if (s == s_check) {
        return BCD2BIN(s) * 1000 + ms;
    } else {
        return BCD2BIN(s_check) * 1000;
    }
}

// file

extern int fparse(char *path, fcb_t *fcb, uint8_t *area);

FILE *fopen(char *path) {
    FILE *f = calloc(1, sizeof(FILE));
    uint8_t area;
    fparse(path, &f->fcb, &area);
    bdos_ret_t result;
    bdosret(F_OPEN, (uint16_t)&f->fcb, &result);
    if (result.reta == BDOS_FAILURE) {
        free(f);
        return NULL;
    }
    return f;
}

uint8_t *fread(FILE *f, uint8_t *buf, uint16_t pos, uint16_t len) {
    bdos(F_DMAOFF, (uint16_t)f->dma);
    uint16_t rec = pos / DMA_SIZE;
    uint16_t dma_offs = pos % DMA_SIZE;
    f->fcb.rrec = rec;
    bdos_ret_t result;
    bdosret(F_READRAND, (uint16_t)&f->fcb, &result);
    if (result.reta == BDOS_FAILURE) {
        return NULL;
    }
    // read
    uint16_t r_len = 0;
    uint8_t *p_buf = buf;
    while (r_len < len) {
        bdosret(F_READ, (uint16_t)&f->fcb, &result);
        if (result.reta != 0) {
            return NULL;
        }
        uint16_t count = DMA_SIZE - dma_offs;
        if (r_len + count > len) {
            count = len - r_len;
        }
        memcpy(p_buf, f->dma + dma_offs, count);
        p_buf += count;
        r_len += count;
        dma_offs = 0;
    }
    return buf;
}

uint16_t fwrite(FILE *f, uint8_t *buf, uint16_t len) {
    bdos(F_DMAOFF, (uint16_t)f->dma);
    // write
    uint16_t w_len = 0;
    uint8_t *p_buf = buf;
    bdos_ret_t result;
    while (w_len < len) {
        // set DMA
        uint16_t sz = (len - w_len) > DMA_SIZE ? DMA_SIZE : (len - w_len);
        memcpy(f->dma, p_buf, sz);
        // write
        bdosret(F_WRITE, (uint16_t)&f->fcb, &result);
        if (result.reta != 0) {
            return w_len;
        }
        p_buf += sz;
        w_len += sz;
    }
    return w_len;
}

void fclose(FILE *f) {
    bdos_ret_t result;
    bdosret(F_CLOSE, (uint16_t)&f->fcb, &result);
    free(f); // WARNME: works if created by fopen
}

// string

unsigned int strlen(const char *str) {
	uint16_t len = 0;
    for (; *str; str++, len++);
    return len;
}

char *strchr(const char *str, unsigned int c) {
    char *ptr = (char *)str;
	for (; *ptr; ptr++) {
        if (*ptr == c) {
            return ptr;
        }
    }
    return NULL;
}

char *strcpy(char *dest, const char *src) {
	char *ptr = dest;
    for (; *src; src++, ptr++) {
        *ptr = *src;
    }
    *ptr = '\0';
    return dest;
}

int atoi(const char *str) {
    int val = 0;
    uint8_t sign = 1;
    uint8_t i = 0;
    if (str[0] == '-') {
        sign = -1;
        i++;
    }
    for (; str[i] != '\0'; i++) {
        val = val * 10 + str[i] - '0';
    }
    return sign * val;
}

char *strcat(char *dest, const char *src) {
	char *ptr = dest;
    for (; *ptr; ptr++);
    for (; *src; src++, ptr++) {
        *ptr = *src;
    }
    *ptr = '\0';
    return dest;
}

bool isspace(int arg) {
	return arg == '\t' || arg == '\n\r' ||
        arg == '\v' || arg == '\f' || arg == '\r' || arg == ' ';
}

bool ispunct(int arg) {
	return (arg >= 33 && arg <= 47) || (arg >= 58 && arg <= 64) 
        || (arg >= 91 && arg <= 96) || (arg >= 123 && arg <= 126);
}

int strcmp(const char *str1, const char *str2) {
    while (*str1) {
        if (*str1 != *str2) {
            break;
        }
        str1++;
        str2++;
    }
    return *str1 - *str2;
}

int strncmp(const char *str1, const char *str2, int n) {
    for (uint16_t i = 0; i < n; i++) {
        if (*str1 != *str2) {
            return *str1 - *str2;
        }
        str1++;
        str2++;
    }
    return 0;
}

// conio

void setWindowSize() {
    // nothing to do here
}

void clearScreen() {
    printf("\x1B[2J");
    setPos(1, 1);
}

void setPos(int col, int row) {
    printf("\x1B[%d;%dH", row, col);
}

void savePos() {
    printf("\x1B""7");
}

void restorePos() {
    printf("\x1B""8");
}

void setColor(int color) {
    switch (color) {
        case REGULARCOLOR:
            printf("\x1B[0m");
            break;
        case BOLDCOLOR:
            printf("\x1B[1m");
            break;
        case HEADERCOLOR:
            printf("\x1B[7m");
            break;
    }
}

void writeHeader(int line, const char *leftText, const char *rightText) {
    savePos();
    line++;
    setPos(1, line);
    setColor(HEADERCOLOR);
    for (int i = 0; i < COLS; i++) {
        putchar(' ');
    }
    if (leftText != NULL) {
        setPos(2, line);
        printLine(leftText);
    }
    if (rightText != NULL) {
        setPos(COLS - strlen(rightText), line);
        printLine(rightText);
    }
    setColor(REGULARCOLOR); 
    restorePos();
}

void delay(int millisec) {
    millisec;
    /* For 1 millisec we need to sleep
       0.001/(1/4000000)=4000 t-states,
       but
        ...outer loop adds 38 t-states
        ...push and pop commands add 42 t-states to everything
       so we need 3962 t-states for 1ms + outer loop overhead */
    __asm
        pop     de                      ; return address
        pop     hl                      ; milliseconds
        ;; restore stack
        push    hl
        push    de
        ;; now count hl
    msl_loop:
        ld      b,#233                  ; 7 t-states outer counter
        ;; inner loop is 17 * 233 t-states = 3961 t-states
    msl_inner_loop:
        nop                             ; 4 t-states
        djnz    msl_inner_loop          ; 13/8 t-states
        ;; and loop.
        ;; this loop and ld b adds 38 t-states to each millisecond
        dec     hl                      ; 6 t-states
        ld      a,h                     ; 9 t-states
        or      l                       ; 4 t-states
        jr      nz,msl_loop             ; 12/7 t-states
    __endasm;
}

char getKey() {
    char ch;
    while (!(ch = toupper(kbhit())));
    //putchar(ch); // WARNME: do we need this? (this is not in the original code)
    printLine(""); 
    return ch;
}

char getKeySilent() {
    char ch;
    while (!(ch = toupper(kbhit())));
    return ch;
}

char getOneOf(char key1, char key2, char key3) {
    return 0;
}

char *getString(char *buffer, int max) {
    char ch;
    uint8_t len = 0;
    do {
        // TODO: history, back/fwd arrow, insert, delete...
        while (!(ch = kbhit()));
        if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9') || ch == ' ') {
            if (len < 2 * max + 1) {
                buffer[len] = ch;
                printf("%c", ch);
                buffer[++len] = '\0'; 
            } 
        } else if (ch == '\b') { 
            if (len > 0) {
                printf("\b \b");
                buffer[--len] = '\0';
            }
        } 
    } while (ch != '\r' || len == 0);
    printf("\n\r");
    return buffer;
}

// agiparse

#define DICT_SIZE 1717

int results[8];
char *dict = NULL;

void loadDict()
{
    FILE *f = fopen("WORDS.BIN");
    if (f == NULL)
    {
        printf("Couldn't open dictionary.\n\r");
        return;
    }
    dict = (char *)malloc(DICT_SIZE);
    fread(f, dict, 0, DICT_SIZE);
    fclose(f);
}

void parse(char *input)
{
    if (dict == NULL)
        loadDict();

    int i;
    for (i = 0; i < 8; i++)
        results[i] = -1;
    int numRes = 0;

    int cursor = 0;
    int inputLen = (int)strlen(input);
    while (cursor < inputLen)
    {
        if (isspace(input[cursor]) || ispunct(input[cursor]))
            cursor++;
        int foundLen = 1;
        char firstCh = input[cursor];
        char *here = input + cursor;
        int id = -1; //unknown word;
        if ((firstCh >= 'a' && firstCh <= 'z'))
        {
            if ((cursor + 1 < inputLen) && (input[cursor + 1] == ' ' || cursor >= inputLen))
            {
                //current word is one letter?
                if (firstCh == 'a' || firstCh == 'i')
                    id = 0; //ignore
            }
        }

        i = ((firstCh - 'a') * 2);
        int a = (unsigned char)dict[i++];
        int b = (unsigned char)dict[i++];
        i = (a << 8) | b;
        if (i == 0)
        {
            numRes = 0;
            return;
        }
        int words = dict[i];
        char thisWord[64] = { firstCh, 0 };

        while (thisWord[0] == firstCh && i < DICT_SIZE)
        {
            char prefixLen = dict[i++];
            char *prefixedWord = thisWord + prefixLen;
            char c = 0;
            do
            {
                c = dict[i++];
                if (i >= DICT_SIZE)
                {
                    //give up
                    results[numRes++] = -1;
                    return;
                }
                *prefixedWord++ = (char)((c ^ 0x7F) & 0x7F);
            } while (((c & 0x80) != 0x80));
            if (thisWord[0] != firstCh)
            {
                numRes = 0;
                results[0] = -1;
                cursor += (int)strlen(thisWord);
                break;
            }
            *prefixedWord = 0;
            int a = (unsigned char)dict[i++];
            int b = (unsigned char)dict[i++];
            id = (a << 8) | b;
        
            int wordLen = (int)strlen(thisWord);
            if (!strncmp(here, thisWord, wordLen))
            {
                results[numRes++] = id;
                cursor += wordLen + 1;
                break;
            }
        }
    }
}

// tests

void run_tests()
{
    uint8_t buffer[256];
    
    // clear screen
    
    printf("SOME TEXT\n\r"); 
    clearScreen(); 
    setPos(1, 2);
    
    // string
    
    printf("strlen: 4 == %d\n\r", strlen("TEST"));
    
    printf("strchr: EST == %s\n\r", strchr("TEST", 'E'));
    
    printf("strchr: 0 == %d\n\r", strchr("TEST", 'X'));
    
    printf("strcpy: TEST == %s\n\r", strcpy(buffer, "TEST"));
    
    printf("atoi: 42 == %d\n\r", atoi("42"));
    
    buffer[2] = 0; 
    printf("strcat: TEST == %s\n\r", strcat(buffer, "ST"));
    
    printf("isspace: 1 == %d\n\r", isspace(' '));
    
    printf("isspace: 0 == %d\n\r", isspace('X'));
    
    printf("ispunct: 1 == %d\n\r", ispunct(','));
    
    printf("ispunct: 0 == %d\n\r", ispunct('X'));
    
    // conio
    
    setPos(1, 1); 
    savePos(); 
    printf("?"); 
    
    setPos(1, 12); 
    printf("X");
    restorePos(); 
    printf("X"); 
    setPos(1, 13);
    
    setColor(BOLDCOLOR);
    printf("BOLD\n\r");
    
    setColor(HEADERCOLOR);
    printf("HEADER\n\r");
    
    setColor(REGULARCOLOR);
    printf("REGULAR\n\r");   

    writeHeader(20, "LEFT", "RIGHT");
}