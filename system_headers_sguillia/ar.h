
#ifndef AR_H
#define	AR_H

#define	OARMAG1	0177555
#define	OARMAG2	0177545

#define	ARMAG		"!<arch>\n"
#define	SARMAG		8

#define	AR_EFMT1	"#1/"

#define	ARFMAG	"`\n"

struct ar_hdr {
	char ar_name[16];
	char ar_date[12];
	char ar_uid[6];
	char ar_gid[6];
	char ar_mode[8];
	char ar_size[10];
	char ar_fmag[2];
};

#endif
