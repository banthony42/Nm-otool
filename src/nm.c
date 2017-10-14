/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: banthony <banthony@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/13 18:56:31 by banthony          #+#    #+#             */
/*   Updated: 2017/10/14 18:27:25 by banthony         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static void	arch_64_handler(void *file)
{
	if (file)
		ft_putendl("Arch_64");
}

static void	arch_32_handler(void *file)
{
	if (file)
		ft_putendl("Arch_32");
}

void		ft_nm(t_list *elem)
{
	unsigned int	magic;
	t_data			*d;

	if ((d = (t_data*)elem->content)->token != PATH || !d->file)
		return ;
	magic = *(unsigned int*)d->file;
	if (magic == MH_MAGIC_64)
		arch_64_handler(d->file);
	else if (magic == MH_MAGIC)
		arch_32_handler(d->file);
}
