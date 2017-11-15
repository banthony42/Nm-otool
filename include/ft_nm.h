/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: banthony <banthony@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/06 20:12:42 by banthony          #+#    #+#             */
/*   Updated: 2017/11/15 19:08:02 by banthony         ###   ########.fr       */
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
#include <ar.h>
#include <ranlib.h>
# include "libft.h"


#include <stdio.h>


/*
**	Messages
*/
# define NM_USG "ft_nm [-gnopruUxj--] [file...]\n"
# define FILE_NOT_FOUND "No such file or directory."
# define ERR_FILE "The file was not recognized as a valid object file"
# define ERR_MAGIC "The magic number was not recognized"
# define FSTAT_ERROR "fstat error."
# define MMAP_ERROR "mmap has failed."
# define CANT_READ "Permission denied."
# define UNKNOWN_OPTION "Unknown command line argument"

# define AVAILABLE_OPTIONS "gnopruUxj-"
# define NB_OPTIONS ft_strlen(AVAILABLE_OPTIONS) - 1

# define OFFSET(OFFSET, BEGIN) ((char*)OFFSET - (char*)BEGIN)
# define ARCHIVE_CONCAT 42
# define ARCHIVE_CONCAT_MSG "ft_nm: archive inside an other archive, is not managed"
# define RANLIB	-42
# define PADD_ZERO	"0000000000000000"
# define PADD_SPACE	"                "

typedef enum	e_options
{
	OPTION, PATH,
}				t_options;

typedef	struct	s_smb
{
	char		*value;
	uint8_t		type;
	char		padding[7];
	char		*name;
}				t_smb;

typedef struct	s_data
{
	char		*av;
	void		*file;
	int			fd;
	unsigned int	data_len;
	struct stat	stat;
	size_t		token;
	char		opt[16];
	void		(*lstadd_somewhere)(t_list **begin, t_list *new);
	void		(*lst_browser)(t_list *lst, void (*f)(t_list *elem));
	t_list		*sym;
}				t_data;


/*
**	Fonction pour lstadd_somewhere, valeur par defaut: ft_lstaddback
*/
void			lstadd_alpha(t_list **begin, t_list *new);		/*Tri alphabetiquement a l'insertion*/
void			lstadd_numeric(t_list **begin, t_list *new);	/*Tri numeriquement a l'insertion*/

/*
**	Fonction pour lst_browser, valeur par defaut: ft_lstiter
*/
void			lstiter_reverse(t_list *lst, void (*f)(t_list *elem));	/*Idem a ft_lstiter, mais fin vers debut*/


/*
**	Parsing Nm
*/
int				ft_nm_info(char *str, char *info);
void			print_elem(t_list *elem);
t_list			*parsing(char **av);
void			prepare_files(t_list *elm);
void			default_file(t_list **lst);

/*
**	Nm
*/
uint32_t		swap_uint32(uint32_t val);
uint64_t		swap_uint64(uint64_t val);
int				file_access(void *file, off_t read, off_t file_size);
char			*itoa_base_uint64(uint64_t value, int base);

void			ft_nm(t_list *elem);
void			nm_output(t_list *elem);
void			data_del(void *content, size_t size);
void			smb_del(void *content, size_t size);

int				archive_handler(t_data *d);
int				fat_arch_32_cigam(uint32_t nfat_arch, t_data *d, unsigned char *file, off_t size);
int				fat_arch_32_magic(uint32_t nfat_arch, t_data *d, unsigned char *file, off_t size);
int				fat_arch_32_handler(uint32_t magic, t_data *d, unsigned char *file, off_t size);
int				arch_32_handler(uint32_t magic, t_data *d, void *file, off_t size);
int				arch_64_handler(uint32_t magic, t_data *d, void *file, off_t size);

int				arch_64_magic(uint32_t ncmds, t_data *d, unsigned char *file, off_t size);
int				arch_64_cigam(uint32_t ncmds, t_data *d, unsigned char *file, off_t size);

/*
**	struct fat_arch_64 pas toujours present sur les mac
*/
/*
int				fat_arch_64_handler(uint32_t magic, unsigned char *file, off_t size);
int				fat_arch_64_cigam(uint32_t nfat_arch, struct fat_arch_64 *frh, unsigned char *file, off_t size);
int				fat_arch_64_magic(uint32_t nfat_arch, struct fat_arch_64 *frh, unsigned char *file, off_t size);
*/
#endif

















