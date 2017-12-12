/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm_master.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: banthony <banthony@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/11 13:45:17 by banthony          #+#    #+#             */
/*   Updated: 2017/12/11 22:18:48 by banthony         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm_otool.h"

/*
**	Avant l'affichage, transmission des options saisies
**	aux maillons de la liste de struct t_smb.
*/

void	nm_display(t_data *d)
{
	t_smb *tmp;

	tmp = (t_smb*)d->sym;
	while (d->sym)
	{
		ft_memcpy(((t_smb*)d->sym->content)->options, d->opt, NB_OPT_NM);
		d->sym = d->sym->next;
	}
	d->sym = (t_list*)tmp;
	d->lst_browser(d->sym, nm_output);
}

int		arch_32_handler(uint32_t magic, t_data *d, void *file, off_t size)
{
	int					error;
	uint32_t			ncmds;
	struct mach_header	*hdr;

	error = -1;
	hdr = (struct mach_header *)file;
	if (is_corrup((void *)(hdr + 1), file, size))
		return (1);
	ncmds = hdr->ncmds;
	if (magic == MH_MAGIC)
		error = arch_32_magic(ncmds, d, file, size);
	if (magic == MH_CIGAM)
	{
		ncmds = swap_uint32(ncmds);
		error = arch_32_cigam(ncmds, d, file, size);
	}
	if (error)
		return (error);
	return (0);
}

int		arch_64_handler(uint32_t magic, t_data *d, void *file, off_t size)
{
	int						error;
	uint32_t				ncmds;
	struct mach_header_64	*hdr64;

	error = -1;
	hdr64 = (struct mach_header_64 *)file;
	if (is_corrup((void*)(hdr64 + 1), file, size))
		return (1);
	ncmds = hdr64->ncmds;
	if (magic == MH_MAGIC_64)
		error = arch_64_magic(ncmds, d, file, size);
	if (magic == MH_CIGAM_64)
	{
		ncmds = swap_uint32(ncmds);
		error = arch_64_cigam(ncmds, d, file, size);
	}
	if (error)
		return (error);
	return (0);
}

void	ft_nm_otool(t_list *elem)
{
	uint32_t	magic;
	t_data		*d;
	int			error;

	error = 1;
	if (!elem || !elem->content)
		return ;
	if ((d = (t_data*)elem->content)->token[TYPE] != PATH || !d->file)
		return ;
	if (d->data_len > 2)
		cmd_info(FT_NM, d->av, NULL);
	if ((magic = *(uint32_t*)d->file) == MH_MAGIC_64 || magic == MH_CIGAM_64)
		error = arch_64_handler(magic, d, d->file, d->stat.st_size);
	else if (magic == MH_MAGIC || magic == MH_CIGAM)
		error = arch_32_handler(magic, d, d->file, d->stat.st_size);
	else if (magic == FAT_MAGIC || magic == FAT_CIGAM)
		error = fat_arch_32_handler(magic, d,
									(unsigned char*)d->file, d->stat.st_size);
	else if (!(ft_strncmp(ARMAG, (char*)d->file, SARMAG)))
		error = archive_handler(d->file, d->stat.st_size, d);
	if (error)
		cmd_info(FT_NM, d->av, ERR_FILE);
	error_number(&error);
}