/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fat_32.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: banthony <banthony@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/29 21:48:31 by banthony          #+#    #+#             */
/*   Updated: 2017/12/06 22:22:27 by banthony         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

int			file_access(void *file, off_t read, off_t file_size)
{
	unsigned char *ptr;

	if (!file)
		return (0);
	ptr = (unsigned char *)file;
	if ((ptr + read) < (ptr + file_size))
		return (1);
	return (0);
}

int			is_corrup(unsigned char *ptr, void *file, off_t size)
{
	if (ptr < (unsigned char *)file)
		return (1);
	if (ptr > (unsigned char *)((unsigned char *)file + size))
		return (1);
	return (0);
}

static int	is_arch(t_data *d, struct fat_arch *frh, int *i, uint32_t nfat_arch)
{
	uint8_t	n;
	int arch;

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
		ft_nm_info(d->av, NULL);
	else if (!arch)
		print_arch(frh[i[0]], d, i[2]);
	if (!arch)	/*Aucune arch ne correspond a celle du system, affichage de toutes les arch*/
		return (ARCH_ALL);
	if (((uint32_t)frh[i[0]].cputype & CPU_TYPE_X86) || nfat_arch == 1)
		return (ARCH_OK);
	return (0);
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
	frh = (void*)(file + sizeof(struct fat_header));/*Recup du pointeur vers la premiere struct fat_header*/
	while (++i[0] < (int)nfat_arch)
	{
		if (!(i[1] = is_arch(d, frh, i, nfat_arch)) && i[1] != ARCH_ALL)
			continue ;
		if (is_corrup((void *)(file + swap_uint32(frh[i[0]].offset)), file, size))
			return (1);
		mgc = (uint32_t*)(void*)(file + swap_uint32(frh[i[0]].offset));
		if (*mgc == MH_MAGIC_64 || *mgc == MH_CIGAM_64)
			error = arch_64_handler(*mgc, d, (void*)mgc, swap_uint32(frh[i[0]].size));
		else if (*mgc == MH_MAGIC || *mgc == MH_CIGAM)
			error = arch_32_handler(*mgc, d, (void*)mgc, swap_uint32(frh[i[0]].size));
		else if (!(ft_strncmp(ARMAG, (char*)mgc, SARMAG)))
			error = archive_handler((void *)mgc, swap_uint32(frh[i[0]].size), d);
		if (error)
			return (error);
		if (i[1] && i[1] != ARCH_ALL)
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
	frh = (void*)(file + sizeof(struct fat_header));/*Recup du pointeur vers la premiere struct fat_header*/
	while (++i[0] < (int)nfat_arch + 1)
	{
		if (!(i[1] = is_arch(d, frh, i, nfat_arch)) && i[1] != ARCH_ALL)
			continue ;
		if (is_corrup((void *)(file + frh[i[0]].offset), file, size))
			return (1);
		mgc = (uint32_t*)(void*)(file + frh[i[0]].offset);/*Recup du magic Mach-O*/
		if (*mgc == MH_MAGIC_64 || *mgc == MH_CIGAM_64)/*Mach-O 64bit*/
			error = arch_64_handler(*mgc, d, (void*)mgc, frh[i[0]].size);
		else if (*mgc == MH_MAGIC || *mgc == MH_CIGAM)/*Mach-O 32bit*/
			error = arch_32_handler(*mgc, d, (void*)mgc, frh[i[0]].size);
		else if (!(ft_strncmp(ARMAG, (char*)mgc, SARMAG)))
			error = archive_handler((void*)mgc, frh[i[0]].size, d);
		if (error)
			return (error);
		if (i[1] && i[1] != ARCH_ALL)
			break ;
	}
	return (0);
}

int			fat_arch_32_handler(uint32_t magic, t_data *d,
							unsigned char *file, off_t size)
{
	int				error;
	uint32_t		i;
	uint32_t		nfat_arch;

/*	if (!file_access(file, sizeof(struct fat_header)
							+ sizeof(struct fat_arch), size))
		return (0);*/
	if (is_corrup((void*)(file + sizeof(struct fat_header)), file ,size))
		return (1);
	i = 0;
	error = 1;
	nfat_arch = *(uint32_t*)(void*)(file + sizeof(uint32_t));/*Recup du nb de struct fat_arch*/
	if (magic == FAT_CIGAM)
	{
		nfat_arch = swap_uint32(nfat_arch);/*Si BIG_ENDIAN il faut swap*/
		error = fat_arch_32_cigam(nfat_arch, d, file, size);
	}
	if (magic == FAT_MAGIC)
		error = fat_arch_32_magic(nfat_arch, d, file, size);
	if (error)
		return (error);
	return (0);
}
