/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   symtype_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: banthony <banthony@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/24 19:17:22 by banthony          #+#    #+#             */
/*   Updated: 2017/12/06 00:12:06 by banthony         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

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

/*
** probleme sur /usr/lib/libmlx.a (section _bss et _data qui saute au parsing)
*/

static uint8_t	get_sect_type64(unsigned char *file, off_t size,
				struct segment_command_64 *sgmt, struct nlist_64 symtable)
{
	uint8_t				n;
	uint8_t					i;
	struct section_64	*sect;

	n = 0;
	sect = NULL;
	if (!sgmt || (unsigned char *)(sgmt + 1) > (file + size))
		return (0);
	while (n < symtable.n_sect)
	{
		i = 1;
		n++;
		sect = (struct section_64 *)(sgmt + 1);
		while (n < symtable.n_sect && i < sgmt->nsects)
		{
			sect++;
			n++;
			i++;
		}
		sgmt = (void*)((unsigned char*)sgmt + sgmt->cmdsize);
	}
	if (sect && !(ft_strcmp(SECT_TEXT, sect->sectname)))
		return ((uint8_t)'T');
	else if (sect && !(ft_strcmp(SECT_DATA, sect->sectname)))
		return ((uint8_t)'D');
	else if (sect && !(ft_strcmp(SECT_BSS, sect->sectname)))
		return ((uint8_t)'B');
	return ((uint8_t)'S');
}

uint8_t			get_symboltype64(t_data *d, struct nlist_64 symtable, uint8_t is_magic)
{
	uint8_t	type;

	if (is_magic)
		;
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
		type = get_sect_type64(d->file, d->stat.st_size, (void*)d->first_sectoff, symtable);
	else if ((symtable.n_type & N_TYPE) == N_INDR)
		type = (uint8_t)'I';
	else
		type = (uint8_t)'?';
	if (!(symtable.n_type & N_EXT))
		type = (uint8_t)ft_tolower((int)type);
	return (type);
}

static uint8_t	get_sect_type32(unsigned char *file, off_t size, struct segment_command *sgmt,
									struct nlist symtable)
{
	uint8_t				n;
	uint8_t					i;
	struct section		*sect;

	n = 0;
	sect = NULL;
	if (!sgmt || (unsigned char *)(sgmt + 1) > (file + size))
		return (0);
	while (n < symtable.n_sect)
	{
		i = 1;
		n++;
		sect = (struct section *)(sgmt + 1);
		while (i < sgmt->nsects && n < symtable.n_sect)
		{
			i++;
			n++;
			sect++;
		}
		sgmt = (void*)((unsigned char*)sgmt + sgmt->cmdsize);
	}
	if (sect && !(ft_strcmp(SECT_TEXT, sect->sectname)))
		return ((uint8_t)'T');
	else if (sect && !(ft_strcmp(SECT_DATA, sect->sectname)))
		return ((uint8_t)'D');
	else if (sect && !(ft_strcmp(SECT_BSS, sect->sectname)))
		return ((uint8_t)'B');
	return ((uint8_t)'S');
}

static uint8_t	get_sect_type32cigam(unsigned char *file, off_t size, struct segment_command *sgmt,
									struct nlist symtable)
{
	uint8_t				n;
	uint8_t					i;
	struct section		*sect;

	n = 0;
	sect = NULL;
	if (!sgmt || (unsigned char *)(sgmt + 1) > (file + size))
		return (0);
	while (n < symtable.n_sect)
	{
		i = 1;
		n++;
		sect = (struct section *)(sgmt + 1);
		while (i < swap_uint32(sgmt->nsects) && n < symtable.n_sect)
		{
			i++;
			n++;
			sect++;
		}
		sgmt = (void*)((unsigned char*)sgmt + swap_uint32(sgmt->cmdsize));
	}
	if (sect && !(ft_strcmp(SECT_TEXT, sect->sectname)))
		return ((uint8_t)'T');
	else if (sect && !(ft_strcmp(SECT_DATA, sect->sectname)))
		return ((uint8_t)'D');
	else if (sect && !(ft_strcmp(SECT_BSS, sect->sectname)))
		return ((uint8_t)'B');
	return ((uint8_t)'S');
}

uint8_t			get_symboltype32(t_data *d, struct nlist symtable, uint8_t is_magic)
{
	struct nlist symt;
	uint8_t	type;

	symt.n_type = symtable.n_type;
	symt.n_sect = symtable.n_sect;
	if (!is_magic)
		symt.n_value = swap_uint32(symtable.n_value);
	else
		symt.n_value = symtable.n_value;
	if ((symt.n_type & N_STAB))
		return ((uint8_t)'-');
	if ((symt.n_type & N_TYPE) == N_UNDF)
	{
		type = (uint8_t)'U';
		if (symt.n_type & N_EXT && symt.n_type & N_PEXT)
			type = (uint8_t)'u';
		if (symt.n_type & N_EXT && symt.n_value)
			type = (uint8_t)'C';
	}
	else if ((symt.n_type & N_TYPE) == N_ABS)
		type = (uint8_t)'A';
	else if ((symt.n_type & N_TYPE) == N_SECT)
	{
		if (!is_magic)
			type = get_sect_type32cigam(d->file, d->stat.st_size, (void*)d->first_sectoff, symt);
		else
			type = get_sect_type32(d->file, d->stat.st_size, (void*)d->first_sectoff, symt);
	}
	else if ((symt.n_type & N_TYPE) == N_INDR)
		type = (uint8_t)'I';
	else
		type = (uint8_t)'?';
	if (!(symt.n_type & N_EXT))
		type = (uint8_t)ft_tolower((int)type);
	return (type);
}
