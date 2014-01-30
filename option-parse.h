#ifndef __OPTION_PARSE_H__
#define __OPTION_PARSE_H__

#include <ctype.h>
#include <sys/types.h>

typedef enum {
    OP_UNSIGNED_SHORT,
    OP_UNSIGNED,
    OP_UNSIGNED_LONG,
    OP_SHORT,
    OP_INT,
    OP_LONG,
    OP_LONG_LONG,
    OP_DOUBLE,
    OP_CHAR,
    OP_CONST_CHAR_PTR,
} OP_TYPE;

void op_usage(FILE* fh);
void op_add(char short_name, const char* long_name, OP_TYPE type, void* var);
void op_run(int argc, char * argv[], FILE* fh);

#endif /* __OPTION_PARSE_H__ */
