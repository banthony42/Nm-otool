/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: banthony <banthony@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/06 20:12:42 by banthony          #+#    #+#             */
/*   Updated: 2017/11/26 19:26:19 by banthony         ###   ########.fr       */
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
	char		*value;				/*valeur du symbol*/
	uint8_t		type;				/*Type du symbol*/
	char		padding[7];			/*variable d'alignement de la structure*/
	char		*name;				/*Nom du symbol*/
}				t_smb;

typedef struct	s_data
{
	char			*av;													/*Entree utilisateur*/
	void			*file;													/*fichier mappe en memoire*/
	void			*first_sectoff;											/*ptr vers premiere segment*/
	int				fd;														/*fd du fichier*/
	unsigned int	data_len;												/*taille de la liste*/
	struct stat		stat;													/*stat du fichier*/
	size_t			token;													/*option ou fichier*/
	char			opt[16];												/*si option, lesquelles*/
	void			(*lstadd_somewhere)(t_list **begin, t_list *new);		/*voir ci dessous*/
	void			(*lst_browser)(t_list *lst, void (*f)(t_list *elem));	/*voir ci dessous*/
	t_list			*sym;													/*Liste des symbol du fichier*/
}					t_data;


/*
**	Fonction pour lstadd_somewhere, valeur par defaut: lstadd_alpha
*/
void			lstadd_alpha(t_list **begin, t_list *new);		/*Tri alphabetiquement a l'insertion (nm)*/
void			lstadd_numeric(t_list **begin, t_list *new);	/*Tri numeriquement a l'insertion (nm -n)*/
/*void			ft_lstaddback(t_list **begin, t_list **new);*/	/*Ajout dans l'ordre de la symtable (nm -p)*/

/*
**	Fonction pour lst_browser, valeur par defaut: ft_lstiter
*/
void			lstiter_reverse(t_list *lst, void (*f)(t_list *elem));	/*(nm -r)*/

/*
**	Parsing Nm
*/
int				ft_nm_info(char *str, char *info);
void			print_elem(t_list *elem);	/*Temporaire, affichage de la liste t_data*/
t_list			*parsing(char **av);
void			prepare_files(t_list *elm);
void			default_file(t_list **lst);

/*
**	Nm
*/
uint32_t		swap_uint32(uint32_t val);
uint64_t		swap_uint64(uint64_t val);
int				is_opt(void *data, char opt);
int				file_access(void *file, off_t read, off_t file_size);
char			*itoa_base_uint64(uint64_t value, int base);

void			ft_nm(t_list *elem);
int				*error_number(int *err);
void			nm_output(t_list *elem);
void			data_del(void *content, size_t size);
void			smb_del(void *content, size_t size);
uint8_t			get_symboltype64(t_data *d, struct nlist_64 symtable);

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

















