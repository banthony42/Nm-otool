/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   macho_32cigam.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: banthony <banthony@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/01 22:41:24 by banthony          #+#    #+#             */
/*   Updated: 2017/12/06 00:14:42 by banthony         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static t_list	*create_symbol_list32(t_data *d, struct nlist symtable,
												char *strtable)
{
	t_smb *tmp;

	if (!(tmp = (t_smb *)ft_memalloc(sizeof(t_smb))))
		return (NULL);
	if (!(tmp->name = ft_strdup(strtable + swap_uint32(symtable.n_un.n_strx))))
		return (NULL);
	if (!(tmp->value = itoa_base_uint32(swap_uint32(symtable.n_value), 16)))
		return (NULL);
	if (!(tmp->type = get_symboltype32(d, symtable, 0)))
		return (NULL);
	tmp->arch = ARCH32;
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

static int		symtab_handler_32cigam(struct symtab_command *sym, t_data *d,
										unsigned char *file, off_t size)
{
	uint32_t		i;
	char			*strtable;
	struct nlist	*symtable;

	i = 0;
	if ((unsigned char *)(sym + 1) > (file + size))
		return (1);
	strtable = (void *)(file + swap_uint32(sym->stroff));
	symtable = (void *)(file + swap_uint32(sym->symoff));
	if ((unsigned char *)(symtable + 1) > (file + size))
		return (1);
	while (i < swap_uint32(sym->nsyms))
	{
		if (!(create_symbol_list32(d, symtable[i], strtable)))
			return (0);
		i++;
	}
	return (0);
}

static int		get_first_sectoff_cigam(t_data *d, struct segment_command *sgmt,
									unsigned char *file, off_t size)
{
	if (!ft_strcmp(SEG_PAGEZERO, sgmt->segname))
	{
		if ((unsigned char *)(sgmt + 1) > (file + size))
			return (1);
		d->first_sectoff = (void*)(sgmt + 1);
	}
	else
		d->first_sectoff = (void*)(sgmt);
	return (0);
}

int				arch_32_cigam(uint32_t ncmds, t_data *d,
								unsigned char *file, off_t size)
{
	int					i;
	int					error;
	struct load_command	*lc;

	i = -1;
	error = 0;
	lc = (void *)(file + sizeof(struct mach_header));
	while (++i < (int)ncmds)
	{
		if ((unsigned char *)(lc + 1) > (file + size))
			return (1);
		if (swap_uint32(lc->cmd) == LC_SEGMENT && !d->first_sectoff)
		{
			if (get_first_sectoff_cigam(d, (void*)lc, file, size))
				return (1);
		}
		if (swap_uint32(lc->cmd) == LC_SYMTAB)
			error = symtab_handler_32cigam((void*)lc, d, file, size);
		if (error)
			return (error);
		lc = (void*)((unsigned char *)lc + swap_uint32(lc->cmdsize));
	}
	d->lst_browser(d->sym, nm_output);
	d->first_sectoff = NULL;
	ft_lstdel(&d->sym, smb_del);
	return (0);
}










