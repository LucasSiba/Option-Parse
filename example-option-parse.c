#include <ctype.h>
#include <stdio.h>

#include "option-parse.h"

int
main(int argc, char * argv[])
{
    unsigned short  port               = 1337;
    unsigned        connection_count   = 20000;
    unsigned long   max_transactions   = 9999999999;
    int             offset             = -1;
    double          timeout_in_seconds = 2.5;
    char            magic_character    = 'Z';
    const char*     ip_address         = "127.0.0.1";
    
    //op_add(short_option_character, long_option_name, c_data_type, pointer_to_variable); 
    op_add('p', "port",               OP_UNSIGNED_SHORT, &port);
    op_add('c', "connection_count",   OP_UNSIGNED,       &connection_count);
    op_add('m', "max_transactions",   OP_UNSIGNED_LONG,  &max_transactions);
    op_add('o', "offset",             OP_INT,            &offset);
    op_add('t', "timeout_in_seconds", OP_DOUBLE,         &timeout_in_seconds);
    op_add('r', "magic_character",    OP_CHAR,           &magic_character);
    op_add('i', "ip_address",         OP_CONST_CHAR_PTR, &ip_address);

    // Shows the usage
    op_usage(stdout);
    // Usage:
    //  -p --port      (default 1337)
    //  -c --connection_count  (default 20000)
    //  -m --max_transactions  (default 9999999999)
    //  -o --offset    (default -1)
    //  -t --timeout_in_seconds        (default 2.500)
    //  -r --magic_character   (default Z)
    //  -i --ip_address        (default 127.0.0.1)

    // Will show usage on failure, otherwise shows what options are set to
    op_run(argc, argv, stdout);
    // Using options:
    //  -p --port 1337
    //  -c --connection_count 20000
    //  -m --max_transactions 9999999999
    //  -o --offset -1
    //  -t --timeout_in_seconds 2.500
    //  -r --magic_character Z
    //  -i --ip_address '127.0.0.1'

    return 0;
}
