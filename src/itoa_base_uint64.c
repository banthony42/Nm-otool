/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   itoa_base_uint64.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: banthony <banthony@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/10 20:11:09 by banthony          #+#    #+#             */
/*   Updated: 2017/11/10 21:06:19 by banthony         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

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
