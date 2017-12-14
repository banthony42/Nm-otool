/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_otool_master.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: banthony <banthony@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/11 21:33:48 by banthony          #+#    #+#             */
/*   Updated: 2017/12/14 23:12:42 by banthony         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm_otool.h"

static int	get_text_section64(struct section_64 section, unsigned char *file,
								off_t size, int is_magic)
{
	struct section_64	sect;

	ft_memcpy(&sect, &section, sizeof(struct section_64));
	if (!is_magic)
	{
		sect.size = swap_uint64(sect.size);
		sect.offset = swap_uint32(sect.offset);
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

static int	get_text_section32(struct section section, unsigned char *file,
								off_t size, int is_magic)
{
	struct section	sect;

	ft_memcpy(&sect, &section, sizeof(struct section));
	if (!is_magic)
	{
		sect.size = swap_uint32(sect.size);
		sect.offset = swap_uint32(sect.offset);
	}
	if (!ft_strncmp(SECT_TEXT, sect.sectname, 16))
	{
		if (sect.size > (uint64_t)size)
			return (1);
		if (is_corrup((void*)(file + sect.offset + sect.size), file, size))
			return (1);
		print_memory32(sect.addr, (void*)(file + sect.offset), sect.size);
	}
	return (0);
}

static int	otool_64magic(unsigned char *file, off_t size,
							struct load_command *lc)
{
	int							i;
	struct segment_command_64	*sgmt;
	struct section_64			*sect;

	sgmt = NULL;
	sect = NULL;
	if (!lc || is_corrup((void*)(lc + 1), file, size))
		return (1);
	while (lc->cmd == LC_SEGMENT_64)
	{
		sgmt = (void*)lc;
		if (is_corrup((void*)(sgmt + 1), file, size))
			return (1);
		i = -1;
		if (is_corrup((void*)((unsigned char *)sgmt + sgmt->cmdsize)
						, file, size))
			return (1);
		sect = (struct section_64*)(sgmt + 1);
		while (++i < (int)sgmt->nsects)
			get_text_section64(sect[i], file, size, 1);
		lc = (void*)((unsigned char*)lc + lc->cmdsize);
	}
	return (0);
}

static int	otool_64cigam(unsigned char *file, off_t size,
							struct load_command *lc)
{
	int							i;
	struct segment_command_64	*sgmt;
	struct section_64			*sect;

	sgmt = NULL;
	sect = NULL;
	if (!lc || is_corrup((void*)(lc + 1), file, size))
		return (1);
	while (swap_uint32(lc->cmd) == LC_SEGMENT_64)
	{
		sgmt = (void*)lc;
		if (is_corrup((void*)(sgmt + 1), file, size))
			return (1);
		i = -1;
		if (is_corrup((void*)((unsigned char *)sgmt + swap_uint32(sgmt->cmdsize))
						, file, size))
			return (1);
		sect = (struct section_64*)(sgmt + 1);
		while (++i < (int)swap_uint32(sgmt->nsects))
			get_text_section64(sect[i], file, size, 0);
		lc = (void*)((unsigned char*)lc + swap_uint32(lc->cmdsize));
	}
	return (0);
}

static int	otool_32cigam(unsigned char *file, off_t size,
							struct load_command *lc)
{
	int						i;
	struct segment_command	*sgmt;
	struct section			*sect;

	sgmt = NULL;
	sect = NULL;
	if (!lc || is_corrup((void*)(lc + 1), file, size))
		return (1);
	while (swap_uint32(lc->cmd) == LC_SEGMENT)
	{
		sgmt = (void*)lc;
		if (is_corrup((void*)(sgmt + 1), file, size))
			return (1);
		i = -1;
		if (is_corrup((void*)((unsigned char *)sgmt + swap_uint32(sgmt->cmdsize))
						, file, size))
			return (1);
		sect = (struct section*)(sgmt + 1);
		while (++i < (int)swap_uint32(sgmt->nsects))
			get_text_section32(sect[i], file, size, 0);
		lc = (void*)((unsigned char*)lc + swap_uint32(lc->cmdsize));
	}
	return (0);
}

static int	otool_32magic(unsigned char *file, off_t size,
							struct load_command *lc)
{
	int						i;
	struct segment_command	*sgmt;
	struct section			*sect;

	sgmt = NULL;
	sect = NULL;
	if (!lc || is_corrup((void*)(lc + 1), file, size))
		return (1);
	while (lc->cmd == LC_SEGMENT)
	{
		sgmt = (void*)lc;
		if (is_corrup((void*)(sgmt + 1), file, size))
			return (1);
		i = -1;
		if (is_corrup((void*)((unsigned char *)sgmt + sgmt->cmdsize)
						, file, size))
			return (1);
		sect = (struct section*)(sgmt + 1);
		while (++i < (int)sgmt->nsects)
			get_text_section32(sect[i], file, size, 1);
		lc = (void*)((unsigned char*)lc + lc->cmdsize);
	}
	return (0);
}

void		ft_otool(t_data *d, unsigned char *file, off_t size, uint32_t arch)
{
	int error;

	error = 0;
	if (!d || !arch || !d->file)
		return ;
	if (d->token[TYPE] == MACHO)
		cmd_info(0, d->av, NULL);
	if (arch == ARCH32)
		error = otool_32magic(file, size, (void*)d->first_sectoff);
	else if (arch == ARCH64)
		error = otool_64magic(file, size, (void*)d->first_sectoff);
	else if (arch == swap_uint32(ARCH32))
		error = otool_32cigam(file, size, (void*)d->first_sectoff);
	else if (arch == swap_uint32(ARCH64))
		error = otool_64cigam(file, size, (void*)d->first_sectoff);
	if (error)
		cmd_info(d->token[CMD], d->av, ERR_FILE);
	error_number(&error);
}
