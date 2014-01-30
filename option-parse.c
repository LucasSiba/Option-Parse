#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "option-parse.h"

#define OP_MAX_ARGS 255

typedef struct {
    char        short_name;
    const char* long_name;
    OP_TYPE     type;
    void*       var;
} OP_ARGUMENT;

// Could malloc on the fly, but meh...
static OP_ARGUMENT op_arg_list[OP_MAX_ARGS];
static int op_arg_list_next = 0;

void
op_usage(FILE* fh)
{
    int i;

    fprintf(fh, "Usage:\n");
    for (i = 0; i < op_arg_list_next; i++) {
             if (op_arg_list[i].type == OP_UNSIGNED_SHORT) { fprintf(fh, " -%c --%s\t(default %u)\n",    op_arg_list[i].short_name, op_arg_list[i].long_name, *((unsigned short*)op_arg_list[i].var)); }
        else if (op_arg_list[i].type == OP_UNSIGNED)       { fprintf(fh, " -%c --%s\t(default %u)\n",    op_arg_list[i].short_name, op_arg_list[i].long_name, *(      (unsigned*)op_arg_list[i].var)); }
        else if (op_arg_list[i].type == OP_UNSIGNED_LONG)  { fprintf(fh, " -%c --%s\t(default %lu)\n",   op_arg_list[i].short_name, op_arg_list[i].long_name, *( (unsigned long*)op_arg_list[i].var)); }
        else if (op_arg_list[i].type == OP_SHORT)          { fprintf(fh, " -%c --%s\t(default %u)\n",    op_arg_list[i].short_name, op_arg_list[i].long_name, *(         (short*)op_arg_list[i].var)); }
        else if (op_arg_list[i].type == OP_INT)            { fprintf(fh, " -%c --%s\t(default %d)\n",    op_arg_list[i].short_name, op_arg_list[i].long_name, *(           (int*)op_arg_list[i].var)); }
        else if (op_arg_list[i].type == OP_LONG)           { fprintf(fh, " -%c --%s\t(default %ld)\n",   op_arg_list[i].short_name, op_arg_list[i].long_name, *(          (long*)op_arg_list[i].var)); }
        else if (op_arg_list[i].type == OP_LONG_LONG)      { fprintf(fh, " -%c --%s\t(default %lld)\n",  op_arg_list[i].short_name, op_arg_list[i].long_name, *(     (long long*)op_arg_list[i].var)); }
        else if (op_arg_list[i].type == OP_DOUBLE)         { fprintf(fh, " -%c --%s\t(default %1.3f)\n", op_arg_list[i].short_name, op_arg_list[i].long_name, *(        (double*)op_arg_list[i].var)); }
        else if (op_arg_list[i].type == OP_CHAR)           { fprintf(fh, " -%c --%s\t(default %c)\n",    op_arg_list[i].short_name, op_arg_list[i].long_name, *(          (char*)op_arg_list[i].var)); }
        else if (op_arg_list[i].type == OP_CONST_CHAR_PTR) { fprintf(fh, " -%c --%s\t(default %s)\n",    op_arg_list[i].short_name, op_arg_list[i].long_name, *(   (const char**)op_arg_list[i].var)); }
    }
}

void
op_add(char short_name, const char* long_name, OP_TYPE type, void* var)
{
    op_arg_list[op_arg_list_next].short_name = short_name;
    op_arg_list[op_arg_list_next].long_name  = long_name;
    op_arg_list[op_arg_list_next].type       = type;
    op_arg_list[op_arg_list_next].var        = var;
    op_arg_list_next++;
    // What descriptor would be safe to write an error message to?
    assert(op_arg_list_next != OP_MAX_ARGS);
}

void
op_run(int argc, char * argv[], FILE* fh)
{
    char shortopts[255 * 2] = ""; // All the potential short opts [a-zA-Z...] + ":"
    struct option longopts[op_arg_list_next + 1];
    int i, c;

    for (i = 0; i < op_arg_list_next; i++) {
        longopts[i].name    = op_arg_list[i].long_name;
        longopts[i].has_arg = required_argument;
        longopts[i].flag    = NULL;
        longopts[i].val     = op_arg_list[i].short_name;
        char shrtstr[3];
        sprintf(shrtstr, "%c:", op_arg_list[i].short_name);
        strcat(shortopts, shrtstr);
    }
    longopts[op_arg_list_next].name    = 0;
    longopts[op_arg_list_next].has_arg = 0;
    longopts[op_arg_list_next].flag    = NULL;
    longopts[op_arg_list_next].val     = 0;

    while ((c = getopt_long(argc, argv, shortopts, longopts, NULL)) != -1) {
        if (c == 0) {
            continue;
        } else if (c == ':') {
            fprintf(fh, "option '-%c' requires an argument\n", optopt);
            op_usage(fh);
            exit(-1);
        } else if (c == '?') {
            fprintf(fh, "invalid option '-%c'\n", optopt);
            op_usage(fh);
            exit(-1);
        }

        for (i = 0; i < op_arg_list_next; i++) {
            if (c != op_arg_list[i].short_name) { continue; }
            else if (op_arg_list[i].type == OP_UNSIGNED_SHORT) { *((unsigned short*)op_arg_list[i].var) = (unsigned short)atoi (optarg);  }
            else if (op_arg_list[i].type == OP_UNSIGNED)       { *(      (unsigned*)op_arg_list[i].var) =       (unsigned)atoi (optarg);  }
            else if (op_arg_list[i].type == OP_UNSIGNED_LONG)  { *( (unsigned long*)op_arg_list[i].var) =  (unsigned long)atol (optarg);  }
            else if (op_arg_list[i].type == OP_SHORT)          { *(         (short*)op_arg_list[i].var) =          (short)atoi (optarg);  }
            else if (op_arg_list[i].type == OP_INT)            { *(           (int*)op_arg_list[i].var) =            (int)atoi (optarg);  }
            else if (op_arg_list[i].type == OP_LONG)           { *(          (long*)op_arg_list[i].var) =           (long)atol (optarg);  }
            else if (op_arg_list[i].type == OP_LONG_LONG)      { *(     (long long*)op_arg_list[i].var) =      (long long)atoll(optarg);  }
            else if (op_arg_list[i].type == OP_DOUBLE)         { *(        (double*)op_arg_list[i].var) =         (double)atof (optarg);  }
            else if (op_arg_list[i].type == OP_CHAR)           { *(          (char*)op_arg_list[i].var) =           (char)      optarg[0];}
            else if (op_arg_list[i].type == OP_CONST_CHAR_PTR) { *(  (const char **)op_arg_list[i].var) =   (const char *)      optarg;   }
            else { op_usage(fh); }
        }
    }

    fprintf(fh, "Using options:\n");
    for (i = 0; i < op_arg_list_next; i++) {
             if (op_arg_list[i].type == OP_UNSIGNED_SHORT) { fprintf(fh, " -%c --%s %u\n",    op_arg_list[i].short_name, op_arg_list[i].long_name, *((unsigned short*)op_arg_list[i].var)); }
        else if (op_arg_list[i].type == OP_UNSIGNED)       { fprintf(fh, " -%c --%s %u\n",    op_arg_list[i].short_name, op_arg_list[i].long_name, *(      (unsigned*)op_arg_list[i].var)); }
        else if (op_arg_list[i].type == OP_UNSIGNED_LONG)  { fprintf(fh, " -%c --%s %lu\n",   op_arg_list[i].short_name, op_arg_list[i].long_name, *( (unsigned long*)op_arg_list[i].var)); }
        else if (op_arg_list[i].type == OP_SHORT)          { fprintf(fh, " -%c --%s %d\n",    op_arg_list[i].short_name, op_arg_list[i].long_name, *(         (short*)op_arg_list[i].var)); }
        else if (op_arg_list[i].type == OP_INT)            { fprintf(fh, " -%c --%s %d\n",    op_arg_list[i].short_name, op_arg_list[i].long_name, *(           (int*)op_arg_list[i].var)); }
        else if (op_arg_list[i].type == OP_LONG)           { fprintf(fh, " -%c --%s %ld\n",   op_arg_list[i].short_name, op_arg_list[i].long_name, *(          (long*)op_arg_list[i].var)); }
        else if (op_arg_list[i].type == OP_LONG_LONG)      { fprintf(fh, " -%c --%s %llu\n",  op_arg_list[i].short_name, op_arg_list[i].long_name, *(     (long long*)op_arg_list[i].var)); }
        else if (op_arg_list[i].type == OP_DOUBLE)         { fprintf(fh, " -%c --%s %1.3f\n", op_arg_list[i].short_name, op_arg_list[i].long_name, *(        (double*)op_arg_list[i].var)); }
        else if (op_arg_list[i].type == OP_CHAR)           { fprintf(fh, " -%c --%s %c\n",    op_arg_list[i].short_name, op_arg_list[i].long_name, *(          (char*)op_arg_list[i].var)); }
        else if (op_arg_list[i].type == OP_CONST_CHAR_PTR) { fprintf(fh, " -%c --%s '%s'\n",  op_arg_list[i].short_name, op_arg_list[i].long_name, *(  (const char **)op_arg_list[i].var)); }
    }
}

