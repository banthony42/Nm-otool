/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   misc.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: banthony <banthony@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/14 18:33:13 by banthony          #+#    #+#             */
/*   Updated: 2017/10/15 20:18:56 by banthony         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

/*
**	Exemple sur 0xbebafeca
**
**	(val << 8) & 0xFF00FF00
**	  0xbafeca00
**	& 0xFF00FF00
**	------------
**	  0xba00ca00
**
**	(val >> 8) & 0x00FF00FF
**	  0x00bebafe
**	& 0x00FF00FF
**	------------
**	  0x00be00fe
**
**	=> 0xbabecafe
**	(val << 16) | (val >> 16)
**		0xcafe0000
**	| 0x0000babe
**		-----------
**		0xcafebabe
*/
uint32_t	swap_uint32(uint32_t val)
{
	val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF);
	return (val << 16) | (val >> 16);
}

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
