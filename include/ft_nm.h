/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: banthony <banthony@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/06 20:12:42 by banthony          #+#    #+#             */
/*   Updated: 2017/10/15 15:26:23 by banthony         ###   ########.fr       */
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
# include "libft.h"


#include <stdio.h>


/*
**	Messages
*/
# define NM_USG "ft_nm [-gnopruUx--] [file...]\n"
# define FILE_NOT_FOUND "No such file or directory."
# define FSTAT_ERROR "fstat error."
# define MMAP_ERROR "mmap has failed."
# define CANT_READ "Permission denied."
# define UNKNOWN_OPTION "Unknown command line argument"

# define AVAILABLE_OPTIONS "gnopruUx-"
# define NB_OPTIONS 8
# define SHIFT(N) (1 << N)

typedef enum	e_options
{
	G, N, O, P, R, U1, U2, X, OPTION, PATH,
}				t_options;

typedef struct	s_data
{
	char		*av;
	void		*file;
	int			fd;
	int			align;
	struct stat	stat;
	size_t		token;
	char		opt[8];
}				t_data;

/*
**	Parsing
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
void			ft_nm(t_list *elem);

#endif
