#ifndef _MACH_O_RANLIB_H_
#define _MACH_O_RANLIB_H_

#include <stdint.h>
#include <sys/types.h>		/* off_t */

#define SYMDEF		"__.SYMDEF"
#define SYMDEF_SORTED	"__.SYMDEF SORTED"

/*
 * Structure of the __.SYMDEF table of contents for an archive.
 * __.SYMDEF begins with a long giving the size in bytes of the ranlib
 * structures which immediately follow, and then continues with a string
 * table consisting of a long giving the number of bytes of strings which
 * follow and then the strings themselves.  The ran_strx fields index the
 * string table whose first byte is numbered 0.
 */
struct	ranlib {
    union {
	uint32_t	ran_strx;	/* string table index of */
#ifndef __LP64__
	char		*ran_name;	/* symbol defined by */
#endif
    } ran_un;
    uint32_t		ran_off;	/* library member at this offset */
};
#endif /* _MACH_O_RANLIB_H_ */
