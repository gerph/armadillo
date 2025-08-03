#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int _concat_internal(char **dst, const char *src, va_list args){
    size_t srclen, dstlen;
    va_list args1;
    size_t total;
    char *dst1;
    int w;

    if(!src || !dst)
        return 0;

    srclen = strlen(src), dstlen = 0;

    if(*dst)
        dstlen = strlen(*dst);

    /* Back up args before it gets used. Client calls va_end
     * on the parameter themselves when calling vconcat.
     */
#if !defined(__riscos) || defined (__riscos64)
    va_copy(args1, args);
#else
    memcpy(args1, args, sizeof(args1));
#endif

    total = srclen + dstlen + vsnprintf(NULL, 0, src, args) + 1;

    dst1 = malloc(total);

    if(!(*dst))
        *dst1 = '\0';
    else{
        strncpy(dst1, *dst, dstlen + 1);
        free(*dst);
        *dst = NULL;
    }

    w = vsnprintf(dst1 + dstlen, total, src, args1);

    va_end(args1);

    *dst = realloc(dst1, strlen(dst1) + 1);

    return w;
}

int concat(char **dst, const char *src, ...){
    va_list args;
    int w;
    va_start(args, src);

    w = _concat_internal(dst, src, args);

    va_end(args);

    return w;
}

int vconcat(char **dst, const char *src, va_list args){
    return _concat_internal(dst, src, args);
}
