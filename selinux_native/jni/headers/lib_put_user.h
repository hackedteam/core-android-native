#include <stdbool.h>

bool write_value_at_address(unsigned long address, int value);
int read_value_at_address(unsigned long int address, unsigned long int *value);

int put_user_check_exploitability(void);
int put_user_get_root(char *, char *);
