/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   symtype_handler32.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: banthony <banthony@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/07 21:27:39 by banthony          #+#    #+#             */
/*   Updated: 2017/12/15 22:23:32 by banthony         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm_otool.h"

/*
**	U  (undefined),  A  (absolute),  T  (text section symbol),
**	D (data section symbol), B (bss section symbol), C (common symbol),
**	S (symbol in  a  section  other than  those above),
**	or I (indirect symbol). If the symbol is local (non-external),
**	the symbol's type is instead represented by the corresponding
**	lowercase letter. A lower case u  in  a  dynamic  shared library
**	indicates a undefined reference to a private external in another
**	module in the same library.
*/

static uint8_t	compare_sect(struct section *sect, unsigned char *file,
								off_t size)
{
	if (is_corrup((void *)(sect + 1), file, size))
		return (0);
	if (sect && !(ft_strcmp(SECT_TEXT, sect->sectname)))
		return ((uint8_t)'T');
	else if (sect && !(ft_strcmp(SECT_DATA, sect->sectname)))
		return ((uint8_t)'D');
	else if (sect && !(ft_strcmp(SECT_BSS, sect->sectname)))
		return ((uint8_t)'B');
	else
		return ((uint8_t)'S');
}

static uint8_t	get_sect_type32magic(unsigned char *file, off_t size,
					struct segment_command *sgmt, struct nlist symtable)
{
	uint8_t				n;
	uint8_t				i;
	struct section		*sect;

	n = 0;
	sect = NULL;
	if (!sgmt || is_corrup((void *)(sgmt + 1), file, size))
		return (0);
	while (n < symtable.n_sect)
	{
		i = 1;
		n++;
		sect = (struct section *)(sgmt + 1);
		if (is_corrup((void *)(sgmt + 1), file, size))
			return (0);
		while (i < sgmt->nsects && n < symtable.n_sect)
		{
			i++;
			n++;
			sect++;
		}
		sgmt = (void*)((unsigned char*)sgmt + sgmt->cmdsize);
	}
	i = compare_sect(sect, file, size);
	return (i);
}

uint8_t			get_symboltype32magic(t_data *d, struct nlist symtable)
{
	uint8_t	type;

	if ((symtable.n_type & N_STAB))
		return ((uint8_t)'-');
	if ((symtable.n_type & N_TYPE) == N_UNDF)
	{
		type = (uint8_t)'U';
		if (symtable.n_type & N_EXT && symtable.n_type & N_PEXT)
			type = (uint8_t)'u';
		if (symtable.n_type & N_EXT && symtable.n_value)
			type = (uint8_t)'C';
	}
	else if ((symtable.n_type & N_TYPE) == N_ABS)
		type = (uint8_t)'A';
	else if ((symtable.n_type & N_TYPE) == N_SECT)
		type = get_sect_type32magic(d->file, d->stat.st_size,
							(void*)d->first_sectoff, symtable);
	else if ((symtable.n_type & N_TYPE) == N_INDR)
		type = (uint8_t)'I';
	else
		type = (uint8_t)'?';
	if (!(symtable.n_type & N_EXT))
		type = (uint8_t)ft_tolower((int)type);
	return (type);
}

static uint8_t	get_sect_type32cigam(unsigned char *file, off_t size,
					struct segment_command *sgmt, struct nlist symtable)
{
	uint8_t				n;
	uint8_t				i;
	struct section		*sect;

	n = 0;
	sect = NULL;
	if (!sgmt || is_corrup((void *)(sgmt + 1), file, size))
		return (0);
	while (n < symtable.n_sect)
	{
		i = 1;
		n++;
		sect = (struct section *)(sgmt + 1);
		if (is_corrup((void *)(sgmt + 1), file, size))
			return (0);
		while (i < swap_uint32(sgmt->nsects) && n < symtable.n_sect)
		{
			i++;
			n++;
			sect++;
		}
		sgmt = (void*)((unsigned char*)sgmt + swap_uint32(sgmt->cmdsize));
	}
	i = compare_sect(sect, file, size);
	return (i);
}

uint8_t			get_symboltype32cigam(t_data *d, struct nlist symtable)
{
	uint8_t	type;

	if ((symtable.n_type & N_STAB))
		return ((uint8_t)'-');
	if ((symtable.n_type & N_TYPE) == N_UNDF)
	{
		type = (uint8_t)'U';
		if (symtable.n_type & N_EXT && symtable.n_type & N_PEXT)
			type = (uint8_t)'u';
		if (symtable.n_type & N_EXT && swap_uint32(symtable.n_value))
			type = (uint8_t)'C';
	}
	else if ((symtable.n_type & N_TYPE) == N_ABS)
		type = (uint8_t)'A';
	else if ((symtable.n_type & N_TYPE) == N_SECT)
		type = get_sect_type32cigam(d->file, d->stat.st_size,
							(void*)d->first_sectoff, symtable);
	else if ((symtable.n_type & N_TYPE) == N_INDR)
		type = (uint8_t)'I';
	else
		type = (uint8_t)'?';
	if (!(symtable.n_type & N_EXT))
		type = (uint8_t)ft_tolower((int)type);
	return (type);
}
