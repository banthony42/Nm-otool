/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   misc3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: banthony <banthony@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/08 16:20:42 by banthony          #+#    #+#             */
/*   Updated: 2017/12/11 13:38:43 by banthony         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

void	nm_display(t_data *d)
{
	t_smb *tmp;

	tmp = (t_smb*)d->sym;
	while (d->sym)
	{
		ft_memcpy(((t_smb*)d->sym->content)->options, d->opt, NB_OPTIONS);
		d->sym = d->sym->next;
	}
	d->sym = (t_list*)tmp;
	d->lst_browser(d->sym, nm_output);
}

int		ft_islower(int c)
{
	if (c >= (int)'a' && c <= (int)'z')
		return (1);
	return (0);
}

/*
**	Fonction temporaire, Affichage de la liste t_data
*/

void	print_elem(t_list *elem)
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
