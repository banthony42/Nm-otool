/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: banthony <banthony@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/06 20:11:56 by banthony          #+#    #+#             */
/*   Updated: 2017/10/15 23:10:32 by banthony         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static void	fill_option(t_list **lst, t_list **l, t_list **last, t_data **d)
{
	t_data	*tmp;
	char	*ptr;
	char	*tab;
	int		i;

	if (!*l || (tmp = (t_data*)(*l)->content)->token != OPTION)
		return ;
	tab = ft_strdup(AVAILABLE_OPTIONS);
	i = 0;
	tab[8] = '\0';
	while (tmp->av[++i])
		if ((ptr = ft_strchr(tab, (int)tmp->av[i])))
			(*d)->opt[ptr - &tab[0]] = tmp->av[i];
	ft_strdel(&tab);
	if (!*last)
	{
		*lst = (*l)->next;
		ft_lstdelone(l, &data_del);
		*l = *lst;
		fill_option(lst, l, last, d);
		return ;
	}
	(*last)->next = (*l)->next;
	ft_lstdelone(l, &data_del);
	*l = *last;
}

static void	concat_options(t_list **lst)
{
	t_list	*l;
	t_list	*last;
	t_data	*d;

	l = *lst;
	last = NULL;
	if (!(d = (t_data*)malloc((sizeof(t_data)))))
		return ;
	ft_bzero(d, sizeof(t_data));
	d->token = OPTION;
	ft_memset(d->opt, '-', 8);
	while (l)
	{
		fill_option(lst, &l, &last, &d);
		if ((last = l))
			l = l->next;
	}
	ft_lstadd(lst, ft_lstnew(d, sizeof(t_data)));
	data_del(d, sizeof(t_data));
}

int			main(int ac, char **av)
{
	t_list *entry;

	entry = NULL;
	if (ac < 2)
	{
		concat_options(&entry);
		default_file(&entry);
		ft_lstiter(entry, &prepare_files);
	}
	else if ((entry = parsing(av)))
		concat_options(&entry);
	else
	{
		ft_putendl(NM_USG);
		return (EXIT_FAILURE);
	}
	ft_lstiter(entry, &print_elem);
	ft_putchar('\n');
	ft_lstiter(entry, &ft_nm);
/*			ATTENTION !!!
**	Desactiver fsanitize pour tester les leaks
system("leaks ft_nm");*/
	return (EXIT_SUCCESS);
}
