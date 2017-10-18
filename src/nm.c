/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: banthony <banthony@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/13 18:56:31 by banthony          #+#    #+#             */
/*   Updated: 2017/10/18 19:42:23 by banthony         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

void	arch_64_handler(uint32_t magic, void *file)
{
	struct mach_header_64 *hdr64;

	if (!file)
		return ;
	if (magic == MH_CIGAM_64)
		;
	ft_putendlcol(YELLOW, "Arch_64");
	hdr64 = (struct mach_header_64 *)file;
	if (hdr64->filetype == MH_OBJECT)
		ft_putendl("OBJECT");
}

void	arch_32_handler(uint32_t magic, void *file)
{
	if (!file)
		return ;
	if (magic == MH_CIGAM)
		;
	ft_putendlcol(RED, "Arch_32");
}

void		ft_nm(t_list *elem)
{
	uint64_t n = 0xbebafeca2afecabe;
	uint32_t	magic;
	t_data		*d;

	printf("n:%llx - %llx\n", n, swap_uint64(n));
	if (!elem || !elem->content)
		return ;
	if ((d = (t_data*)elem->content)->token != PATH || !d->file)
		return ;
	if ((magic = *(uint32_t*)d->file) == MH_MAGIC_64 || magic == MH_CIGAM_64)	/*Mach-O 64bit*/
		arch_64_handler(magic, d->file);
	else if (magic == MH_MAGIC || magic == MH_CIGAM)	/*Mach-O 32bit*/
		arch_32_handler(magic, d->file);
	else if (magic == FAT_MAGIC || magic == FAT_CIGAM)	/*Universal Binary*/
		fat_arch_32_handler(magic, (unsigned char*)d->file);
	else if (magic == FAT_MAGIC_64 || magic == FAT_CIGAM_64)	/*Universal Binary*/
		fat_arch_64_handler(magic, (unsigned char*)d->file);
	else if (!(ft_strncmp(ARMAG, (char*)d->file, SARMAG)))	/*Archive*/
		archive_handler(d);
	else
		ft_putendl("NONE");
	/*ARMAG/SARMAG - Correspond au magic_str et size_magic_str - voir ar.h - */
}










