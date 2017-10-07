/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: banthony <banthony@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/06 20:11:56 by banthony          #+#    #+#             */
/*   Updated: 2017/10/07 21:08:33 by banthony         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

void		print_elem(t_list *elem)
{
	t_data	*h;

	h = (t_data*)elem->content;
	if (h->token == OPTION)
		ft_putstr("OPTION\t: ");
	if (h->token == PATH)
		ft_putstr("FILE\t: ");
	ft_putstr(h->av);
	ft_putstr("\tfd: ");
	ft_putnbr(h->fd);
	ft_putstr("\topt: ");
	ft_print_memory(h->opt, 8);
}

int		error_str(char *str, char *error)
{
	ft_putstr("nm: ");
	ft_putstr(str);
	ft_putstr(": ");
	ft_putendl(error);
	return (0);
}

static void	fill_option(t_list **l, t_list **last, t_data **d)
{
	t_data	*tmp;
	char	*ptr;
	char	*tab;
	int		i;

	tmp = (t_data*)(*l)->content;
	if (tmp->token != OPTION)
		return ;
	tab = ft_strdup(AVAILABLE_OPTIONS);
	tab[8] = '\0';
	i = 0;
	while (tmp->av[++i])
	{
		if (!(ptr = ft_strchr(tab, (int)tmp->av[i])))
			continue ;
		(*d)->opt[ptr - &tab[0]] = tmp->av[i];
	}
	ft_strdel(&tab);
	if (!*last)
		return ;
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
		fill_option(&l, &last, &d);
		last = l;
		if (l)
			l = l->next;
	}
	last = *lst;
	ft_lstadd(lst, ft_lstnew(d, sizeof(t_data)));
	(*lst)->next = last->next;
	ft_lstdelone(&last, &data_del);
}

int				main(int ac, char **av)
{
	t_list *entry;

	if (!(entry = parsing(ac, av)))
		ft_putstr(NM_USG);
	else
	{
		ft_putendl("--------- NM -------------");
		ft_lstiter(entry, &print_elem);
		ft_putendl("--------------------------");
		concat_options(&entry);
		ft_putendl("--------- NM -------------");
		ft_lstiter(entry, &print_elem);
		ft_putendl("--------------------------");
	}
/*			ATTENTION !!!
**	Desactiver fsanitize pour tester les leaks
system("leaks ft_nm");*/
	return (0);
}
