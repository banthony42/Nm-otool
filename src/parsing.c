/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: banthony <banthony@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/07 18:44:27 by banthony          #+#    #+#             */
/*   Updated: 2017/12/16 20:47:21 by banthony         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm_otool.h"

static int	option_error_handler(t_list **lst, char *options)
{
	int		i;
	char	dash;
	t_data	*d;
	t_list	*l;

	l = *lst;
	while (l)
	{
		i = -1;
		dash = 0;
		d = (t_data*)l->content;
		if (d->token[ELMT] == OPTION)
		{
			while (d->av[++i])
			{
				if (!(ft_strchr(options, (int)d->av[i]))
					|| (dash && d->av[i] == '-') || !ft_strcmp(d->av, "-"))
					return (cmd_info(d->token[CMD], d->av, UNKNOWN_OPTION));
				if ((ft_strcmp(d->av, "--")) && d->av[i] == '-')
					dash++;
			}
		}
		l = l->next;
	}
	return (1);
}

/*
**	Pour chaque argument creer un maillon a la liste t_data.
**	Verifie les options presentes, erreur si une options n'est pas reconnue.
**	Creer un maillon de fichier par defaut si aucun fichier present. (a.out)
**	Prepare tout les fichier pour nm ou otool: open, fstat, mmap ...
*/

t_list		*parsing(char **av, char *cmd, char *options)
{
	t_list	*lst;
	int		i;
	int		wait;
	t_data	*h;

	i = 0;
	lst = NULL;
	wait = 1;
	h = NULL;
	while (av[++i])
	{
		if (!(h = new_data(av[i], &wait, cmd)))
			continue ;
		if (!lst)
			lst = ft_lstnew((void*)h, sizeof(t_data));
		else
			ft_lstaddback(&lst, ft_lstnew(h, sizeof(t_data)));
		ft_memdel((void**)&h);
	}
	if (!(option_error_handler(&lst, options)))
		ft_lstdel(&lst, &data_del);
	if (!ft_strcmp(cmd, FT_NM))
		default_file(&lst);
	ft_lstiter(lst, &prepare_files);
	return (lst);
}

/*
**	Ouverture du fichier, Lecture avec fstat
**	Gestion d'erreur sur permission denied, fichier inexistant,
**	ou si le path est un dossier.
*/

static int	get_type(t_data *d)
{
	int ret;

	ret = 0;
	if ((d->fd = open(d->av, O_RDONLY)) < 0)
	{
		if (errno == EACCES)
			cmd_info(d->token[CMD], d->av, PERMISSION);
		else if (errno == ENOENT)
			cmd_info(d->token[CMD], d->av, FILE_NOT_FOUND);
		return (1);
	}
	else if ((ret = fstat(d->fd, &d->stat)) < 0)
	{
		cmd_info(d->token[CMD], d->av, FSTAT_ERROR);
		return (1);
	}
	else if (S_ISDIR(d->stat.st_mode))
	{
		cmd_info(d->token[CMD], d->av, IS_A_DIR);
		return (1);
	}
	return (0);
}

void		prepare_files(t_list *elm)
{
	int		error;
	t_data	*d;

	if (!elm || !(d = (t_data*)elm->content) || d->token[ELMT] != PATH)
		return ;
	error = 0;
	error = get_type(d);
	if (!error && (d->file = mmap(NULL, (size_t)d->stat.st_size,
					PROT_READ, MAP_PRIVATE, d->fd, 0)) == MAP_FAILED)
		error = 1;
	if (d->file == MAP_FAILED)
		d->file = NULL;
	error_number(&error);
}

void		default_file(t_list **lst)
{
	t_list	*l;
	t_data	*d;

	d = NULL;
	if (!lst || !(l = *lst))
		return ;
	while (l)
	{
		if (((t_data*)l->content)->token[ELMT] == PATH)
			return ;
		l = l->next;
	}
	if (!(d = (t_data*)malloc((sizeof(t_data)))))
		return ;
	ft_bzero(d, sizeof(t_data));
	d->token[ELMT] = PATH;
	d->token[CMD] = NM;
	d->av = ft_strdup("a.out");
	ft_lstaddback(lst, ft_lstnew(d, sizeof(t_data)));
	ft_memdel((void**)&d);
}
