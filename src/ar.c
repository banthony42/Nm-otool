/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ar.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: banthony <banthony@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/18 16:50:57 by banthony          #+#    #+#             */
/*   Updated: 2017/11/08 20:25:36 by banthony         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

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

static int	magic_handler(off_t *i, t_data *d, struct ar_hdr *h)
{
	uint32_t	magic;
	int error;

	error = -1;
	if (ft_strncmp(h->ar_fmag, ARFMAG, ft_strlen(ARFMAG)))
		return (error);
	*i = extract_ar_name(d, h);
	magic = *(uint32_t *)(void*)&h->ar_fmag[*i];
	if (magic == MH_MAGIC_64 || magic == MH_CIGAM_64)	/*Magic juste apres la mdata*/
		error = arch_64_handler(magic, (void*)&h->ar_fmag[*i],(off_t)ft_atoi(h->ar_size));	/*Gestion Mach-O x64*/
	else if (magic == MH_MAGIC || magic == MH_CIGAM)
		error = arch_32_handler(magic, (void*)&h->ar_fmag[*i], (off_t)ft_atoi(h->ar_size));	/*Gestion Mach-O x32*/
	else if (magic == FAT_MAGIC || magic == FAT_CIGAM)	/*FAT BINARY 32*/
		error = fat_arch_32_handler(magic, (void*)&h->ar_fmag[*i], (off_t)ft_atoi(h->ar_size));
	else if (!(ft_strncmp(ARMAG, (char*)&h->ar_fmag[*i], SARMAG)))	/*Archive*/
		return (IGNORE_FILE);
	else
		ft_putendlcol(YELLOW, ERR_MAGIC);
	return (error);
}

int		archive_handler(t_data *d)
{
	off_t			i;
	int				error;
	struct ar_hdr	*h;
	unsigned char	*ptr;

	i = SARMAG;
	error = -1;
	ptr = ((unsigned char *)d->file) + i;
	while (OFFSET(ptr, d->file) < d->stat.st_size)	/*Tant que l'offset entre debut et ptr < size*/
	{
		h = (struct ar_hdr *)ptr;	/*Recuperation de la metadata header archive*/
		if (i > SARMAG && h)
			error = magic_handler(&i, d, h);
		if (error <= 0 && i != SARMAG)
			return (error);
		i = (off_t)((sizeof(struct ar_hdr)) + (size_t)ft_atoi(h->ar_size));	/*calcul de l'offset*/
		ptr = ptr + i;	/*Decalage du ptr avec l'offset*/
	}
	return (1);
}
