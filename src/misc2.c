/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_manage2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: banthony <banthony@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/30 16:08:29 by banthony          #+#    #+#             */
/*   Updated: 2017/11/30 20:06:42 by banthony         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

t_arch arch_data[ARCH_DATA_SIZE] =
{
	{"x86_64"	,	CPU_TYPE_X86_64		,	0},
	{"i386"		,	CPU_TYPE_I386		,	0},
	{"arm"		,	CPU_TYPE_ARM		,	0},
	{"arm64"	,	CPU_TYPE_ARM64		,	0},
	{"ppc"		,	CPU_TYPE_POWERPC	,	0},
	{"ppc64"	,	CPU_TYPE_POWERPC64	,	0},
};

void		print_arch(struct fat_arch frh, t_data *d, int mgc)
{
	int			i;
	uint32_t	cpu;

	i = 0;
	if (mgc == -1)
		cpu = ~CPU_ARCH_MASK & (swap_uint32((uint32_t)frh.cputype));
	else
		cpu = ~CPU_ARCH_MASK & ((uint32_t)frh.cputype);
	ft_putstr(d->av);
	ft_putstr(" (architecture ");
	while (i < ARCH_DATA_SIZE)
	{
		if (cpu ==  (uint32_t)arch_data[i].cputype)
			ft_putstr(arch_data[i].name);
		i++;
	}
	ft_putendl("):");
}

void		lstiter_reverse(t_list *lst, void (*f)(t_list *elem))
{
	if (lst)
		lstiter_reverse(lst->next, f);
	if (lst)
		f(lst);
}

void		smb_del(void *content, size_t size)
{
	t_smb *tmp;

	if (!content || !size)
		return ;
	tmp = (t_smb *)content;
	ft_strdel(&tmp->name);
	ft_strdel(&tmp->value);
	ft_memdel((void **)&tmp);
}

/*
**Fonction temporaire
*/

void		print_elem(t_list *elem)
{
	t_data*h;

	h = (t_data*)elem->content;
	if (h->token == OPTION)
		ft_putstr("OPTION\t");
	if (h->token == PATH)
		ft_putstr("FILE\t");
	ft_putstr(h->av);
	ft_putstr("\tfd ");
	ft_putnbr(h->fd);
	ft_putstr("\tdlen: ");
	ft_putnbr(h->data_len);
	ft_putchar('\t');
	ft_putstr("\topt ");
	ft_print_memory(h->opt, NB_OPTIONS);
}
