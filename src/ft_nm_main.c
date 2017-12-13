/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: banthony <banthony@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/11 13:45:08 by banthony          #+#    #+#             */
/*   Updated: 2017/12/13 16:40:40 by banthony         ###   ########.fr       */
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
	tab = ft_strdup(AVAILABLE_OPT_NM);
	i = 0;
	tab[NB_OPT_NM] = '\0';
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
	if (!(d = (t_data*)ft_memalloc((sizeof(t_data)))))
		return ;
	d->token[TYPE] = OPTION;
	ft_memset(d->opt, '-', NB_OPT_NM);
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
		ft_memcpy(((t_data*)(l)->content)->opt, d->opt, NB_OPT_NM);
		((t_data*)(l)->content)->data_len = (unsigned int)ft_lstlen(*lst);
		l = (l)->next;
	}
	data_del(d, sizeof(t_data));
}

/*
**	Enregistrement des ordres demandes  dans le maillon one. (temporaire)
**	Parcours de la liste data, ajout des ordres dans chaque maillons.
*/

static void	option_analyse(t_list **entry)
{
	t_data	*one;
	t_data	*d;
	t_list	*l;

	if (!(one = (t_data*)(*entry)->content))
		return ;
	one->lst_browser = &(ft_lstiter);
	one->lstadd_somewhere = &(lstadd_alpha);
	if (is_opt(one->opt, 'r') && !(is_opt(one->opt, 'p')))
		one->lst_browser = &(lstiter_reverse);
	if (is_opt(one->opt, 'p'))
		one->lstadd_somewhere = &(ft_lstaddback);
	else if (is_opt(one->opt, 'n'))
		one->lstadd_somewhere = &(lstadd_numeric);
	l = *entry;
	while ((*entry))
	{
		if ((d = (t_data*)(*entry)->content))
		{
			d->lst_browser = one->lst_browser;
			d->lstadd_somewhere = one->lstadd_somewhere;
		}
		(*entry) = (*entry)->next;
	}
	*entry = l;
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
	else if ((entry = parsing(av, FT_NM, AVAILABLE_OPT_NM)))
		concat_options(&entry);
	else
	{
		ft_putendl(NM_USG);
		return (EXIT_FAILURE);
	}
	option_analyse(&entry);
	ft_lstiter(entry, &ft_nm_otool);
	ft_lstdel(&entry, data_del);
	system("leaks ft_nm");
	return (*(error_number(NULL)));
}
