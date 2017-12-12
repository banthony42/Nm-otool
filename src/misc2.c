/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   misc2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: banthony <banthony@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/07 21:27:29 by banthony          #+#    #+#             */
/*   Updated: 2017/12/12 21:01:48 by banthony         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm_otool.h"

/*
**	Base de donnees pour les architectures.
**	Utiliser lors d'un affichage de donnees de plusieurs architecture.
**	(fat32)
*/

t_arch g_arch_data[ARCH_DATA_SIZE] =
{
	{	"x86_64"		, CPU_TYPE_X86_64		, CPU_SUBTYPE_X86_64_ALL	},
	{	"i386"			, CPU_TYPE_I386			, CPU_SUBTYPE_I386_ALL		},
	{	"arm"			, CPU_TYPE_ARM			, CPU_SUBTYPE_ARM_ALL		},
	{	"arm6"			, CPU_TYPE_ARM64		, CPU_SUBTYPE_ARM64_ALL		},
	{	"ppc"			, CPU_TYPE_POWERPC		, CPU_SUBTYPE_POWERPC_ALL	},
	{	"ppc64"			, CPU_TYPE_POWERPC64	, CPU_SUBTYPE_POWERPC_ALL	},
	{	"m68k"			, CPU_TYPE_MC680x0		, CPU_SUBTYPE_MC680x0_ALL	},
	{	"hppa"			, CPU_TYPE_HPPA			, CPU_SUBTYPE_HPPA_ALL		},
	{	"i860"			, CPU_TYPE_I860			, CPU_SUBTYPE_I860_ALL		},
	{	"m88k"			, CPU_TYPE_MC88000		, CPU_SUBTYPE_MC88000_ALL	},
	{	"sparc"			, CPU_TYPE_SPARC		, CPU_SUBTYPE_SPARC_ALL		},
	{	"i486"			, CPU_TYPE_I386			, CPU_SUBTYPE_486			},
	{	"i486SX"		, CPU_TYPE_I386			, CPU_SUBTYPE_486SX			},
	{	"pentium"		, CPU_TYPE_I386			, CPU_SUBTYPE_PENT			},
	{	"i586"			, CPU_TYPE_I386			, CPU_SUBTYPE_586			},
	{	"pentpro"		, CPU_TYPE_I386			, CPU_SUBTYPE_PENTPRO		},
	{	"i686"			, CPU_TYPE_I386			, CPU_SUBTYPE_PENTPRO		},
	{	"pentIIm3"		, CPU_TYPE_I386			, CPU_SUBTYPE_PENTII_M3		},
	{	"pentIIm5"		, CPU_TYPE_I386			, CPU_SUBTYPE_PENTII_M5		},
	{	"pentium4"		, CPU_TYPE_I386			, CPU_SUBTYPE_PENTIUM_4		},
	{	"armv4t"		, CPU_TYPE_ARM			, CPU_SUBTYPE_ARM_V4T		},
	{	"armv5"			, CPU_TYPE_ARM			, CPU_SUBTYPE_ARM_V5TEJ		},
	{	"xscale"		, CPU_TYPE_ARM			, CPU_SUBTYPE_ARM_XSCALE	},
	{	"armv6"			, CPU_TYPE_ARM			, CPU_SUBTYPE_ARM_V6		},
	{	"armv6m"		, CPU_TYPE_ARM			, CPU_SUBTYPE_ARM_V6M		},
	{	"armv7"			, CPU_TYPE_ARM			, CPU_SUBTYPE_ARM_V7		},
	{	"armv7f"		, CPU_TYPE_ARM			, CPU_SUBTYPE_ARM_V7F		},
	{	"armv7s"		, CPU_TYPE_ARM			, CPU_SUBTYPE_ARM_V7S		},
	{	"armv7k"		, CPU_TYPE_ARM			, CPU_SUBTYPE_ARM_V7K		},
	{	"armv7m"		, CPU_TYPE_ARM			, CPU_SUBTYPE_ARM_V7M		},
	{	"armv7em"		, CPU_TYPE_ARM			, CPU_SUBTYPE_ARM_V7EM		},
	{	"armv8"			, CPU_TYPE_ARM			, CPU_SUBTYPE_ARM_V8		},
	{	"arm64"			, CPU_TYPE_ARM64		, CPU_SUBTYPE_ARM64_V8		},
	{	"ppc601"		, CPU_TYPE_POWERPC		, CPU_SUBTYPE_POWERPC_601	},
	{	"ppc603"		, CPU_TYPE_POWERPC		, CPU_SUBTYPE_POWERPC_603	},
	{	"ppc604"		, CPU_TYPE_POWERPC		, CPU_SUBTYPE_POWERPC_604	},
	{	"ppc604e"		, CPU_TYPE_POWERPC		, CPU_SUBTYPE_POWERPC_604e	},
	{	"ppc750"		, CPU_TYPE_POWERPC		, CPU_SUBTYPE_POWERPC_750	},
	{	"ppc7400"		, CPU_TYPE_POWERPC		, CPU_SUBTYPE_POWERPC_7400	},
	{	"ppc7450"		, CPU_TYPE_POWERPC		, CPU_SUBTYPE_POWERPC_7450	},
	{	"ppc970"		, CPU_TYPE_POWERPC		, CPU_SUBTYPE_POWERPC_970	},
	{	"m68030"		, CPU_TYPE_MC680x0		, CPU_SUBTYPE_MC68030_ONLY	},
	{	"m68040"		, CPU_TYPE_MC680x0		, CPU_SUBTYPE_MC68040		},
	{	"hppa7100LC"	, CPU_TYPE_HPPA			, CPU_SUBTYPE_HPPA_7100LC	},
};

/*
**	Parcours de la base de donnee, affichage du nom de l'arch lors d'un match.
*/

void	print_arch(struct fat_arch frh, t_data *d, int mgc)
{
	int			i;
	uint32_t	cpu;
	uint32_t	cpusub;

	i = -1;
	if (mgc == -1)
	{
		cpu = ~CPU_ARCH_MASK & (swap_uint32((uint32_t)frh.cputype));
		cpusub = ~CPU_SUBTYPE_MASK & (swap_uint32((uint32_t)frh.cpusubtype));
	}
	else
	{
		cpu = ~CPU_ARCH_MASK & ((uint32_t)frh.cputype);
		cpusub = ~CPU_SUBTYPE_MASK & ((uint32_t)frh.cpusubtype);
	}
	ft_putchar('\n');
	ft_putstr(d->av);
	ft_putstr(" (for architecture ");
	while (++i < ARCH_DATA_SIZE)
	{
		if (cpu == (uint32_t)g_arch_data[i].cputype &&
				(cpusub == (uint32_t)g_arch_data[i].cpusubtype))
			ft_putstr(g_arch_data[i].name);
	}
	ft_putendl("):");
}

int		is_corrup(unsigned char *ptr, void *file, off_t size)
{
	if (ptr < (unsigned char *)file)
		return (1);
	if (ptr > (unsigned char *)((unsigned char *)file + size))
		return (1);
	return (0);
}

/*
**	Recursivite pour parcour inverse de la liste.
*/

void	lstiter_reverse(t_list *lst, void (*f)(t_list *elem))
{
	if (lst)
		lstiter_reverse(lst->next, f);
	if (lst)
		f(lst);
}

void	smb_del(void *content, size_t size)
{
	t_smb	*tmp;

	if (!content || !size)
		return ;
	tmp = (t_smb *)content;
	ft_strdel(&tmp->name);
	ft_strdel(&tmp->value);
	ft_memdel((void **)&tmp);
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
	d->token[TYPE] = PATH;
	d->token[CMD] = OTOOL;
	if (!ft_strcmp(cmd, FT_NM))
		d->token[CMD] = NM;
	if (str[0] == '-' && *wait)
		d->token[TYPE] = OPTION;
	if (!(ft_strcmp(str, "--")))
		*wait = 0;
	return (d);
}
