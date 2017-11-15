/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: banthony <banthony@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/06 20:11:56 by banthony          #+#    #+#             */
/*   Updated: 2017/11/15 19:20:28 by banthony         ###   ########.fr       */
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
	while (*lst)
	{
		((t_data*)(*lst)->content)->data_len = (unsigned int)ft_lstlen(l);
		*lst = (*lst)->next;
	}
	*lst = l;
}

void		nm_output(t_list *elem)
{
	t_smb	*tmp;

	if (!(tmp = (t_smb*)elem->content))
		return ;
	if (!tmp->value || !ft_strcmp(tmp->value, PADD_ZERO))
		ft_putstr(PADD_SPACE);
	else
		ft_putstr(tmp->value);
	ft_putchar(' ');
	if (tmp->type == N_UNDF)
		ft_putstr("U ");
	else if (tmp->type == N_ABS)
		ft_putstr("A ");
	else if (tmp->type == N_SECT)
		ft_putstr("T ");
	else if (tmp->type == N_PBUD)
		ft_putstr("* ");
	else if (tmp->type == N_INDR)
		ft_putstr("I ");
	else
		ft_putstr("? ");
	ft_putendl(tmp->name);
}

static void		display(t_list *elem)
{
	t_data *d;

	if (!(d = (t_data*)elem->content))
		return ;
	if (!d->lst_browser)	/*Comportement par defaut, si le ptr_func vaut NULL*/
		ft_lstiter(d->sym, nm_output);
	else
		d->lst_browser(d->sym, nm_output);	/*Sens du parcours, Voir option nm -r*/
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
	//BEGIN PARSING VIEWER
	ft_putstr(GREEN);
	ft_lstiter(entry, print_elem);
	ft_putstr(WHITE);
	ft_putstr("\n\n");
	//END PARSING VIEWER
	ft_lstiter(entry, &ft_nm);		/*Recup des data*/
	ft_lstiter(entry, &display);	/*Affichage des data*/
	ft_lstdel(&entry, data_del);	/*Liberation de la memoire*/
/*			ATTENTION !!!
**	Desactiver fsanitize pour tester les leaks
*/
//	system("leaks ft_nm");
	return (EXIT_SUCCESS);
}
