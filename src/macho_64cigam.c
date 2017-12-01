/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   macho_64cigam.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: banthony <banthony@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/01 22:42:10 by banthony          #+#    #+#             */
/*   Updated: 2017/12/01 22:42:34 by banthony         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

int				arch_64_cigam(uint32_t ncmds, t_data *d, unsigned char *file,
							off_t size)
{
	int error;

	error = -1;
	ft_putendlcol(YELLOW, "_64_cigam_handler");
	if (ncmds && file && size && d)
		;
	if (error)
		return (error);
	return (0);
}
