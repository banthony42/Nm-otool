/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: banthony <banthony@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/06 20:12:42 by banthony          #+#    #+#             */
/*   Updated: 2017/12/11 13:30:05 by banthony         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_NM_H
# define FT_NM_H

# include <sys/mman.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>
# include <mach-o/loader.h>
# include <mach-o/nlist.h>
# include <mach-o/fat.h>
# include <ar.h>
# include <ranlib.h>
# include <errno.h>
# include "libft.h"

/*
**	Messages
*/
# define NM_USG "ft_nm [-gnpruUj--] [file...]\n"
# define FILE_NOT_FOUND "No such file or directory."
# define ERR_FILE "The file was not recognized as a valid object file"
# define ERR_MAGIC "The magic number was not recognized"
# define FSTAT_ERROR "fstat error."
# define MMAP_ERROR "mmap has failed."
# define PERMISSION "Permission denied."
# define IS_A_DIR "Is a directory."
# define UNKNOWN_OPTION "Unknown command line argument"

# define AVAILABLE_OPTIONS "gnopruUxj-"
# define NB_OPTIONS ft_strlen(AVAILABLE_OPTIONS) - 1

# define OFFSET(OFFSET, BEGIN) ((char*)OFFSET - (char*)BEGIN)
# define ARCHIVE_CONCAT 42
# define ARCHIVE_CONCAT_MSG "ft_nm: archive inside an archive, is not managed"
# define RANLIB	-42
# define ARCH32 32
# define ARCH64 64
# define ARCH_OK 21
# define ARCH_ALL -21
# define PADD_SPACE64	"                "
# define PADD_SPACE32	"        "

typedef enum		e_options
{
	OPTION, PATH
}					t_options;

typedef struct		s_arch
{
	char			*name;
	cpu_type_t		cputype;
	cpu_type_t		cpusubtype;
}					t_arch;

# define ARCH_DATA_SIZE 44

extern t_arch		g_arch_data[ARCH_DATA_SIZE];

/*
**	value: valeur du symbol
**	type: type du symbol
**	arch: ARCH32 ou ARCH64
**	options: Tableau d'options presentes
**	name: nom du symbol
*/
typedef	struct		s_smb
{
	char			*value;
	uint8_t			type;
	uint8_t			arch;
	char			options[22];
	char			*name;
}					t_smb;

/*
**	av: Entree utilisateur
**	file: Fichier mappe en memoire
**	first_sectoff: ptr vers le premiers segment du fichier
**	fd: fd du fichier
**	data_len: taille de la liste de fichier + un maillon d'options
**	stat: info du fichier via fstat
**	token: Flag pour differencier options d'un fichier
**	opt: tableau d'options, rempli par la lettre correspondante sinon '-'
**	lstadd_somewhere: fonction de tri par insertion, suivant options -np
**	lst_browser: fonction de parcourt de liste, suivant option -r
*/
typedef struct		s_data
{
	char			*av;
	void			*file;
	void			*first_sectoff;
	int				fd;
	unsigned int	data_len;
	struct stat		stat;
	size_t			token;
	char			opt[16];
	void			(*lstadd_somewhere)(t_list **begin, t_list *new);
	void			(*lst_browser)(t_list *lst, void (*f)(t_list *elem));
	t_list			*sym;
}					t_data;

/*
**	Fonction pour lstadd_somewhere, valeur par defaut: lstadd_alpha
*/
void				lstadd_alpha(t_list **begin, t_list *new);
void				lstadd_numeric(t_list **begin, t_list *new);

/*
**	Fonction pour lst_browser, valeur par defaut: ft_lstiter
*/
void				lstiter_reverse(t_list *lst, void (*f)(t_list *elem));

/*
**	Parsing Nm
*/
int					ft_nm_info(char *str, char *info);
t_list				*parsing(char **av);
void				prepare_files(t_list *elm);
void				default_file(t_list **lst);
char				*ft_strndup(char *s1, uint32_t n, void *file, off_t size);

/*
**	Temporaire
*/
void				print_elem(t_list *elem);

/*
**	Nm
*/
void				ft_nm(t_list *elem);
int					ft_islower(int c);
int					is_opt(char *data, char opt);
int					is_corrup(unsigned char *ptr, void *file, off_t size);
int					*error_number(int *err);
void				nm_display(t_data *d);
void				nm_output(t_list *elem);
t_data				*new_data(char *str, int *wait);
void				data_del(void *content, size_t size);
void				smb_del(void *content, size_t size);

/*
**	Nm - Archive
*/
int					archive_handler(void *file, off_t size, t_data *d);

/*
**	Nm - Universal binaries x32
*/
int					fat_arch_32_handler(uint32_t magic, t_data *d
										, unsigned char *file, off_t size);
int					fat_arch_32_cigam(uint32_t nfat_arch, t_data *d
										, unsigned char *file, off_t size);
int					fat_arch_32_magic(uint32_t nfat_arch, t_data *d
										, unsigned char *file, off_t size);
void				print_arch(struct fat_arch frh, t_data *d, int mgc);

/*
**	Nm - Universal binaries x64
**	La struct fat_arch_64 n'est pas toujours presentes sur les mac.
**	De plus lorsqu'elle est presente on peut y lire ce commentaire:
**	"The support for the 64-bit fat file format described here is a work in
**	progress and not yet fully supported in all the Apple Developer Tools."
**	C'est pourquoi j'ai fait le choix de ne pas gerer ce type de fichier.
*/

/*
**	Nm - Mach-o x32
*/
int					arch_32_handler(uint32_t magic, t_data *d, void *file
															, off_t size);
int					arch_32_magic(uint32_t ncmds, t_data *d
										, unsigned char *file, off_t size);
int					arch_32_cigam(uint32_t ncmds, t_data *d
										, unsigned char *file, off_t size);
uint32_t			swap_uint32(uint32_t val);
uint8_t				get_symboltype32magic(t_data *d, struct nlist symtable);
uint8_t				get_symboltype32cigam(t_data *d, struct nlist symtable);
char				*itoa_base_uint32(uint32_t value, int base);

/*
**	Nm - Mach-o x64
*/
int					arch_64_handler(uint32_t magic, t_data *d, void *file
															, off_t size);
int					arch_64_magic(uint32_t ncmds, t_data *d
										, unsigned char *file, off_t size);
int					arch_64_cigam(uint32_t ncmds, t_data *d
										, unsigned char *file, off_t size);
uint64_t			swap_uint64(uint64_t val);
char				*itoa_base_uint64(uint64_t value, int base);
uint8_t				get_symboltype64magic(t_data *d, struct nlist_64 symtable);
uint8_t				get_symboltype64cigam(t_data *d, struct nlist_64 symtable);

#endif
