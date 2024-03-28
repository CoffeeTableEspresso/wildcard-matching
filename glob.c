#include <stdio.h>
#include <ctype.h>

#include "glob.h"

// #define LOGCHARS(a, b) printf("%c, %c\n", a, b)
#define LOGCHARS(a, b)

#define LBRACKET '['
#define RBRACKET ']'
#define RANGE '-'
#define NEGATE '^'
#define ONECHAR '?'
#define MANYCHAR '*'
#define NUM '#'
#define ESCAPE '\\'

bool glob_bracket(const char **pattern_ptr, const char **str_ptr) {
    bool b = false;
    const char *pattern = *pattern_ptr;
    const char *str = *str_ptr;
    if (*pattern == *str) {
        pattern++;
        while (*pattern != RBRACKET)
            pattern++;
        b = true;
        goto end;
    }
    while (*pattern != RBRACKET) {
        if (*pattern == *str) {
            b = true;
        }
        if (pattern[1] == RANGE && pattern[2] != RBRACKET)  {
            const char start = pattern[0];
            const char end = pattern[2];
            pattern += 2;
            if (start <= *str && *str <= end) {
                // while (*pattern != RBRACKET)
                //     pattern++;
                b = true;
                // goto end;
            }
        }
        pattern++;
    }
end:
    pattern++;
    str++;
    *pattern_ptr = pattern;
    *str_ptr = str;
    return b;
}

bool glob(const char *pattern, const char *str) {
    while (*pattern && *str) {
        LOGCHARS(*pattern, *str);
        switch (*pattern) {
        case ONECHAR:
            pattern++;
            str++;
            break;
        case NUM:
            if (!isdigit(*str)) return false;
            pattern++;
            str++;
            break;
        case LBRACKET:
            pattern++;
            switch (*pattern) {
                case NEGATE:
                    pattern++;
                    if (!glob_bracket(&pattern, &str)) break;
                    return false;
                    break;
                default:
                    if (glob_bracket(&pattern, &str)) break;
                    return false;
            }
            break;
       case MANYCHAR: {
            pattern++;
            const char *tmp = str;
            bool b = false;
            while (*tmp) {
                if (glob(pattern, tmp)) {
                    b = true;
                    break;
                }
                tmp++;
            }
            if (glob("", tmp))
                return true;
            return b;
            return false;
        }
        case ESCAPE:
            pattern++;
            /* fallthrough */
        default:
            if (*pattern != *str) return false;
            pattern++;
            str++;
            break;
        }
    }

    LOGCHARS(*pattern, *str);
    return !*pattern && !*str;
}

