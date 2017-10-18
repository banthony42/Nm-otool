/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fat.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: banthony <banthony@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/18 16:51:23 by banthony          #+#    #+#             */
/*   Updated: 2017/10/18 18:13:12 by banthony         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

void	fat_arch_64_handler(uint32_t magic, unsigned char *file)
{
	if (!file || !magic)
		return ;
	ft_putendlcol(GREEN, "FatArch64");
}

void	fat_arch_32_handler(uint32_t magic, unsigned char *file)
{
	uint32_t		*mgc;
	uint32_t		i;
	uint32_t		nfat_arch;
	struct fat_arch	*frh;

	if (!file)
		return ;
	i = 0;
	nfat_arch = *(uint32_t*)(void*)(file + sizeof(uint32_t));
	frh = (void*)(file + (2 * sizeof(uint32_t)));
	if (magic == FAT_CIGAM)
		nfat_arch = swap_uint32(nfat_arch);
	while (i < nfat_arch)
	{
		mgc = (uint32_t*)(void*)(file + frh[i].offset);
		if (magic == FAT_CIGAM)
			mgc = (uint32_t*)(void*)(file + swap_uint32(frh[i].offset));
		if (*mgc == MH_MAGIC_64 || *mgc == MH_CIGAM_64)	/*Mach-O 64bit*/
			arch_64_handler(*mgc, (void*)mgc);
		else if (*mgc == MH_MAGIC || *mgc == MH_CIGAM)	/*Mach-O 32bit*/
			arch_32_handler(*mgc, (void*)mgc);
		i++;
	}
}
