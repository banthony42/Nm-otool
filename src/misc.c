/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   misc.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: banthony <banthony@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/14 18:33:13 by banthony          #+#    #+#             */
/*   Updated: 2017/10/14 18:34:28 by banthony         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

void			data_del(void *content, size_t size)
{
	t_data	*d;

	if (!content || !size)
		return ;
	d = (t_data*)content;
	if (d->fd > 0 && (close(d->fd) < 0))
		error_str(d->av, "close error.");
	if (d->file && munmap(d->file, (size_t)d->stat.st_size) < 0)
		error_str(d->av, "munmap error.");
	ft_strdel(&d->av);
	ft_memdel((void**)&d);
}

void		print_elem(t_list *elem)
{
	t_data	*h;

	h = (t_data*)elem->content;
	if (h->token == OPTION)
		ft_putstr("OPTION\t");
	if (h->token == PATH)
		ft_putstr("FILE\t");
	ft_putstr(h->av);
	ft_putstr("\tfd ");
	ft_putnbr(h->fd);
	ft_putstr("\topt ");
	ft_print_memory(h->opt, 8);
}

int			error_str(char *str, char *error)
{
	ft_putstr("ft_nm: ");
	ft_putstr(str);
	ft_putstr(": ");
	ft_putendl(error);
	return (0);
}
