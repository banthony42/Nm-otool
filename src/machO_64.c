/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   macho_64.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: banthony <banthony@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/24 19:36:17 by banthony          #+#    #+#             */
/*   Updated: 2017/11/25 18:24:22 by banthony         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

int				arch_64_cigam(uint32_t ncmds, t_data *d, unsigned char *file,
							off_t size)
{
	int error;

	error = -1;
	printf("ncmds swapped:%d\n", ncmds);
	if (ncmds && file && size && d)
		;
	if (error)
		return (error);
	return (0);
}

static t_list	*create_symbol_list(t_data *d, struct nlist_64 symtable,
												char *strtable)
{
	t_smb	*tmp;

	if (!(tmp = (t_smb*)ft_memalloc(sizeof(t_smb))))
		return (NULL);
	if (!(tmp->name = ft_strdup(strtable + symtable.n_un.n_strx)))
		return (NULL);
	if (!(tmp->value = itoa_base_uint64(symtable.n_value, 16)))
		return (NULL);
	if (!(tmp->type = get_symboltype64(d, symtable)))
		return (NULL);
	if (tmp->name[0] != '\0')
	{
		if (!d->sym)
			d->sym = ft_lstnew((void*)tmp, sizeof(t_smb));
		else
			d->lstadd_somewhere(&d->sym, ft_lstnew((void*)tmp, sizeof(t_smb)));
	}
	ft_memdel((void**)&tmp);
	return (d->sym);
}

static int		symtab_handler_64(struct symtab_command *sym, t_data *d,
										unsigned char *file, off_t size)
{
	uint32_t		i;
	char			*strtable;
	struct nlist_64	*symtable;

	i = 0;
	if ((unsigned char *)(sym + 1) > (file + size))
		return (1);
	strtable = (void *)(file + sym->stroff);
	symtable = (void *)(file + sym->symoff);
	if ((unsigned char *)(strtable + sym->strsize) > (file + size))
		return (1);
	if ((unsigned char *)(symtable + 1) > (file + size))
		return (1);
	while (i < sym->nsyms)
	{
		if (!(create_symbol_list(d, symtable[i], strtable)))
			return (1);
		i++;
	}
	return (0);
}

int				arch_64_magic(uint32_t ncmds, t_data *d, unsigned char *file,
									off_t size)
{
	int							error;
	uint32_t					i;
	struct load_command			*lc;
	struct segment_command_64	*sgmt64;

	i = 0;
	error = 0;
	lc = (void *)(file + sizeof(struct mach_header_64));
	while (i < ncmds)
	{
		if ((unsigned char *)(lc + 1) > (file + size))
			return (1);
		if (lc->cmd == LC_SEGMENT_64 && !d->first_sectoff)
		{
			sgmt64 = (struct segment_command_64 *)(void*)lc;
			if (!ft_strcmp(SEG_PAGEZERO, sgmt64->segname))
			{
				if ((unsigned char *)(sgmt64 + 1) > (file + size))
					return (1);
				d->first_sectoff = (void*)(sgmt64 + 1);
			}
			else
				d->first_sectoff = (void*)(sgmt64);
		}
		if (lc->cmd == LC_SYMTAB)
			error = symtab_handler_64((void *)lc, d, file, size);
		if (error)
			return (error);
		lc = (void *)((unsigned char*)lc + lc->cmdsize);
		i++;
	}
	d->lst_browser(d->sym, nm_output);
	ft_lstdel(&d->sym, smb_del);
	return (error);
}








