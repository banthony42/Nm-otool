/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   misc3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: banthony <banthony@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/08 16:20:42 by banthony          #+#    #+#             */
/*   Updated: 2017/12/18 19:13:20 by banthony         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm_otool.h"

void	list_builder(t_data **d, t_list *l)
{
	if (!d || !(*d))
		return ;
	if (!((*d)->sym))
		(*d)->sym = l;
	else
		(*d)->lstadd_somewhere(&(*d)->sym, l);
}

int		*error_number(int *err)
{
	static int error;

	if (err)
		error = *err;
	return (&error);
}

int		ft_islower(int c)
{
	if (c >= (int)'a' && c <= (int)'z')
		return (1);
	return (0);
}

void	nm_output(t_list *elem)
{
	t_smb	*tmp;

	if (!(tmp = (t_smb*)(elem->content)) || (tmp->type == '?')
										|| (tmp->type == '-'))
		return ;
	if (is_opt(tmp->options, 'U') && ft_strchr("UuCc", tmp->type))
		return ;
	if (is_opt(tmp->options, 'u') && !ft_strchr("UuCc", tmp->type))
		return ;
	if (is_opt(tmp->options, 'g') && ft_islower((int)tmp->type))
		return ;
	if (!is_opt(tmp->options, 'u') && !is_opt(tmp->options, 'j'))
	{
		if ((tmp->type == 'U') && tmp->arch == ARCH64)
			ft_putstr(PADD_SPACE64);
		else if ((tmp->type == 'U') && tmp->arch == ARCH32)
			ft_putstr(PADD_SPACE32);
		else
			ft_putstr(tmp->value);
		ft_putchar(' ');
		ft_putchar((char)tmp->type);
		ft_putchar(' ');
	}
	ft_putendl(tmp->name);
}

t_data	*new_data(char *str, int *wait, char *cmd)
{
	t_data *d;

	d = NULL;
	if (!str)
		return (NULL);
	if (!(d = (t_data*)ft_memalloc(sizeof(t_data))))
		return (NULL);
	if (!(d->av = ft_strdup(str)))
		return (NULL);
	d->token[ELMT] = PATH;
	d->token[CMD] = OTOOL;
	if (!ft_strcmp(cmd, FT_NM))
		d->token[CMD] = NM;
	if (str[0] == '-' && *wait)
		d->token[ELMT] = OPTION;
	if (!(ft_strcmp(str, "--")))
		*wait = 0;
	return (d);
}
