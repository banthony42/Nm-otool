/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   section_text_handler.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: banthony <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/15 22:32:57 by banthony          #+#    #+#             */
/*   Updated: 2017/12/15 22:39:01 by banthony         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm_otool.h"

int	get_text_section64(struct section_64 section, unsigned char *file,
								off_t size, int is_magic)
{
	struct section_64	sect;

	ft_memcpy(&sect, &section, sizeof(struct section_64));
	if (!is_magic)
	{
		sect.size = swap_uint64(sect.size);
		sect.offset = swap_uint32(sect.offset);
		sect.addr = swap_uint64(sect.addr);
	}
	if (!ft_strncmp(SECT_TEXT, sect.sectname, 16))
	{
		if (sect.size > (uint64_t)size)
			return (1);
		if (is_corrup((void*)(file + sect.offset + sect.size), file, size))
			return (1);
		print_memory64(sect.addr, (void*)(file + sect.offset), sect.size);
	}
	return (0);
}

int	get_text_section32(struct section section, unsigned char *file,
								off_t size, int is_magic)
{
	struct section	sect;

	ft_memcpy(&sect, &section, sizeof(struct section));
	if (is_magic < 0)
	{
		sect.size = swap_uint32(sect.size);
		sect.offset = swap_uint32(sect.offset);
		sect.addr = swap_uint32(sect.addr);
	}
	if (!ft_strncmp(SECT_TEXT, sect.sectname, 16))
	{
		if (sect.size > (uint64_t)size)
			return (1);
		if (is_corrup((void*)(file + sect.offset + sect.size), file, size))
			return (1);
		print_memory32(sect.addr, (void*)(file + sect.offset)
						, sect.size, is_magic);
	}
	return (0);
}
