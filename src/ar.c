/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ar.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: banthony <banthony@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/18 16:50:57 by banthony          #+#    #+#             */
/*   Updated: 2017/12/15 23:33:57 by banthony         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm_otool.h"

static off_t	is_ranlib(t_data *d, char **name, off_t n)
{
	if (!(ft_strncmp(*name, SYMDEF, ft_strlen(SYMDEF)))
			|| !(ft_strncmp(*name, SYMDEF_SORTED, ft_strlen(SYMDEF_SORTED))))
	{
		ft_strdel(name);
		return (RANLIB);
	}
	if (d->token[CMD] == NM)
		ft_putchar('\n');
	ft_putstr(d->av);
	ft_putchar('(');
	if (name && *name)
		ft_putstr(*name);
	ft_putendl("):");
	ft_strdel(name);
	return (n);
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
	if ((str = ft_strnstr(h->ar_name, "#1/", 16)))
	{
		ft_strncpy(nb, &str[3], 13);
		n += ft_atoi(nb);
		name = ft_strsub(&h->ar_fmag[2], 0, (size_t)(n - 2));
	}
	if (!name && (str = ft_strnstr(h->ar_name, " ", 16)))
		name = ft_strsub(h->ar_name, 0, (size_t)OFFSET(str, h->ar_name));
	else if (!name)
		name = ft_strsub(h->ar_name, 0, 16);
	return (is_ranlib(d, &name, n));
}

static int		magic_handler(off_t *i, t_data *d, struct ar_hdr *h)
{
	uint32_t	magic;
	int			error;

	error = 1;
	if (ft_strncmp(h->ar_fmag, ARFMAG, ft_strlen(ARFMAG)))
		return (error);
	if ((*i = extract_ar_name(d, h)) == RANLIB)
		return (0);
	magic = *(uint32_t *)(void*)&h->ar_fmag[*i];
	if (magic == MH_MAGIC_64 || magic == MH_CIGAM_64)
		error = arch_64_handler(magic, d, (void*)&h->ar_fmag[*i],
									(off_t)ft_atoi(h->ar_size));
	else if (magic == MH_MAGIC || magic == MH_CIGAM)
		error = arch_32_handler(magic, d, (void*)&h->ar_fmag[*i],
									(off_t)ft_atoi(h->ar_size));
	else if (magic == FAT_MAGIC || magic == FAT_CIGAM)
		error = fat_arch_32_handler(magic, d, (void*)&h->ar_fmag[*i],
									(off_t)ft_atoi(h->ar_size));
	else if (!(ft_strncmp(ARMAG, (char*)&h->ar_fmag[*i], SARMAG)))
		return (ARCHIVE_CONCAT);
	return (error);
}

static void		otool_display(t_data *d)
{
	if (!d->token[TYPE])
		d->token[TYPE] = ARCHIVE;
	if (d->token[CMD] == OTOOL)
	{
		ft_putstr("Archive : ");
		ft_putendl(d->av);
	}
}

int				archive_handler(void *file, off_t size, t_data *d)
{
	off_t			i;
	int				error;
	struct ar_hdr	*h;
	unsigned char	*ptr;

	i = SARMAG;
	error = 1;
	ptr = ((unsigned char *)file) + i;
	otool_display(d);
	while (OFFSET(ptr, file) < size)
	{
		h = (struct ar_hdr *)ptr;
		if (is_corrup((void *)(h + 1), file, size))
			return (1);
		error = magic_handler(&i, d, h);
		if (error == ARCHIVE_CONCAT)
			ft_putendl(ARCHIVE_CONCAT_MSG);
		i = (off_t)((sizeof(struct ar_hdr)) + (size_t)ft_atoi(h->ar_size));
		ptr = ptr + i;
	}
	if (error && error != ARCHIVE_CONCAT && i != SARMAG)
		return (error);
	return (0);
}
