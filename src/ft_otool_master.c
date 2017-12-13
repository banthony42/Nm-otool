/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_otool_master.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: banthony <banthony@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/11 21:33:48 by banthony          #+#    #+#             */
/*   Updated: 2017/12/13 21:52:23 by banthony         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm_otool.h"

static int	otool_64magic(unsigned char *file, off_t size, struct load_command *lc)
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
		if (is_corrup((void*)((unsigned char *)sgmt + sgmt->cmdsize), file, size))
			return (1);
		sect = (struct section_64*)(sgmt + 1);
		while (++i < (int)sgmt->nsects)
		{
			if (!ft_strncmp(SECT_TEXT, sect[i].sectname, 16))
			{
				if (sect->size > (uint64_t)size)
					return (1);
				if (is_corrup((void*)(file + sect->offset + sect->size), file, size))
					return (1);
				ft_print_memory((void*)(file + sect->offset), sect->size);
				return (0);
			}
		}
		lc = (void*)((unsigned char*)lc + lc->cmdsize);
	}
	return (0);
}

void	ft_otool(t_data *d, unsigned char *file, off_t size, uint32_t arch)
{
	int error;

	error = 1;
	if (!d || !arch)
		return ;
	if (arch == ARCH32)
		ft_putendlcol(YELLOW, "_otool_32magic");
	else if (arch == ARCH64)
		error = otool_64magic(file, size, (void*)d->first_sectoff);
	else if (arch == swap_uint32(ARCH32))
		ft_putendlcol(YELLOW, "_otool_32cigam");
	else if (arch == swap_uint32(ARCH64))
		ft_putendlcol(YELLOW, "_otool_64cigam");
	if (error)
		cmd_info(FT_OTOOL, d->av, ERR_FILE);
	error_number(&error);
}















