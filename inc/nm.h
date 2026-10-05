#ifndef NM_H
# define NM_H

# include <stdio.h>
# include <fcntl.h>
# include <unistd.h>
# include <sys/stat.h>
# include <sys/mman.h>
# include <elf.h>
# include <stdint.h>

typedef struct s_symbol {
    char        *name;
    uint64_t    address;
    char        type;
    int         is_64;
}               t_symbol;

#endif