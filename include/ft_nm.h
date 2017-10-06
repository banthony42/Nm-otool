/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: banthony <banthony@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/06 20:12:42 by banthony          #+#    #+#             */
/*   Updated: 2017/10/06 20:50:34 by banthony         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef		FT_NM_H
# define	FT_NM_H

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "libft.h"

# define NM_USG "ft_nm [-gnopruUx] [file...]\n"

typedef enum	e_options
{
	G, N, O, P, R, U1, U2, X,
}				t_options;

typedef struct	s_nm
{
	char		opt[8];
}				t_nm;

#endif

















