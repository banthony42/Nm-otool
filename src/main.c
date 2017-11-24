/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: banthony <banthony@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/06 20:11:56 by banthony          #+#    #+#             */
/*   Updated: 2017/11/24 19:17:17 by banthony         ###   ########.fr       */
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
	tab[NB_OPTIONS] = '\0';
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
	ft_memset(d->opt, '-', NB_OPTIONS);
	while (l)
	{
		fill_option(lst, &l, &last, &d);
		if ((last = l))
			l = l->next;
	}
	ft_lstadd(lst, ft_lstnew(d, sizeof(t_data)));
	data_del(d, sizeof(t_data));
	l = *lst;
	while (l)
	{
		((t_data*)(l)->content)->data_len = (unsigned int)ft_lstlen(*lst);
		l = (l)->next;
	}
}

void		nm_output(t_list *elem)
{
	t_smb	*tmp;

	if (!(tmp = (t_smb*)elem->content) || tmp->type == (uint8_t)'?')
		return ;
	if (tmp->type == (uint8_t)'U')
		ft_putstr(PADD_SPACE);
	else
		ft_putstr(tmp->value);
	ft_putchar(' ');
	ft_putchar((char)tmp->type);
	ft_putchar(' ');
	ft_putendl(tmp->name);
}

static void	option_analyse(t_list **entry)
{
	t_data	*one;
	t_data	*d;
	t_list	*l;

	if (!(one = (t_data*)(*entry)->content))
		return ;
	one->lst_browser = &(ft_lstiter);
	one->lstadd_somewhere = &(lstadd_alpha);
	if (is_opt(one, 'r') && !(is_opt(one, 'p')))
		one->lst_browser = &(lstiter_reverse);
	else if (is_opt(one, 'n'))
		one->lstadd_somewhere = &(lstadd_numeric);
	else if (is_opt(one, 'p'))
		one->lstadd_somewhere = &(ft_lstaddback);
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
	else if ((entry = parsing(av)))
		concat_options(&entry);
	else
	{
		ft_putendl(NM_USG);
		return (EXIT_FAILURE);
	}
	option_analyse(&entry);
	ft_lstiter(entry, &ft_nm);		/*Recup des data*/
	ft_lstdel(&entry, data_del);	/*Liberation de la memoire*/
//	system("leaks ft_nm");
	return (EXIT_SUCCESS);
}














