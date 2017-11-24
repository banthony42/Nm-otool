/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   symtype_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: banthony <banthony@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/24 19:17:22 by banthony          #+#    #+#             */
/*   Updated: 2017/11/24 19:53:25 by banthony         ###   ########.fr       */
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

static uint8_t	get_sect_type(struct segment_command_64 *sgmt,
									struct nlist_64 symtable)
{
	uint8_t				n;
	int					i;
	struct section_64	*sect;

	n = 0;
	sect = NULL;
	while (sgmt->cmd == LC_SEGMENT_64)
	{
		i = -1;
		sect = (struct section_64 *)(sgmt + 1);
		while (++n < symtable.n_sect && (uint8_t)++i < sgmt->nsects)
			sect++;
		if (n == symtable.n_sect)
			break ;
		sgmt = (void*)((unsigned char*)sgmt + sgmt->cmdsize);
	}
	if (sect && !(ft_strcmp(SECT_TEXT, sect->sectname)))
		return ((uint8_t)'T');
	else if (sect && !(ft_strcmp(SECT_DATA, sect->sectname)))
		return ((uint8_t)'D');
	else if (sect && !(ft_strcmp(SECT_BSS, sect->sectname)))
		return ((uint8_t)'B');
	else if (sect && !(ft_strcmp(SECT_COMMON, sect->sectname)))
		return ((uint8_t)'C');
	return ((uint8_t)'S');
}

uint8_t			get_symboltype64(t_data *d, struct nlist_64 symtable)
{
	uint8_t	type;

	if ((symtable.n_type & N_STAB))
		return ((uint8_t)'?');
	if ((symtable.n_type & N_TYPE) == N_UNDF)
		type = (uint8_t)'U';
	else if ((symtable.n_type & N_TYPE) == N_ABS)
		type = (uint8_t)'A';
	else if ((symtable.n_type & N_TYPE) == N_SECT)
		type = get_sect_type((void*)d->first_sectoff, symtable);
	else if ((symtable.n_type & N_TYPE) == N_INDR)
		type = (uint8_t)'I';
	else
		type = (uint8_t)'?';
	return (type);
}
