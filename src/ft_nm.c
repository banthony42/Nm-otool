/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: banthony <banthony@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/15 19:39:57 by banthony          #+#    #+#             */
/*   Updated: 2017/11/15 19:39:58 by banthony         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

int		arch_32_handler(uint32_t magic, t_data *d, void *file, off_t size)
{
	if (!file)
		return (0);
	if (magic == MH_CIGAM && size && d)
		;
	ft_putendlcol(RED, "Arch_32");
	return (1);
}


int		arch_64_handler(uint32_t magic, t_data *d, void *file, off_t size)
{
	int error;
	uint32_t ncmds;
	struct mach_header_64	*hdr64;

	if (!file_access(file, sizeof(struct mach_header_64), size))
		return (0);
	error = -1;
	hdr64 = (struct mach_header_64 *)file;
	ncmds = hdr64->ncmds;
	if (magic == MH_MAGIC_64)
			error = arch_64_magic(ncmds, d, file, size);
	if (magic == MH_CIGAM_64)
	{
		ft_putendlcol(YELLOW, "MH_CIGAM_64 - TODO");
		ncmds = swap_uint32(ncmds);
		error = arch_64_cigam(ncmds, d, file, size);
	}
	if (error <= 0)
		return (error);
/*Affichage des data ? Attention pour les archives*/
	return (1);
}

void	ft_nm(t_list *elem)
{
	uint32_t	magic;
	t_data		*d;
	int			error;

	error = -2;
	if (!elem || !elem->content)
		return ;
	if ((d = (t_data*)elem->content)->token != PATH || !d->file)
		return ;
	if (!(ft_strncmp(ARMAG, (char*)d->file, SARMAG)))
		error = archive_handler(d);
	else if (d->data_len > 2)
		ft_nm_info(d->av, NULL);
	if ((magic = *(uint32_t*)d->file) == MH_MAGIC_64 || magic == MH_CIGAM_64)
		error = arch_64_handler(magic, d, d->file, d->stat.st_size);
	else if (magic == MH_MAGIC || magic == MH_CIGAM)
		error = arch_32_handler(magic, d, d->file, d->stat.st_size);
	else if (magic == FAT_MAGIC || magic == FAT_CIGAM)
		error = fat_arch_32_handler(magic, d, (unsigned char*)d->file, d->stat.st_size);
	if (error == -2)
		ft_putendlcol(YELLOW, "NM: UNKNOW MAGIC");
	if (error <= 0)
		ft_nm_info(d->av, ERR_FILE);
}

/*
**	error < 0: Erreur sur magic_number ;
**	error == 0: Erreur lors de la lecture fichier
**	ARMAG/SARMAG - Correspond au magic_str et size_magic_str - voir ar.h
*/