/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   macho_32cigam.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: banthony <banthony@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/01 22:41:24 by banthony          #+#    #+#             */
/*   Updated: 2017/12/13 21:00:32 by banthony         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm_otool.h"

static t_list	*create_symbol_list32(t_data *d, struct nlist symtable,
									char *strtable, uint32_t strtable_size)
{
	t_list	*l;
	t_smb	*tmp;

	l = NULL;
	if (!(tmp = (t_smb *)ft_memalloc(sizeof(t_smb))))
		return (NULL);
	if (!(tmp->name = ft_strndup(strtable + swap_uint32(symtable.n_un.n_strx),
							strtable_size - swap_uint32(symtable.n_un.n_strx),
							d->file, d->stat.st_size)))
		return (NULL);
	if (!(tmp->type = get_symboltype32cigam(d, symtable)))
		return (NULL);
	tmp->arch = ARCH32;
	if (tmp->name[0] != '\0')
	{
		l = ft_lstnew((void*)tmp, sizeof(t_smb));
		((t_smb*)l->content)->name = ft_strdup(tmp->name);
		if (!(((t_smb*)l->content)->value =
						itoa_base_uint32(swap_uint32(symtable.n_value), 16)))
			return (NULL);
		list_builder(&d, l);
	}
	ft_strdel(&tmp->name);
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
	if (is_corrup((void *)(sym + 1), file, size))
		return (1);
	strtable = (void *)(file + swap_uint32(sym->stroff));
	symtable = (void *)(file + swap_uint32(sym->symoff));
	if (is_corrup((void *)(strtable + swap_uint32(sym->strsize)), file, size))
		return (1);
	if (is_corrup((void *)(symtable + swap_uint32(sym->nsyms)), file, size))
		return (1);
	while (i < swap_uint32(sym->nsyms))
	{
		if (is_corrup((void*)(strtable + swap_uint32(symtable[i].n_un.n_strx)),
				file, size))
			return (1);
		if (!(create_symbol_list32(d, symtable[i], strtable,
				swap_uint32(sym->strsize))))
			return (0);
		i++;
	}
	return (0);
}

static int		get_first_sectoff(t_data *d, struct segment_command *sgmt,
									unsigned char *file, off_t size)
{
	if (is_corrup((void *)(sgmt + 2), file, size))
		return (1);
	if (!ft_strncmp(SEG_PAGEZERO, sgmt->segname, 16))
		d->first_sectoff = (void*)(sgmt + 1);
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
		if (is_corrup((void *)(lc + 1), file, size))
			return (1);
		if (swap_uint32(lc->cmd) == LC_SEGMENT && !d->first_sectoff)
			error = get_first_sectoff(d, (void*)lc, file, size);
		if (!error && swap_uint32(lc->cmd) == LC_SYMTAB)
			error = symtab_handler_32cigam((void*)lc, d, file, size);
		if (error)
			return (error);
		lc = (void*)((unsigned char *)lc + swap_uint32(lc->cmdsize));
	}
	(d->token[CMD] == NM) ? (nm_display(d)) : (ft_otool(d, file, size, swap_uint32(ARCH32)));
	ft_lstdel(&d->sym, smb_del);
	d->first_sectoff = NULL;
	return (0);
}
