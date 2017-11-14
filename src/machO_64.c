/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   machO_64.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: banthony <banthony@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/14 17:24:36 by banthony          #+#    #+#             */
/*   Updated: 2017/11/14 19:28:31 by banthony         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

int		arch_64_cigam(uint32_t ncmds, t_data *d, unsigned char *file, off_t size)
{
	int error;

	error = -1;
	printf("ncmds swapped:%d\n", ncmds);
	if (ncmds && file && size && d)
		;
	if (error <= 0)
		return (error);
	return (1);
}

static t_list	*create_symbol_list(t_data *d, struct nlist_64 *symtable, uint32_t i, char *strtable)
{
	t_smb *tmp;

	if (!(tmp = (t_smb*)ft_memalloc(sizeof(t_smb))))
		return(NULL);
	if (!(tmp->name = ft_strdup(strtable + symtable[i].n_un.n_strx)))	/*Recup symbol name*/
		return (NULL);
	if (!(tmp->value = itoa_base_uint64(symtable[i].n_value, 16)))		/*Recup symbol value*/
		return (NULL);
	tmp->type = symtable[i].n_type & N_TYPE;
	if (!d->sym)
	{
		if (!(d->sym = ft_lstnew((void*)tmp, sizeof(t_smb))))
			return (NULL);
	}
	else
		ft_lstaddback(&d->sym, ft_lstnew((void*)tmp, sizeof(t_smb)));	/*A modifier suivant les options*/
	ft_memdel((void**)&tmp);
	return (d->sym);
}

/*Dupliquer pour cigam_64 ?*/
static int		symtab_handler_64(struct symtab_command *sym, t_data *d, unsigned char *file, off_t size)
{
	uint32_t		i;
	char			*strtable;
	struct nlist_64	*symtable;

	i = 0;
	if ((unsigned char *)(sym + 1) > (file + size))
		return (0);
	strtable = (void *)(file + sym->stroff);
	symtable = (void *)(file + sym->symoff);
	if ((unsigned char *)(strtable + sym->strsize) > (file + size))
		return (0);
	if ((unsigned char *)(symtable + 1) > (file + size))
		return (0);
	while (i < sym->nsyms)
	{
		if (!(create_symbol_list(d, symtable, i,strtable)))
			return (0);
		i++;
	}
	return (1);
}

int		arch_64_magic(uint32_t ncmds, t_data *d, unsigned char *file, off_t size)
{
	int error;
	uint32_t i;
	struct load_command		*lc;

	i = 0;
	error = 1;
	lc = (void *)(file + sizeof(struct mach_header_64));
	while (i < ncmds)
	{
		if ((unsigned char *)(lc + 1) > (file + size))
			return (0);
		if (lc->cmd == LC_SYMTAB)
			error = symtab_handler_64((void *)lc, d, file, size);
		if (error <= 0)
			return (error);
		lc = (void *)((unsigned char*)lc + lc->cmdsize);
		i++;
	}
	return (error);
}
