/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   misc.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: banthony <banthony@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/14 18:33:13 by banthony          #+#    #+#             */
/*   Updated: 2017/12/14 22:30:22 by banthony         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm_otool.h"

/*
**	Affiche le nom du programme suivi de deux char*
**	Exemple d'utilisation: (message d'erreur) ft_nm_info(path, ERROR_PATH)
**	pour notifier sur une erreur de nom de fichier
*/

int			cmd_info(size_t cmd, char *str, char *info)
{
	if (info)
	{
		if (cmd == NM)
			ft_putstr(FT_NM);
		else if (cmd == OTOOL)
			;
		else if (cmd)
			ft_putstr(COMMON);
	}
	ft_putstr(str);
	if (str)
		ft_putstr(":");
	if (info)
		ft_putendl(info);
	else
		ft_putchar('\n');
	return (0);
}

int			is_opt(char *data, char opt)
{
	char	*table;

	if (!data)
		return (0);
	table = ft_strdup(data);
	if (ft_strchr(table, (int)opt))
	{
		ft_strdel(&table);
		return (1);
	}
	ft_strdel(&table);
	return (0);
}

uint32_t	swap_uint32(uint32_t val)
{
	val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF);
	return (val << 16) | (val >> 16);
}

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

void		data_del(void *content, size_t size)
{
	t_data	*d;

	if (!content || !size)
		return ;
	d = (t_data*)content;
	ft_strdel(&d->av);
	ft_lstdel(&d->sym, smb_del);
	if (d->fd > 0 && (close(d->fd) < 0))
		cmd_info(d->token[CMD], d->av, "close error.");
	if (d->file && munmap(d->file, (size_t)d->stat.st_size) < 0)
		cmd_info(d->token[CMD], d->av, "munmap error.");
	ft_memdel((void**)&d);
}

/*
**	Swap sur 32bit - Gestion des MH_CIGAM & FAT_CIGAM
**
**	Exemple sur 0xbebafeca
**
**	shift_1 = (val << 8) & 0xFF00FF00 donne: 0xba00ca00
**
**	  0xbafeca00
**	& 0xFF00FF00
**	------------
**	  0xba00ca00
**
**	shift_2 = (val >> 8) & 0x00FF00FF donne: 0x00be00fe
**
**	  0x00bebafe
**	& 0x00FF00FF
**	------------
**	  0x00be00fe
**
**	(shift_1 | shift_2) donne: 0xbabecafe
**
**	(val << 16) | (val >> 16) donne :	0xcafebabe
**
**	  0xcafe0000
**	| 0x0000babe
**	------------
**	  0xcafebabe
*/

/*
**	Swap sur 64bit - Gestion des FAT_CIGAM_64
**
**	Exemple: 0xbebafeca2afecabe
**	Resultat attendu ==>   0xbecafe2acafebabe
**
**     0xbafe ca2a feca be00	-- << 8
**	&  0xFF00 FF00 FF00 FF00	-- MASK
**	-----------------
**	r1:0xba00 ca00 fe00 be00
**
**     0x00be bafe ca2a feca	-- >> 8
**	&  0x00FF 00FF 00FF 00FF	-- MASK
**	------------------
**	r2:0x00be 00fe 002a 00ca
**
**    0xba00 ca00 fe00 be00	= r1
**	| 0x00be 00fe 002a 00ca	= r2
**	------------------
**	x:0xbabe cafe fe2a beca
**
**
**
**	x1: 0xfe2a beca 0000 0000	= (x << 32)
**	r3:	0xbeca 0000 0000 0000	= (x1 << 16)
**
**	   0x0000 fe2a beca 0000	= (x1 >> 16)
**	&  0x0000 FFFF 0000 0000	= MASK1
**	-----------------------
**	   0x0000 fe2a 0000 0000	= Resultat de ((x1 >> 16) & MASK1)
**	|  0xbeca 0000 0000 0000	= r3
**	-----------------------
**	v1:0xbeca fe2a 0000 0000
**
**
**	x2: 0x0000 0000 babe cafe	= (x >> 32)
**	r4:	0x0000 0000 0000 babe	= (x2 >> 16)
**
**	   0x0000 babe cafe 0000	= (x2 << 16)
**	&  0x0000 0000 FFFF 0000	= MASK2
**	-----------------------
**	   0x0000 0000 cafe 0000	= Resultat de ((x2 << 16) & MASK2)
**	|  0x0000 0000 0000 babe	= r4
**	-----------------------
**	v2:0x0000 0000 cafe babe
**
**	v1:	  0xbeca fe2a 0000 0000
**	v2:	| 0x0000 0000 cafe babe
**		---------------------------
**		  0xbeca fe2a cafe babe
*/
