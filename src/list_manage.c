/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_manage.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: banthony <banthony@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/17 17:29:26 by banthony          #+#    #+#             */
/*   Updated: 2017/11/24 19:35:02 by banthony         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

/*
**	Si previous == NULL => (ajout en tete de liste)
**		New devient le premier maillon et begin le second
**	Si previous != NULL => (ajout avant l'elmt begin)
**		New est ajouter avant la ou en est le pointeur begin
*/

static void	lst_swap(t_list **begin, t_list *previous, t_list *new)
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

void		lstadd_alpha(t_list **begin, t_list *new)
{
	t_list	*mem[2];
	t_smb	*e[2];

	mem[0] = *begin;
	mem[1] = NULL;
	if (!(e[0] = (t_smb*)new->content) || !(e[1] = (t_smb*)mem[0]->content))
		return ;
	if (ft_strcmp(e[1]->name, e[0]->name) > 0)
	{
		lst_swap(begin, mem[1], new);
		return ;
	}
	while (mem[0])
	{
		if (!(e[1] = (t_smb*)mem[0]->content))
			return ;
		if (ft_strcmp(e[1]->name, e[0]->name) > 0)
		{
			lst_swap(&mem[0], mem[1], new);
			return ;
		}
		mem[1] = mem[0];
		mem[0] = mem[0]->next;
	}
	ft_lstaddback(begin, new);
}

void		lstadd_numeric(t_list **begin, t_list *new)
{
	t_list	*mem[2];
	t_smb	*e[2];

	mem[0] = *begin;
	mem[1] = NULL;
	if (!(e[0] = (t_smb*)new->content) || !(e[1] = (t_smb*)mem[0]->content))
		return ;
	if (ft_strcmp(e[1]->value, e[0]->value) > 0)
	{
		lst_swap(begin, mem[1], new);
		return ;
	}
	while (mem[0])
	{
		if (!(e[1] = (t_smb*)mem[0]->content))
			return ;
		if (ft_strcmp(e[1]->value, e[0]->value) > 0)
		{
			lst_swap(&mem[0], mem[1], new);
			return ;
		}
		mem[1] = mem[0];
		mem[0] = mem[0]->next;
	}
	ft_lstaddback(begin, new);
}

void		lstiter_reverse(t_list *lst, void (*f)(t_list *elem))
{
	if (lst)
		lstiter_reverse(lst->next, f);
	if (lst)
		f(lst);
}

void		smb_del(void *content, size_t size)
{
	t_smb *tmp;

	if (!content || !size)
		return ;
	tmp = (t_smb *)content;
	ft_strdel(&tmp->name);
	ft_strdel(&tmp->value);
	ft_memdel((void **)&tmp);
}
