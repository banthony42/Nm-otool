/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: banthony <banthony@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/06 20:12:42 by banthony          #+#    #+#             */
/*   Updated: 2017/10/07 20:37:10 by banthony         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef		FT_NM_H
# define	FT_NM_H

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include "libft.h"

// TEMPORAIRE
#include <stdio.h>
// TEMPORAIRE

/*
**	Messages
*/
# define NM_USG "ft_nm [-gnopruUx--] [file...]\n"
# define FILE_NOT_FOUND "No such file or directory."
# define FSTAT_ERROR "fstat error."
# define CANT_READ "Permission denied."
# define UNKNOWN_OPTION "Unknown command line argument"

# define AVAILABLE_OPTIONS "gnopruUx-"
# define SHIFT(N) (1 << N)

typedef enum	e_options
{
	G, N, O, P, R, U1, U2, X, OPTION, PATH,
}				t_options;

typedef struct	s_data
{
	char		*av;
	char		*file;
	int			fd;
	int			_align;
	struct stat	stat;
	size_t		token;
	char		opt[8];
}				t_data;

int				error_str(char *str, char *error);
void			print_elem(t_list *elem);
void			data_del(void *content, size_t size);
t_list			*parsing(int ac, char **av);

#endif

















