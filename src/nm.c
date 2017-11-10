/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: banthony <banthony@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/13 18:56:31 by banthony          #+#    #+#             */
/*   Updated: 2017/11/10 21:23:25 by banthony         ###   ########.fr       */
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

static int		arch_64_cigam(uint32_t ncmds, t_data *d, unsigned char *file, off_t size)
{
	int error;

	error = -1;
	printf("ncmds swapped:%d\n", ncmds);
	if (ncmds && file && size && d)
		;
	if (error <= 0)
		return (error);
	return (1);
}

static int		symtab_handler_64(struct symtab_command *sym, t_data *d, unsigned char *file, off_t size)
{
	uint8_t			type;
	uint32_t		i;
	char			*strtable;
	char			*value;
	struct nlist_64	*symtable;

	i = 0;
	if (d)
		;
	if ((unsigned char *)(sym + 1) > (file + size))
		return (0);
	strtable = (void *)(file + sym->stroff);
	symtable = (void *)(file + sym->symoff);
	if ((unsigned char *)(strtable + sym->strsize) > (file + size))
		return (0);
	if ((unsigned char *)(symtable + 1) > (file + size))
		return (0);
	while (i < sym->nsyms)
	{
		// Creation maillon
		// Insertion new maillon avec fonction de tri (ptr_fct)
		value = itoa_base_uint64(symtable[i].n_value, 16);
		if (!value || !ft_strcmp(value, PADD_ZERO))
			ft_putstr(PADD_SPACE);
		else
			ft_putstr(value);
		ft_putchar(' ');
		type = symtable[i].n_type & N_TYPE;
		if (type == N_UNDF)
			ft_putstr("U ");
		else if (type == N_ABS)
			ft_putstr("A ");
		else if (type == N_SECT)
			ft_putstr("T ");
		else if (type == N_PBUD)
			ft_putstr("? ");
		else if (type == N_INDR)
			ft_putstr("I ");
		ft_putendl(strtable + symtable[i].n_un.n_strx);
		ft_strdel(&value);
		i++;
	}
	return (1);
}

static int		arch_64_magic(uint32_t ncmds, t_data *d, unsigned char *file, off_t size)
{
	int error;
	uint32_t i;
	struct load_command		*lc;

	i = 0;
	error = 1;
	lc = (void *)(file + sizeof(struct mach_header_64));
	while (i < ncmds)
	{
		if ((unsigned char *)(lc + 1) > (file + size))
			return (0);
		if (lc->cmd == LC_SYMTAB)
			error = symtab_handler_64((void *)lc, d, file, size);
		if (error <= 0)
			return (error);
		lc = (void *)((unsigned char*)lc + lc->cmdsize);
		i++;
	}
	return (error);
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
	//Affichage
	//Delete de t_list en fin de main, car possible sortit par return(error)
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
	if ((magic = *(uint32_t*)d->file) == MH_MAGIC_64 || magic == MH_CIGAM_64)
		error = arch_64_handler(magic, d, d->file, d->stat.st_size);
	else if (magic == MH_MAGIC || magic == MH_CIGAM)
		error = arch_32_handler(magic, d, d->file, d->stat.st_size);
	else if (!(ft_strncmp(ARMAG, (char*)d->file, SARMAG)))
		error = archive_handler(d);
	else if (magic == FAT_MAGIC || magic == FAT_CIGAM)
		error = fat_arch_32_handler(magic, d, (unsigned char*)d->file, d->stat.st_size);
	else
		ft_putendlcol(YELLOW, "NM: UNKNOW MAGIC");
	if (error <= 0)
		error_str(d->av, ERR_FILE);
}

/*
**	error < 0: Erreur sur magic_number ;
**	error == 0: Erreur lors de la lecture fichier
**	ARMAG/SARMAG - Correspond au magic_str et size_magic_str - voir ar.h
*/
