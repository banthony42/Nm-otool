/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: banthony <banthony@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/13 18:56:31 by banthony          #+#    #+#             */
/*   Updated: 2017/10/17 16:26:38 by banthony         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static void	arch_64_handler(void *file, char *name)
{
	struct mach_header_64 *hdr64;

	if (!file)
		return ;
	if (name)
	{
		ft_putstr(name);
		ft_putstr(":\n");
	}
	ft_putendlcol(YELLOW, "Arch_64");
	hdr64 = (struct mach_header_64 *)file;
	if (hdr64->filetype == MH_OBJECT)
		ft_putendl("OBJECT");
}

static void	arch_32_handler(void *file, char *name)
{
	if (!file)
		return ;
	if (name)
	{
		ft_putstr(name);
		ft_putstr(":\n");
	}
	ft_putendlcol(RED, "Arch_32");
}

static off_t	extract_ar_name(t_data *d, struct ar_hdr *h)
{
	char	*str;
	char	*name;
	char	nb[16];
	off_t	n;

	n = 2;
	ft_bzero(nb, 16);
	name = NULL;
	if ((str = ft_strstr(h->ar_name, "#1/")))
	{
		ft_strncpy(nb, &str[3], 13);
		n += ft_atoi(nb);
		name = ft_strsub(&h->ar_fmag[2], 0, (size_t)(n - 2));
	}
	if (!name && (str = ft_strnstr(h->ar_name, " ", 16)))
		name = ft_strsub(h->ar_name, 0, (size_t)OFFSET(str, h->ar_name));
	else if (!name)
		name = ft_strsub(h->ar_name, 0, 16);
	ft_putstr(d->av);
	ft_putchar('(');
	if (name)
		ft_putstr(name);
	ft_putendl("):");
	ft_strdel(&name);
	return (n);
}

static void		archive_handler(t_data *d)
{
	uint32_t	magic;
	off_t			i;
	struct ar_hdr	*h;
	unsigned char	*ptr;

	i = SARMAG;
	ptr = (unsigned char *)d->file;
	ptr = ptr + i; /*On passe le ARMAG*/
	while (OFFSET(ptr, d->file) < d->stat.st_size)	/*Tant que l'offset entre debut et ptr < size*/
	{
		h = (struct ar_hdr *)ptr;	/*Recuperation de la metadata header archive*/
		if (i > SARMAG && h)
			i = extract_ar_name(d, h);
		if ((magic = *(uint32_t *)(void*)&h->ar_fmag[i]) == MH_MAGIC_64)	/*Magic juste apres la mdata*/
			arch_64_handler((void*)&h->ar_fmag[i], NULL);	/*Gestion Mach-O x64*/
		else if (magic == MH_MAGIC)
			arch_32_handler((void*)&h->ar_fmag[i], NULL);	/*Gestion Mach-O x32*/
		i = (off_t)((sizeof(struct ar_hdr)) + (size_t)ft_atoi(h->ar_size));	/*calcul de l'offset*/
		ptr = ptr + i;	/*Decalage du ptr avec l'offset*/
	}
}

static void	fat_arch_32_handler(void *file, char *name)
{
	unsigned int i, x;
	struct fat_header	*fh;
 	struct fat_arch *frh;

	i = 0;
	if (!(fh = (struct fat_header *)file))
		return ;
	if (name)
		ft_putendl(name);
	x = swap_uint32(fh->nfat_arch);
	printf("nfat_arch:%x - magic:%x\n", x, swap_uint32(fh->magic));
	frh = (void*)((char*)file + sizeof(struct fat_header));
	while (i < x)
	{
		ft_putendl("\nArchitecture:");
		printf("cputype:%d\ncpusub:%d\noffset:%d\nsize:%d\n", swap_uint32((uint32_t)frh->cputype), swap_uint32((uint32_t)frh->cpusubtype), swap_uint32(frh->offset), swap_uint32(frh->size));
		frh = frh + 1;
		i++;
	}
	ft_putendlcol(RED, "Fat_Arch_32");
}

void		ft_nm(t_list *elem)
{
	uint32_t	magic;
	t_data		*d;

	if (!elem || !elem->content)
		return ;
	if ((d = (t_data*)elem->content)->token != PATH || !d->file)
		return ;
	if ((magic = *(uint32_t*)d->file) == MH_MAGIC_64)	/*Mach-O 64bit*/
		arch_64_handler(d->file, d->av);
	else if (magic == MH_MAGIC)	/*Mach-O 32bit*/
		arch_32_handler(d->file, d->av);
	else if (magic == FAT_MAGIC || magic == FAT_CIGAM)
		fat_arch_32_handler(d->file, d->av);
	else if (!(ft_strncmp(ARMAG, (char*)d->file, SARMAG)))	/*Archive*/
		archive_handler(d);
	else
		ft_putendl("NONE");
	/*ARMAG/SARMAG - Correspond au magic_str et size_magic_str - voir ar.h - */
}
