/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   macho_64magic.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: banthony <banthony@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/01 22:42:18 by banthony          #+#    #+#             */
/*   Updated: 2017/12/18 18:32:49 by banthony         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm_otool.h"

/*
**	strndup securise:
**	Au maximum sur n caracteres, et verifie que la chaine est dans le fichier
*/

char			*ft_strndup(char *s1, uint32_t n, void *file, off_t size)
{
	char	*ptr;
	size_t	len;

	len = 0;
	while (s1[len] && len < n)
		len++;
	if (is_corrup((void*)(s1 + len), file, size))
		return (NULL);
	ptr = NULL;
	if (s1)
	{
		if (!(ptr = (char *)malloc((len + 1) * sizeof(char))))
			return (NULL);
		ft_strncpy(ptr, s1, len);
		ptr[len] = '\0';
	}
	return (ptr);
}

static t_list	*create_symbol_list64(t_data *d, struct nlist_64 symt,
										char *strtable, uint32_t strtable_size)
{
	t_list	*l;
	t_smb	*tmp;

	if (!(tmp = (t_smb*)ft_memalloc(sizeof(t_smb))))
		return (NULL);
	if (!(tmp->name = ft_strndup(strtable + symt.n_un.n_strx,
			strtable_size - symt.n_un.n_strx, d->file, d->stat.st_size)))
		return (NULL);
	if (!(tmp->type = get_symboltype64magic(d, symt)))
		return (NULL);
	tmp->arch = ARCH64;
	if (tmp->name[0] != '\0')
	{
		l = ft_lstnew((void*)tmp, sizeof(t_smb));
		((t_smb*)l->content)->name = ft_strdup(tmp->name);
		if (!(((t_smb*)l->content)->value =
				itoa_base_uint64(symt.n_value, 16)))
			return (NULL);
		list_builder(&d, l);
	}
	ft_strdel(&tmp->name);
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
	strtable = (void *)(file + sym->stroff);
	symtable = (void *)(file + sym->symoff);
	if (is_corrup((void *)(strtable + sym->strsize), file, size))
		return (1);
	if (is_corrup((void *)(symtable + sym->nsyms), file, size))
		return (1);
	while (i < sym->nsyms)
	{
		if (is_corrup((void*)(strtable + symtable[i].n_un.n_strx), file, size))
			return (1);
		if (!(create_symbol_list64(d, symtable[i], strtable, sym->strsize)))
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

int				arch_64_magic(uint32_t ncmds, t_data *d, unsigned char *file,
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
		if (is_corrup((void*)(lc + 1), file, size) || (lc->cmdsize % 8))
			return (1);
		if (lc->cmd == LC_SEGMENT_64 && !d->first_sectoff)
			error = get_first_sectoff(d, (void*)lc, file, size);
		if (!error && lc->cmd == LC_SYMTAB)
			error = symtab_handler_64((void *)lc, d, file, size);
		if (error)
			return (error);
		lc = (void *)((unsigned char*)lc + lc->cmdsize);
	}
	(d->token[CMD] == NM) ? (nm_display(d)) : (ft_otool(d, file, size, ARCH64));
	ft_lstdel(&d->sym, smb_del);
	d->first_sectoff = NULL;
	return (error);
}
