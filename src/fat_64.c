/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fat_64.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: banthony <banthony@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/29 21:48:47 by banthony          #+#    #+#             */
/*   Updated: 2017/11/30 16:18:48 by banthony         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

/*
**  Extrait de fat.h:
**
**	The support for the 64-bit fat file format described here is a work in
**	progress and not yet fully supported in all the Apple Developer Tools.
**
**	Pas encore trouve de binaire en utilisant fat_arch_64
*/

int	fat_arch_64_handler(uint32_t magic, unsigned char *file, off_t size)
{
	int					error;
	uint32_t			i;
	uint32_t			nfat_arch;
	struct fat_arch_64	*frh;

	if (!file_access(file, sizeof(struct fat_header)
							+ sizeof(struct fat_arch_64), size))
		return (0);
	i = 0;
	error = -1;
	nfat_arch = *(uint32_t*)(void*)(file + sizeof(uint32_t));
	frh = (void*)(file + sizeof(struct fat_header));
	if (magic == FAT_CIGAM_64)
	{
		nfat_arch = swap_uint32(nfat_arch);
		error = fat_arch_64_cigam(nfat_arch, frh, file, size);
	}
	if (magic == FAT_MAGIC_64)
		error = fat_arch_64_magic(nfat_arch, frh, file, size);
	if (error <= 0)
		return (error);
	return (1);
}

int	fat_arch_64_cigam(uint32_t nfat_arch, struct fat_arch_64 *frh,
									unsigned char *file, off_t size)
{
	int				error;
	uint32_t		*mgc;
	uint32_t		i;
	uint64_t		offset;

	i = 0;
	error = -1;
	while (i < nfat_arch)
	{
		offset = swap_uint64(frh[i].offset);
		if (!file_access(file, (off_t)offset, size))
			return (-1);
		mgc = (uint32_t*)(void*)(file + offset);
		if (*mgc == MH_MAGIC_64 || *mgc == MH_CIGAM_64)
			error = arch_64_handler(*mgc, (void*)mgc, size);
		else if (*mgc == MH_MAGIC || *mgc == MH_CIGAM)
			error = arch_32_handler(*mgc, (void*)mgc, size);
		if (error <= 0)
			return (error);
		i++;
	}
	return (1);
}

int	fat_arch_64_magic(uint32_t nfat_arch, struct fat_arch_64 *frh,
									unsigned char *file, off_t size)
{
	int				error;
	uint32_t		*mgc;
	uint32_t		i;

	i = 0;
	error = -1;
	while (i < nfat_arch)
	{
		if (!file_access(file, (off_t)frh[i].offset, size))
			return (-1);
		mgc = (uint32_t*)(void*)(file + frh[i].offset);
		if (*mgc == MH_MAGIC_64 || *mgc == MH_CIGAM_64)
			error = arch_64_handler(*mgc, (void*)mgc, size);
		else if (*mgc == MH_MAGIC || *mgc == MH_CIGAM)
			error = arch_32_handler(*mgc, (void*)mgc, size);
		if (error <= 0)
			return (error);
		i++;
	}
	return (1);
}
