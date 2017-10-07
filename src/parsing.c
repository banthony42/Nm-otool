/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: banthony <banthony@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/07 18:44:27 by banthony          #+#    #+#             */
/*   Updated: 2017/10/07 20:36:33 by banthony         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

void		data_del(void *content, size_t size)
{
	int		ret;
	t_data	*d;

	if (!content || !size)
		return ;
	d = (t_data*)content;
	if (d->fd < 0 && (ret = close(d->fd)) < 0)
		error_str(d->av, "close error");
	ft_strdel(&d->av);
	ft_memdel((void**)&d);
}

static int		option_error_handler(t_list **lst)
{
	int		i;
	t_data	*d;
	t_list	*l;

	l = *lst;
	while (l)
	{
		i = -1;
		d = (t_data*)l->content;
		if (d->token == OPTION)
		{
			while (d->av[++i])
				if (!(ft_strchr(AVAILABLE_OPTIONS, (int)d->av[i])))
					return (error_str(d->av, UNKNOWN_OPTION));
		}
		l = l->next;
	}
	return (1);
}

static void	prepare_files(t_list *elm)
{
	int		ret;
	t_data	*d;

	if (!elm)
		return ;
	d = (t_data*)elm->content;
	if (d->token == PATH)
	{
		if ((d->fd = open(d->av, O_RDONLY)) < 0)
			error_str(d->av, FILE_NOT_FOUND);
		else if ((ret = fstat(d->fd, &d->stat)) < 0)
			error_str(d->av, FSTAT_ERROR);
		else if (!(d->stat.st_mode & S_IRUSR))
			error_str(d->av, CANT_READ);
	}
}

static t_data	*new_data(char *str, int *wait)
{
	t_data *d;

	d = NULL;
	if (!str)
		return (NULL);
	if (!(d = (t_data*)malloc(sizeof(t_data))))
		return (NULL);
	ft_bzero(d, sizeof(t_data));
	d->av = ft_strdup(str);
	d->token = PATH;
	if (str[0] == '-' && *wait)
		d->token = OPTION;
	if (!(ft_strcmp(str, "--")))
		*wait = 0;
	return (d);
}

t_list	*parsing(int ac, char **av)
{
	t_list	*lst;
	int		i;
	int		wait;
	t_data	*h;

	i = 0;
	lst = NULL;
	wait = 1;
	h = NULL;
	if (!av || ac < 2)
		return (NULL);
	while (av[++i])
	{
		if (!(h = new_data(av[i], &wait)))
			continue ;
		if (!lst)
			lst = ft_lstnew((void*)h, sizeof(t_data));
		else
			ft_lstaddback(&lst, ft_lstnew(h, sizeof(t_data)));
		ft_memdel((void**)&h);
	}
	if (!(option_error_handler(&lst)))
		ft_lstdel(&lst, &data_del);
	ft_lstiter(lst, &prepare_files);
	return (lst);
}
