#include "glob.h"

#include <assert.h>
#include <stdio.h>

static int num_failed = 0;
static int num_ran = 0;

void check_match(const char *pattern, const char *str) {
    num_ran++;
    if (!glob(pattern, str)) {
        fprintf(stderr, "expected %s to match %s\n", pattern, str);
        num_failed++;
    }
}

void check_no_match(const char *pattern, const char *str) {
    num_ran++;
    if (glob(pattern, str)) {
        fprintf(stderr, "expected %s not to match %s\n", pattern, str);
	num_failed++;
    }
}

int main(void) {
    check_match("abc", "abc");
    check_match("?bc", "abc");
    check_match("a?c", "abc");
    check_match("ab?", "abc");
    check_match("??c", "abc");
    check_match("a??", "abc");
    check_match("???", "abc");

    check_no_match("abcd", "abc");
    check_no_match("abc", "abcd");
    check_no_match("a?d", "abcd");

    check_match("a[]]c", "a]c");
    check_match("a[abc]c", "abc");
    check_match("a[bc]c", "abc");
    check_match("a[a-zA-Z]c", "aBc");
    check_no_match("a[bc]d", "abc");
    check_no_match("a[adc]c", "abc");
    check_match("[A-Z]at", "Cat");
    check_match("[-Z]at", "-at");

    check_no_match("a[^]]c", "a]c");
    check_no_match("a[^abc]c", "abc");
    check_match("a[^adc]c", "abc");

    check_match("*", "abc");
    check_match("*d", "abcd");
    check_match("x*d", "xabcd");
    check_match("x*", "xabcd");
    check_no_match("x*d", "axabcd");
    check_match("da*da*da*", "daaadabadmanda");
    check_match("*?", "XX");

    check_no_match("_[[-]]_", "_]_");
    check_no_match("_[[-]A-Z]_", "_]_");

    check_match("_[*]_", "_*_");
    check_match("_[-]_", "_-_");
    check_match("_[[]_", "_[_");
    check_match("_[]]_", "_]_");
    check_match("_]_", "_]_");
    check_match("_[?]_", "_?_");

    check_no_match("_[*]_", "_**_");
    check_no_match("_[[]_", "_]_");
    check_no_match("_[]]_", "_[_");
    check_no_match("_[?]_", "___");

    check_match("####-##-##", "2000-01-02");

    check_match("\\*", "*");
    check_match("\\\\", "\\");
    check_match("\\[", "[");
    check_match("\\?", "?");
    check_match("\\#", "#");

    printf("failed %d/%d tests.\n", num_failed, num_ran);
    return num_failed;
}

