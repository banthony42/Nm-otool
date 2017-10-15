/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: banthony <banthony@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/13 18:56:31 by banthony          #+#    #+#             */
/*   Updated: 2017/10/15 18:39:51 by banthony         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static void	arch_64_handler(t_data *data)
{
	if (!data)
		return ;
	ft_putendl("Arch_64");
}

static void	arch_32_handler(t_data *data)
{
	if (!data)
		return ;
	ft_putendl("Arch_32");
}

static void archive_handler(t_data *d)
{
	off_t size;
	off_t	i;
	struct ar_hdr *h;
	void	*ptr;
	unsigned int magic;

	if (!d)
		return ;
	i = SARMAG;
	size = 0;
	ptr = d->file;
	ft_putendl("Archive");
	ft_putendlcol(GREEN, "-------------MAGIC----------------");
	ft_print_memory(ptr, SARMAG);
	ft_putendlcol(GREEN, "------------------------------------");
	ft_putnbr(i);
	ft_putchar('\n');
	while (((char*)ptr - (char*)d->file) < d->stat.st_size)
	{
		size += i;
		ptr = (void *)((char *)ptr + i);
		h = (struct ar_hdr *)ptr;
		ft_putendlcol(GREEN, "-------------ELEMENT----------------");
		ft_print_memory(ptr, sizeof(struct ar_hdr));
		magic = *(unsigned int*)(void*)((char*)ptr + sizeof(struct ar_hdr));
		if (magic == MH_MAGIC_64)
			ft_putendlcol(YELLOW, "Arch64");
		ft_putendlcol(GREEN, "------------------------------------");
		i = (off_t)((sizeof(struct ar_hdr)) + (size_t)ft_atoi(h->ar_size));
		ft_putnbr(i);
		ft_putchar('\n');
	}
	ft_putnbr(size);
	ft_putchar('\n');
}

void		ft_nm(t_list *elem)
{
	unsigned int	magic;
	t_data			*d;

	if (!elem || (d = (t_data*)elem->content)->token != PATH || !d->file)
		return ;
	printf("ar_hdr:%lu\n", sizeof(struct ar_hdr));
	ft_putendlcol(BLUE, d->av);
	if ((magic = *(unsigned int*)d->file) == MH_MAGIC_64)
		arch_64_handler(d);
	else if (magic == MH_MAGIC)
		arch_32_handler(d);
	else if (!(ft_strncmp(ARMAG, (char*)d->file, SARMAG)))
		archive_handler(d);
	/*ARMAG/SARMAG - Correspond au magic_str et size_magic_str - voir ar.h - */
}

