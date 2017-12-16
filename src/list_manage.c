/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_manage.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: banthony <banthony@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/17 17:29:26 by banthony          #+#    #+#             */
/*   Updated: 2017/12/16 18:54:37 by banthony         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm_otool.h"

/*
**	Si previous == NULL => (ajout en tete de liste)
**		New devient le premier maillon et begin le second
**	Si previous != NULL => (ajout avant l'elmt begin)
**		New est ajouter avant la ou en est le pointeur begin
*/

static void		lst_swap(t_list **begin, t_list *previous, t_list *new)
{
	t_list	*tmp;

	tmp = *begin;
	if (previous == NULL)
	{
		new->next = *begin;
		*begin = new;
		return ;
	}
	else
	{
		previous->next = new;
		new->next = tmp;
	}
}

/*
**	Insertion par tri lexicographique
*/

static int		add_compare_name(t_smb **e, t_list **begin,
								t_list *mem[2], t_list *new)
{
	if (ft_strcmp(e[1]->name, e[0]->name) > 0)
	{
		if (begin)
			lst_swap(begin, mem[1], new);
		else
			lst_swap(&mem[0], mem[1], new);
		return (0);
	}
	else if (ft_strcmp(e[1]->name, e[0]->name) == 0)
	{
		if (ft_strcmp(e[1]->value, e[0]->value) > 0)
		{
			if (begin)
				lst_swap(begin, mem[1], new);
			else
				lst_swap(&mem[0], mem[1], new);
			return (0);
		}
	}
	return (1);
}

void			lstadd_alpha(t_list **begin, t_list *new)
{
	t_list	*mem[2];
	t_smb	*e[2];

	mem[0] = *begin;
	mem[1] = NULL;
	if (!(e[0] = (t_smb*)new->content)
		|| !(e[1] = (t_smb*)mem[0]->content))
		return ;
	if (!add_compare_name(e, begin, mem, new))
		return ;
	while (mem[0])
	{
		if (!(e[1] = (t_smb*)mem[0]->content))
			return ;
		if (!add_compare_name(e, NULL, mem, new))
			return ;
		mem[1] = mem[0];
		mem[0] = mem[0]->next;
	}
	ft_lstaddback(begin, new);
}

static int		add_compare_value(t_smb **e, t_list **begin,
								t_list *mem[2], t_list *new)
{
	if (ft_strcmp(e[1]->value, e[0]->value) > 0)
	{
		if (begin)
			lst_swap(begin, mem[1], new);
		else
			lst_swap(&mem[0], mem[1], new);
		return (0);
	}
	else if (ft_strcmp(e[1]->value, e[0]->value) == 0)
	{
		if (e[0]->type == 'U' && e[1]->type != 'U')
		{
			if (begin)
				lst_swap(begin, mem[1], new);
			else
				lst_swap(&mem[0], mem[1], new);
			return (0);
		}
		else
		{
			if (!add_compare_name(e, NULL, mem, new))
				return (0);
		}
	}
	return (1);
}

void			lstadd_numeric(t_list **begin, t_list *new)
{
	t_list	*mem[2];
	t_smb	*e[2];

	mem[0] = *begin;
	mem[1] = NULL;
	if (!(e[0] = (t_smb*)new->content)
		|| !(e[1] = (t_smb*)mem[0]->content))
		return ;
	if (!add_compare_value(e, begin, mem, new))
		return ;
	while (mem[0])
	{
		if (!(e[1] = (t_smb*)mem[0]->content))
			return ;
		if (!add_compare_value(e, NULL, mem, new))
			return ;
		mem[1] = mem[0];
		mem[0] = mem[0]->next;
	}
	ft_lstaddback(begin, new);
}
