/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_char.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbloodax <jbloodax@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/27 21:03:59 by akraig            #+#    #+#             */
/*   Updated: 2020/01/29 18:03:36 by jbloodax         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	print_char(int64_t c, t_parse *p)
{
	if (p->flags && ft_strchr(p->flags, '-'))
	{
		if (c != 0) 
		{
			printf("1\n");
			buffer(p, (char*)&c, 0);
		}
		buffer(p, ft_charstr(p->width - 1, ' '), 1);
	}
	else
	{
		buffer(p, ft_charstr(p->width - 1, ' '), 1);
		if (c != 0)
		{
			printf("2\n");
			buffer(p, (char*)&c, 0);
		}
	}
}
