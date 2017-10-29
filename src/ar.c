/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ar.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: banthony <banthony@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/18 16:50:57 by banthony          #+#    #+#             */
/*   Updated: 2017/10/28 18:30:13 by banthony         ###   ########.fr       */
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

void		archive_handler(t_data *d)
{
	uint32_t	magic;
	off_t			i;
	int				error;
	struct ar_hdr	*h;
	unsigned char	*ptr;

	i = SARMAG;
	error = -1;
	ptr = (unsigned char *)d->file;
	ptr = ptr + i; /*On passe le ARMAG*/
	while (OFFSET(ptr, d->file) < d->stat.st_size)	/*Tant que l'offset entre debut et ptr < size*/
	{
		h = (struct ar_hdr *)ptr;	/*Recuperation de la metadata header archive*/
		if (i > SARMAG && h)
			i = extract_ar_name(d, h);
		magic = *(uint32_t *)(void*)&h->ar_fmag[i];
		if (magic == MH_MAGIC_64 || magic == MH_CIGAM_64)	/*Magic juste apres la mdata*/
			error = arch_64_handler(magic, (void*)&h->ar_fmag[i], d->stat.st_size);	/*Gestion Mach-O x64*/
		else if (magic == MH_MAGIC || magic == MH_CIGAM)
			error = arch_32_handler(magic, (void*)&h->ar_fmag[i], d->stat.st_size);	/*Gestion Mach-O x32*/
		if (error <= 0)
			error_str(d->av, ERR_FILE);
		i = (off_t)((sizeof(struct ar_hdr)) + (size_t)ft_atoi(h->ar_size));	/*calcul de l'offset*/
		ptr = ptr + i;	/*Decalage du ptr avec l'offset*/
	}
}










