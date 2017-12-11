/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   macho_64cigam.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: banthony <banthony@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/01 22:42:10 by banthony          #+#    #+#             */
/*   Updated: 2017/12/11 22:11:15 by banthony         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm_otool.h"

static t_list	*create_symbol_list64(t_data *d, struct nlist_64 symtable,
									char *strtable, uint32_t strtable_size)
{
	t_smb	*tmp;

	if (!(tmp = (t_smb*)ft_memalloc(sizeof(t_smb))))
		return (NULL);
	if (!(tmp->name = ft_strndup(strtable + swap_uint32(symtable.n_un.n_strx),
	strtable_size - swap_uint32(symtable.n_un.n_strx),
	d->file, d->stat.st_size)))
		return (NULL);
	if (!(tmp->value = itoa_base_uint64(swap_uint64(symtable.n_value), 16)))
		return (NULL);
	if (!(tmp->type = get_symboltype64magic(d, symtable)))
		return (NULL);
	tmp->arch = ARCH64;
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
	if (is_corrup((void *)(sym + 1), file, size))
		return (1);
	strtable = (void *)(file + swap_uint64(sym->stroff));
	symtable = (void *)(file + swap_uint64(sym->symoff));
	if (is_corrup((void *)(strtable + swap_uint64(sym->strsize)), file, size))
		return (1);
	if (is_corrup((void *)(symtable + swap_uint64(sym->nsyms)), file, size))
		return (1);
	while (i < swap_uint64(sym->nsyms))
	{
		if (is_corrup((void*)(strtable + swap_uint32(symtable[i].n_un.n_strx)),
				file, size))
			return (1);
		if (!(create_symbol_list64(d, symtable[i], strtable,
					swap_uint32(sym->strsize))))
			return (1);
		i++;
	}
	return (0);
}

static int		get_first_sectoff(t_data *d, struct segment_command_64 *sgmt64,
												unsigned char *file, off_t size)
{
	if (is_corrup((void *)(sgmt64 + 2), file, size))
		return (1);
	if (!ft_strncmp(SEG_PAGEZERO, sgmt64->segname, 16))
		d->first_sectoff = (void*)(sgmt64 + 1);
	else
		d->first_sectoff = (void*)(sgmt64);
	return (0);
}

int				arch_64_cigam(uint32_t ncmds, t_data *d, unsigned char *file,
									off_t size)
{
	int							error;
	int							i;
	struct load_command			*lc;

	i = -1;
	error = 0;
	lc = (void *)(file + sizeof(struct mach_header_64));
	while (++i < (int)ncmds)
	{
		if (is_corrup((void*)(lc + 1), file, size))
			return (1);
		if (swap_uint64(lc->cmd) == LC_SEGMENT_64 && !d->first_sectoff)
			error = get_first_sectoff(d, (void*)lc, file, size);
		if (!error && swap_uint64(lc->cmd) == LC_SYMTAB)
			error = symtab_handler_64((void *)lc, d, file, size);
		if (error)
			return (error);
		lc = (void *)((unsigned char*)lc + swap_uint64(lc->cmdsize));
	}
	if (d->token[CMD] == NM)
		nm_display(d);
	ft_lstdel(&d->sym, smb_del);
	d->first_sectoff = NULL;
	return (error);
}
