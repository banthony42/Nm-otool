/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_otool_main.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: banthony <banthony@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/11 13:45:34 by banthony          #+#    #+#             */
/*   Updated: 2017/12/11 22:19:20 by banthony         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm_otool.h"

static void	fill_option(t_list **lst, t_list **l, t_list **last, t_data **d)
{
	t_data	*tmp;
	char	*ptr;
	char	*tab;
	int		i;

	if (!*l || (tmp = (t_data*)(*l)->content)->token[TYPE] != OPTION)
		return ;
	tab = ft_strdup(AVAILABLE_OPT_OTOOL);
	i = 0;
	tab[NB_OPT_OTOOL] = '\0';
	while (tmp->av[++i])
		if ((ptr = ft_strchr(tab, (int)tmp->av[i])))
			(*d)->opt[ptr - tab] = tmp->av[i];
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
	if (!lst || !(d = (t_data*)ft_memalloc((sizeof(t_data)))))
		return ;
	d->token[TYPE] = OPTION;
	ft_memset(d->opt, '-', NB_OPT_OTOOL);
	while (l)
	{
		fill_option(lst, &l, &last, &d);
		if ((last = l))
			l = l->next;
	}
	ft_lstadd(lst, ft_lstnew(d, sizeof(t_data)));
	l = *lst;
	while (l)
	{
		ft_memcpy(((t_data*)(l)->content)->opt, d->opt, NB_OPT_OTOOL);
		((t_data*)(l)->content)->data_len = (unsigned int)ft_lstlen(*lst);
		l = (l)->next;
	}
	data_del(d, sizeof(t_data));
}

static int	lst_nbfile(t_list *lst)
{
	int n;

	n = 0;
	while (lst)
	{
		if (((t_data*)lst->content)->token[TYPE] == PATH)
			n++;
		lst = lst->next;
	}
	return (n);
}

/*
**	Appel a ft_nm qui remplis les structures t_data.
**	La liste de t_smb est deja delete lors du debut de ft_otool.
**	Ce qui nous interresse c'est la variable d->first_sectoff.
*/

int			main(int ac, char **av)
{
	t_list	*entry;

	entry = NULL;
	if (ac < 2)
	{
		ft_putendl(OTOOL_USG);
		return (EXIT_FAILURE);
	}
	entry = parsing(av, FT_OTOOL, AVAILABLE_OPT_OTOOL);
	concat_options(&entry);
	if (!entry || !lst_nbfile(entry))
	{
		ft_putendl(OTOOL_USG);
		return (EXIT_FAILURE);
	}
	ft_lstiter(entry, &ft_nm_otool);
	ft_lstdel(&entry, data_del);
	return (*(error_number(NULL)));
}