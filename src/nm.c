/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: banthony <banthony@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/13 18:56:31 by banthony          #+#    #+#             */
/*   Updated: 2017/11/08 20:12:50 by banthony         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

int		arch_32_handler(uint32_t magic, void *file, off_t size)
{
	if (!file)
		return (0);
	if (magic == MH_CIGAM && size)
		;
	ft_putendlcol(RED, "Arch_32");
	return (1);
}

int		arch_64_handler(uint32_t magic, void *file, off_t size)
{
	struct mach_header_64	*hdr64;

	if (!file)
		return (0);
	if (magic == MH_CIGAM_64)
		ft_putendlcol(RED, "MH_CIGAM_64");
	ft_putendlcol(YELLOW, "Arch_64");
	hdr64 = (struct mach_header_64 *)file;
	if (hdr64->filetype == MH_OBJECT && size)
		ft_putendl("OBJECT");
	return (1);
}

void	ft_nm(t_list *elem)
{
	uint32_t	magic;
	t_data		*d;
	int			error;

	error = -1;
	if (!elem || !elem->content)
		return ;
	if ((d = (t_data*)elem->content)->token != PATH || !d->file)
		return ;
	if ((magic = *(uint32_t*)d->file) == MH_MAGIC_64 || magic == MH_CIGAM_64)	/*Mach-O 64bit*/
		error  = arch_64_handler(magic, d->file, d->stat.st_size);
	else if (magic == MH_MAGIC || magic == MH_CIGAM)	/*Mach-O 32bit*/
		error = arch_32_handler(magic, d->file, d->stat.st_size);
	else if (!(ft_strncmp(ARMAG, (char*)d->file, SARMAG)))	/*Archive*/
		error  = archive_handler(d);
	else if (magic == FAT_MAGIC || magic == FAT_CIGAM)	/*FAT BINARY 32*/
		error = fat_arch_32_handler(magic, (unsigned char*)d->file, d->stat.st_size);
	else
		ft_putendlcol(YELLOW, "NM: UNKNOW MAGIC");
/*	else if (magic == FAT_MAGIC_64 || magic == FAT_CIGAM_64)
	error = fat_arch_64_handler(magic, (unsigned char*)d->file, d->stat.st_size);

	Decrit comme un format en WorkInProgress dans fat.h
	Aucun universal binary utilisant fat_arch_64 trouve pour l'instant
*/
	if (error <= 0)
		error_str(d->av, ERR_FILE);
	/*error < 0: Erreur sur magic_number ; error == 0: Erreur lors de la lecture fichier*/
	/*ARMAG/SARMAG - Correspond au magic_str et size_magic_str - voir ar.h - */
}
