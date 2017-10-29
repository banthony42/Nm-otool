/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   misc.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: banthony <banthony@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/14 18:33:13 by banthony          #+#    #+#             */
/*   Updated: 2017/10/18 21:06:49 by banthony         ###   ########.fr       */
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
/*Swap x32 - Gestion des MH_CIGAM & FAT_CIGAM*/
uint32_t	swap_uint32(uint32_t val)
{
	val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF);
	return (val << 16) | (val >> 16);
}

/*Swap x64 - Gestion des FAT_CIGAM_64*/
/*
Exemple: 0xbebafeca2afecabe
Resultat attendu ==>   0xbecafe2acafebabe

  0xbafeca2afecabe00	-- << 8
& 0xFF00FF00FF00FF00	-- MASK
-----------------
r1 0xba00ca00fe00be00

  0x00bebafeca2afeca	-- >> 8
& 0x00FF00FF00FF00FF	-- MASK
------------------
r2 0x00be00fe002a00ca

  0xba00ca00fe00be00	-- r1
| 0x00be00fe002a00ca	-- r2
------------------
x:  0xbabe cafe fe2a beca

	OMG *_*

x1: 0xfe2a beca 0000 0000	-- x << 32
r3:	0xbeca 0000 0000 0000	-- x1 << 16

	  0x0000 fe2a beca 0000	-- x1 >> 16
	& 0x0000 FFFF 0000 0000	-- MASK
	-----------------------
	  0x0000 fe2a 0000 0000
	| 0xbeca 0000 0000 0000	-- r3
	-----------------------
v1:	  0xbeca fe2a 0000 0000

	WTF T_T

x2: 0x0000 0000 babe cafe	-- x >> 32
r4:	0x0000 0000 0000 babe	-- x2 >> 16


	  0x0000 babe cafe 0000	-- x2 << 16
	& 0x0000 0000 FFFF 0000	-- MASK
	-----------------------
	  0x0000 0000 cafe 0000
	| 0x0000 0000 0000 babe	-- r4
	-----------------------
v2:	  0x0000 0000 cafe babe

v1:	  0xbeca fe2a 0000 0000
v2:	| 0x0000 0000 cafe babe
---------------------------
	  0xbeca fe2a cafe babe

	  :'D
*/
uint64_t	swap_uint64(uint64_t val)
{
	uint64_t v1;
	uint64_t v2;

	val = ((val << 8) & 0xFF00FF00FF00FF00) | ((val >> 8) & 0xFF00FF00FF00FF);
	v1 = val << 32;
	v1 = ((v1 << 16) | ((v1 >> 16) & 0xFFFF00000000));
	v2 = val >> 32;
	v2 = (((v2 << 16) & 0xFFFF0000) | (v2 >> 16));
	return (v1 | v2);
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
	ft_print_memory(h->opt, NB_OPTIONS);
}

int			error_str(char *str, char *error)
{
	ft_putstr("ft_nm: ");
	ft_putstr(str);
	ft_putstr(": ");
	ft_putendl(error);
	return (0);
}
