/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akraig <akraig@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/27 21:03:59 by akraig            #+#    #+#             */
/*   Updated: 2019/12/06 21:43:32 by akraig           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

t_node		*new_piece(char type, int mod)
{
	t_node	*new;

	if (!(new = (t_node*)malloc(sizeof(t_node))))
		return (NULL);
	new->mod = mod;
	new->type = type;
	new->s = NULL;
	new->c = 0;
	new->n = 0;
	new->f = 0;
	new->next = new;
	new->prev = new;
	return (new);
}

t_parse		*new_param(void)
{
	t_parse	*new;

	if (!(new = (t_parse*)malloc(sizeof(t_parse))))
		return (NULL);
	new->type = 0;
	new->is_signed = 0;
	new->is_long = 0;
	new->is_longlong = 0;
	new->is_short = 0;
	new->is__int64 = 0;
	new->is_size_t = 0;
	new->is_intmax_t = 0;
	new->is_ptrdiff_t = 0;
	new->flag = 0;
	new->width = 0;
	new->width_param = 0;
	new->precision = 0;
	new->precision_param = 0;
	new->align = 0;
	new->next = NULL;
	return (new);
}

void		del_param(t_parse *param)
{
	if (!param)
		return ;
	free(param);
}

t_node		*add_last_piece(t_node **head, t_node *new)
{
	t_node *tmp;

	if (head && new)
	{
		if (*head)
		{
			tmp = (*head)->prev;
			new->next = tmp->next;
			new->next->prev = new;
			new->prev = tmp;
			tmp->next = new;
		}
		else
			*head = new;
		return (new);
	}
	return (NULL);
}

int			int_length(int n)
{
	int length;

	length = 0;
	while (n)
	{
		n /= 10;
		++length;
	}
	return (length);
}

void	print_arg(t_parse *params, va_list valist)
{
	char	*types;

	types = "%diufFeEgGxXoscpaAn";
	if (ft_strchr("diuxX", params->type))
		ft_putnbr(va_arg(valist, int));
	else if ('c' == params->type)
		ft_putchar((char)va_arg(valist, int));
	else if ('s' == params->type)
		ft_putstr(va_arg(valist, char*));
}

void	check_size(t_parse *params, char *tmp)
{
	char	*sizes;
	int		index;

	sizes = "hlLzjt";
	index = ft_strchrn(sizes, *tmp);
	if (ft_strchr(sizes, *tmp) == ft_strchr(sizes, *(tmp + 1)))
	{
		if (index == 0)
			params->is_signed = 1;
		else if (index == 1)
			params->is_longlong = 1;
		tmp++;
	}
	else if (index == 0)
		params->is_short = 1;
	else if (index == 1)
		params->is_long = 1;
	else if (index == 2)
		params->is__int64 = 1;
	else if (index == 3)
		params->is_size_t = 1;
	else if (index == 4)
		params->is_intmax_t = 1;
	else if (index == 5)
		params->is_ptrdiff_t = 1;
}

t_parse	*parse_string(char *tmp, t_parse *params)
{
	int		stop;

	stop = 0;
	while (!stop && *tmp)
	{
		if (ft_strchr("-+ 0#", *tmp))				//flag
			params->flag = *tmp;
		else if ((params->width = ft_atoi(tmp)))	//width
			tmp += int_length(params->width) - 1;	//not sure, check
		else if (*tmp == '*')
			params->width_param = 1;				//add parameter here
		else if (*tmp == '.')						//precision
		{
			params->precision = ft_atoi(++tmp);
			if (*tmp == '*')
				params->precision_param = 1;		//add parameter here
			else if (params->precision != 0)
				tmp += int_length(params->precision) - 1;
		}
		else if (ft_strchr("hlLzjt", *tmp))			//size
			check_size(params, tmp);
		else if (ft_strchr("%diufFeEgGxXoscpaAn", *tmp))		//type
		{
			params->type = *tmp;
			stop = 1;
			++tmp;
		}
		++tmp;
	}
	params->next = tmp;
	return (params);
}

int		ft_printf(const char *restrict s, ...)
{
	va_list	valist;
	t_parse	*params;
	char	*tmp;

	tmp = (char*)s;
	va_start(valist, s);
	while (*tmp)
	{
		if (*tmp != '%')
			ft_putchar(*tmp);
		else
		{
			tmp++;
			params = new_param();
			parse_string(tmp, params);
			print_arg(params, valist);
			del_param(params);
		}
		tmp++;
	}
	return (0);
}
