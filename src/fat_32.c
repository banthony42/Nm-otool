/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fat_32.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: banthony <banthony@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/29 21:48:31 by banthony          #+#    #+#             */
/*   Updated: 2017/11/21 14:51:06 by banthony         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

int	file_access(void *file, off_t read, off_t file_size)
{
	unsigned char *ptr;

	if (!file)
		return (0);
	ptr = (unsigned char *)file;
	if ((ptr + read) < (ptr + file_size))
		return (1);
	return (0);
}

int	fat_arch_32_handler(uint32_t magic, t_data *d, unsigned char *file, off_t size)
{
	int				error;
	uint32_t		i;
	uint32_t		nfat_arch;

	if (!file_access(file, sizeof(struct fat_header) + sizeof(struct fat_arch), size))
		return (0);
	i = 0;
	error = -1;
	nfat_arch = *(uint32_t*)(void*)(file + sizeof(uint32_t));	/*Recup du nb de struct fat_arch*/
	if (magic == FAT_CIGAM)
	{
		nfat_arch = swap_uint32(nfat_arch);	/*Si BIG_ENDIAN il faut swap*/
		error = fat_arch_32_cigam(nfat_arch, d, file, size);
	}
	if (magic == FAT_MAGIC)
		error = fat_arch_32_magic(nfat_arch, d, file, size);
	if (error <= 0)
		return (error);
	return (1);
}

int	fat_arch_32_cigam(uint32_t nfat_arch, t_data *d, unsigned char *file, off_t size)
{
	int				error;
	uint32_t		*mgc;
	uint32_t		i;
	uint32_t		offset;
	struct fat_arch	*frh;

	i = 0;
	error = -1;
	frh = (void*)(file + sizeof(struct fat_header));	/*Recup du pointeur vers la premiere struct fat_header*/
	while (i < nfat_arch)
	{
		offset = swap_uint32(frh[i].offset);
		if (!file_access(file, offset, size))
			return (-1);
		mgc = (uint32_t*)(void*)(file + offset);
		if (*mgc == MH_MAGIC_64 || *mgc == MH_CIGAM_64)
			error = arch_64_handler(*mgc, d, (void*)mgc, size);
		else if (*mgc == MH_MAGIC || *mgc == MH_CIGAM)
			error = arch_32_handler(*mgc, d, (void*)mgc, size);
		if (error <= 0)
			return (error);
		i++;
	}
	return (1);
}

int	fat_arch_32_magic(uint32_t nfat_arch, t_data *d, unsigned char *file, off_t size)
{
	int				error;
	uint32_t		*mgc;
	uint32_t		i;
	struct fat_arch	*frh;

	i = 0;
	error = -1;
	frh = (void*)(file + sizeof(struct fat_header));	/*Recup du pointeur vers la premiere struct fat_header*/
	while (i < nfat_arch)
	{
		if (!file_access(file, frh[i].offset, size))
			return (-1);
		mgc = (uint32_t*)(void*)(file + frh[i].offset);	/*Recup du magic Mach-O*/
		if (*mgc == MH_MAGIC_64 || *mgc == MH_CIGAM_64)	/*Mach-O 64bit*/
			error = arch_64_handler(*mgc, d, (void*)mgc, size);
		else if (*mgc == MH_MAGIC || *mgc == MH_CIGAM)	/*Mach-O 32bit*/
			error = arch_32_handler(*mgc, d, (void*)mgc, size);
		if (error <= 0)
			return (error);
		i++;
	}
	return (1);
}
