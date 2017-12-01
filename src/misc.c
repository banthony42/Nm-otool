/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   misc.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: banthony <banthony@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/14 18:33:13 by banthony          #+#    #+#             */
/*   Updated: 2017/12/02 00:42:40 by banthony         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

/*
**	Affiche le nom du programme suivi de deux char*
**	Exemple d'utilisation: (message d'erreur) ft_nm_info(path, ERROR_PATH)
**	pour notifier sur une erreur de nom de fichier
*/

int			ft_nm_info(char *str, char *info)
{
	if (info)
		ft_putstr("ft_nm: ");
	ft_putstr(str);
	if (str)
		ft_putstr(":");
	if (info)
		ft_putendl(info);
	else
		ft_putchar('\n');
	return (0);
}

int			is_opt(void *data, char opt)
{
	t_data	*d;
	char	*table;

	if (!data)
		return (-1);
	d = (t_data *)data;
	table = ft_strdup(d->opt);
	if (ft_strchr(table, (int)opt))
	{
		ft_strdel(&table);
		return (1);
	}
	ft_strdel(&table);
	return (0);
}

/*
**	142 - 0x8e - 1000 1110
**  232 - 0xe8 - 1110 1000
**
*/

uint8_t	swap_uint8(uint8_t val)
{
	val = ((val << 4) & 0xFF) | ((val >> 4) & 0xFF);
	return (val);
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
	if (d->fd > 0 && (close(d->fd) < 0))
		ft_nm_info(d->av, "close error.");
	if (d->file && munmap(d->file, (size_t)d->stat.st_size) < 0)
		ft_nm_info(d->av, "munmap error.");
	ft_strdel(&d->av);
	ft_lstdel(&d->sym, smb_del);
	ft_memdel((void**)&d);
}

/*Swap x32 - Gestion des MH_CIGAM & FAT_CIGAM*/
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
