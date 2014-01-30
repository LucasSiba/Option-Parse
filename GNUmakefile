CFLAGS = -I. -g -O9 -rdynamic -fstack-protector -fno-strict-aliasing -Wall -Werror -Wextra -Wcast-align -Wcast-qual -Wformat=2 -Wformat-security -Wmissing-prototypes -Wnested-externs -Wpointer-arith -Wredundant-decls -Wshadow -Wstrict-prototypes -Wno-unknown-pragmas -Wunused -Wno-unused-result -Wwrite-strings -Wno-attributes

example-option-parse: example-option-parse.o option-parse.o

.PHONY: clean

clean:
	rm -f *.o example-option-parse
