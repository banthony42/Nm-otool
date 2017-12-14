/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fat_32.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: banthony <banthony@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/29 21:48:31 by banthony          #+#    #+#             */
/*   Updated: 2017/12/14 23:42:16 by banthony         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm_otool.h"

/*
**	Recherche d'une architecture identique a l'architecture de l'host.
**	Si l'architecture de l'host n'est pas trouvee, on affiche toute les arch.
*/

static int	is_arch(t_data *d, struct fat_arch *frh, int *i, uint32_t nfat_arch)
{
	uint8_t	n;
	int		arch;

	arch = 0;
	n = 0;
	if (is_corrup((void *)(frh + nfat_arch), d->file, d->stat.st_size))
		return (1);
	while (n < nfat_arch)
	{
		if (((uint32_t)frh[n].cputype & CPU_TYPE_X86))
			arch++;
		n++;
	}
	if (nfat_arch == 1)
		cmd_info(d->token[CMD], d->av, NULL);
	else if (!arch)
		print_arch(frh[i[0]], d, i[2]);
	if (!arch)
		return (ARCH_ALL);
	if (((uint32_t)frh[i[0]].cputype & CPU_TYPE_X86) || nfat_arch == 1)
		return (ARCH_OK);
	return (0);
}

static int	magic_handler(uint32_t *mgc, t_data *d, uint32_t size, int flag)
{
	int error;

	error = 1;
	if (*mgc == MH_MAGIC_64 || *mgc == MH_CIGAM_64)
	{
		if (d->token[CMD] == OTOOL && flag == ARCH_OK)
			cmd_info(0, d->av, NULL);
		error = arch_64_handler(*mgc, d, (void*)mgc, size);
	}
	else if (*mgc == MH_MAGIC || *mgc == MH_CIGAM)
	{
		if (d->token[CMD] == OTOOL && flag == ARCH_OK)
			cmd_info(0, d->av, NULL);
		error = arch_32_handler(*mgc, d, (void*)mgc, size);
	}
	else if (!(ft_strncmp(ARMAG, (char*)mgc, SARMAG)))
		error = archive_handler((void*)mgc, size, d);
	return (error);
}

/*
**	i[0] : compteur pour parcourir le ptr frh
**	i[1] : contient le retour de la fonction is_arch
**	i[3] : contient l'info CIGAM ou MAGIC
*/

int			fat_arch_32_cigam(uint32_t nfat_arch, t_data *d,
								unsigned char *file, off_t size)
{
	int				error;
	uint32_t		*mgc;
	int				i[3];
	struct fat_arch	*frh;

	ft_memset(i, -1, sizeof(int) * 3);
	error = 1;
	frh = (void*)(file + sizeof(struct fat_header));
	while (++i[0] < (int)nfat_arch)
	{
		if (!(i[1] = is_arch(d, frh, i, nfat_arch)) && i[1] != ARCH_ALL)
			continue ;
		if (is_corrup((void *)(file + swap_uint32(frh[i[0]].offset)),
						file, size))
			return (1);
		mgc = (uint32_t*)(void*)(file + swap_uint32(frh[i[0]].offset));
		if ((error = magic_handler(mgc, d, swap_uint32(frh[i[0]].size), i[1])))
			return (error);
		if (i[1] == ARCH_OK)
			break ;
	}
	return (0);
}

/*
**	i[0] : compteur pour parcourir le ptr frh
**	i[1] : contient le retour de la fonction is_arch
**	i[3] : contient l'info CIGAM ou MAGIC
*/

int			fat_arch_32_magic(uint32_t nfat_arch, t_data *d,
								unsigned char *file, off_t size)
{
	int				error;
	uint32_t		*mgc;
	int				i[3];
	struct fat_arch	*frh;

	ft_memset(i, 0, sizeof(int) * 3);
	error = 1;
	frh = (void*)(file + sizeof(struct fat_header));
	while (++i[0] < (int)nfat_arch + 1)
	{
		if (!(i[1] = is_arch(d, frh, i, nfat_arch)) && i[1] != ARCH_ALL)
			continue ;
		if (is_corrup((void *)(file + frh[i[0]].offset), file, size))
			return (1);
		mgc = (uint32_t*)(void*)(file + frh[i[0]].offset);
		if ((error = magic_handler(mgc, d, frh[i[0]].size, i[1])))
			return (error);
		if (i[1] == ARCH_OK)
			break ;
	}
	return (0);
}

int			fat_arch_32_handler(uint32_t magic, t_data *d,
							unsigned char *file, off_t size)
{
	int				error;
	uint32_t		nfat_arch;

	if (is_corrup((void*)(file + sizeof(struct fat_header)), file, size))
		return (1);
	error = 1;
	if (!d->token[TYPE])
		d->token[TYPE] = FAT;
	nfat_arch = *(uint32_t*)(void*)(file + sizeof(uint32_t));
	if (magic == FAT_CIGAM)
	{
		nfat_arch = swap_uint32(nfat_arch);
		error = fat_arch_32_cigam(nfat_arch, d, file, size);
	}
	if (magic == FAT_MAGIC)
		error = fat_arch_32_magic(nfat_arch, d, file, size);
	if (error)
		return (error);
	return (0);
}
