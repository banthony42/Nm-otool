/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   misc4.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: banthony <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/14 23:45:31 by banthony          #+#    #+#             */
/*   Updated: 2017/12/14 23:45:32 by banthony         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm_otool.h"

static void	print_hex(unsigned char *adr, size_t i, size_t size)
{
	size_t	n;

	n = i;
	while (n < i + 16)
	{
		if (n < size)
		{
			ft_putchar_base(adr[n], 16, "0123456789abcdef");
			ft_putchar(' ');
		}
		n++;
	}
}

void		print_memory64(uint64_t vmaddr, void *addr, size_t size)
{
	char			*vmadr;
	unsigned char	*adr;
	size_t			i;

	adr = (unsigned char*)addr;
	i = 0;
	ft_putstr("Contents of (__TEXT,__text) section\n");
	while (i < size)
	{
		vmadr = itoa_base_uint64(vmaddr + i, 16);
		ft_putstr(vmadr);
		ft_putchar('\t');
		print_hex(adr, i, size);
		ft_putchar('\n');
		ft_strdel(&vmadr);
		i += 16;
	}
}

void		print_memory32(uint32_t vmaddr, void *addr, size_t size)
{
	char			*vmadr;
	unsigned char	*adr;
	size_t			i;

	adr = (unsigned char*)addr;
	i = 0;
	ft_putstr("Contents of (__TEXT,__text) section\n");
	while (i < size)
	{
		vmadr = itoa_base_uint32(vmaddr + (uint32_t)i, 16);
		ft_putstr(vmadr);
		ft_putchar('\t');
		print_hex(adr, i, size);
		ft_putchar('\n');
		ft_strdel(&vmadr);
		i += 16;
	}
}

char		*itoa_base_uint64(uint64_t value, int base)
{
	uint64_t	val;
	uint64_t	i;
	uint64_t	len;
	char		*numb;

	i = 0;
	val = value;
	len = 16;
	if (!(numb = (char*)malloc((size_t)(len + 1) * sizeof(char))))
		return (NULL);
	ft_bzero(numb, (size_t)(len + 1) * sizeof(char));
	if (len > 0)
		len--;
	while (i <= len)
	{
		if (val <= 9 || (val % (unsigned int)base) < 10)
			numb[len - i] = (char)((val % (unsigned int)base) + '0');
		else
			numb[len - i] = (char)((val % (unsigned int)base) + ('a' - 10));
		val = val / (unsigned int)base;
		i++;
	}
	return (numb);
}

char		*itoa_base_uint32(uint32_t value, int base)
{
	uint32_t	val;
	uint32_t	i;
	uint32_t	len;
	char		*numb;

	i = 0;
	val = value;
	len = 8;
	if (!(numb = (char*)malloc((size_t)(len + 1) * sizeof(char))))
		return (NULL);
	ft_bzero(numb, (size_t)(len + 1) * sizeof(char));
	if (len > 0)
		len--;
	while (i <= len)
	{
		if (val <= 9 || (val % (unsigned int)base) < 10)
			numb[len - i] = (char)((val % (unsigned int)base) + '0');
		else
			numb[len - i] = (char)((val % (unsigned int)base) + ('a' - 10));
		val = val / (unsigned int)base;
		i++;
	}
	return (numb);
}
