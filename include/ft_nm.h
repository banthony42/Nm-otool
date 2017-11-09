/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: banthony <banthony@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/06 20:12:42 by banthony          #+#    #+#             */
/*   Updated: 2017/11/09 18:32:55 by banthony         ###   ########.fr       */
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

typedef enum	e_options
{
	OPTION, PATH,
}				t_options;

typedef	struct	s_smb
{
	char		*head;
	char		*value;
	char		*type;
	char		*name;
}				t_smb;

typedef struct	s_data
{
	char		*av;
	void		*file;
	int			fd;
	int			padding;
	struct stat	stat;
	size_t		token;
	char		opt[16];
	t_list		*data;
}				t_data;



/*
**	Parsing Nm
*/
int				error_str(char *str, char *error);
void			print_elem(t_list *elem);
void			data_del(void *content, size_t size);
t_list			*parsing(char **av);
void			prepare_files(t_list *elm);
void			default_file(t_list **lst);

/*
**	Nm
*/
uint32_t		swap_uint32(uint32_t val);
uint64_t		swap_uint64(uint64_t val);
int				file_access(void *file, off_t read, off_t file_size);
int				archive_handler(t_data *d);
int				fat_arch_32_cigam(uint32_t nfat_arch, struct fat_arch *frh, unsigned char *file, off_t size);
int				fat_arch_32_magic(uint32_t nfat_arch, struct fat_arch *frh, unsigned char *file, off_t size);
int				fat_arch_32_handler(uint32_t magic, unsigned char *file, off_t size);
int				arch_32_handler(uint32_t magic, void *file, off_t size);
int				arch_64_handler(uint32_t magic, void *file, off_t size);
void			ft_nm(t_list *elem);


/*
**	struct fat_arch_64 pas toujours present sur les mac
*/
/*
int				fat_arch_64_handler(uint32_t magic, unsigned char *file, off_t size);
int				fat_arch_64_cigam(uint32_t nfat_arch, struct fat_arch_64 *frh, unsigned char *file, off_t size);
int				fat_arch_64_magic(uint32_t nfat_arch, struct fat_arch_64 *frh, unsigned char *file, off_t size);
*/
#endif

















