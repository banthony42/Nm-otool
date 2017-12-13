/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   misc3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: banthony <banthony@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/08 16:20:42 by banthony          #+#    #+#             */
/*   Updated: 2017/12/13 16:32:00 by banthony         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm_otool.h"

void	list_builder(t_data **d, t_list *l)
{
	if (!(*d)->sym)
		(*d)->sym = l;
	else
		(*d)->lstadd_somewhere(&(*d)->sym, l);
}

int		*error_number(int *err)
{
	static int error;

	if (err)
		error = *err;
	return (&error);
}

int		ft_islower(int c)
{
	if (c >= (int)'a' && c <= (int)'z')
		return (1);
	return (0);
}

void	nm_output(t_list *elem)
{
	t_smb	*tmp;

	if (!(tmp = (t_smb*)(elem->content)) || (tmp->type == '?')
										|| (tmp->type == '-'))
		return ;
	if (is_opt(tmp->options, 'U') && ft_strchr("UuCc", tmp->type))
		return ;
	if (is_opt(tmp->options, 'u') && !ft_strchr("UuCc", tmp->type))
		return ;
	if (is_opt(tmp->options, 'g') && ft_islower((int)tmp->type))
		return ;
	if (!is_opt(tmp->options, 'u') && !is_opt(tmp->options, 'j'))
	{
		if ((tmp->type == 'U') && tmp->arch == ARCH64)
			ft_putstr(PADD_SPACE64);
		else if ((tmp->type == 'U') && tmp->arch == ARCH32)
			ft_putstr(PADD_SPACE32);
		else
			ft_putstr(tmp->value);
		ft_putchar(' ');
		ft_putchar((char)tmp->type);
		ft_putchar(' ');
	}
	ft_putendl(tmp->name);
}

/*
**	Fonction temporaire, Affichage de la liste t_data
*/

void	print_elem(t_list *elem)
{
	t_data*h;

	h = (t_data*)elem->content;
	if (h->token[TYPE] == OPTION)
		ft_putstr("OPTION\t");
	if (h->token[TYPE] == PATH)
		ft_putstr("FILE\t");
	ft_putstr(h->av);
	ft_putstr("\tfd ");
	ft_putnbr(h->fd);
	ft_putstr("\tdlen: ");
	ft_putnbr(h->data_len);
	ft_putchar('\t');
	ft_putstr("\topt ");
	ft_print_memory(h->opt, NB_OPT_NM);
}
