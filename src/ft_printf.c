/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akraig <akraig@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/27 21:03:59 by akraig            #+#    #+#             */
/*   Updated: 2019/12/20 21:10:09 by akraig           ###   ########.fr       */
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
	new->flags = NULL;
	new->width = 0;
	new->precision = 0;
	new->printed = 0;
	new->length = 0;
	new->next = NULL;
	return (new);
}

void		clear_param(t_parse *p)
{
	if (!p)
		return ;
	free(p->flags);
	p->type = 0;
	p->is_signed = 0;
	p->is_long = 0;
	p->is_longlong = 0;
	p->is_short = 0;
	p->is__int64 = 0;
	p->is_size_t = 0;
	p->is_intmax_t = 0;
	p->is_ptrdiff_t = 0;
	p->flags = NULL;
	p->width = 0;
	p->precision = 0;
	p->length = 0;
	p->next = NULL;
}

void		del_param(t_parse *p)
{
	if (!p)
		return ;
	free(p->flags);
	free(p);
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

void	print_char(va_list valist, t_parse *p)
{
	char	c;

	c = (char)va_arg(valist, int);
	if (p->flags && ft_strchr(p->flags, '-'))
	{
		ft_putchar(c);
		while ((p->width)-- > 0)
		{
			ft_putchar(' ');
			p->printed++;
		}
	}
	else
	{
		while ((p->width)-- > 1)
		{
			ft_putchar(' ');
			p->printed++;
		}
		ft_putchar(c);
		p->printed++;
	}
}

void		print_str(va_list valist, t_parse *p)
{
	(void) valist;
	(void) p;
}

int			int_length(intmax_t n)
{
	int length;

	length = 0;
	if (n <= 0)
		length++;
	while (n)
	{
		n /= 10;
		length++;
	}
	return (length);
}

int			int_length_and_update(intmax_t n, t_parse *p)
{
	p->length = int_length(n);
	if (n >= 0 && ft_strchr(p->flags, '+'))
		p->length++;
	if ((n < 0 || ft_strchr(p->flags, '+')) && p->precision)
		p->precision++;
	return (p->length);
}

void	put_sign(intmax_t n, t_parse *p)
{
	if (n < 0 || ft_strchr(p->flags, '+'))
	{
		if (n < 0)
			ft_putchar('-');
		else if (ft_strchr(p->flags, '+'))
			ft_putchar('+');
		p->length -= 1;
		p->width -= 1;
		if ((n < 0 || ft_strchr(p->flags, '+')) && p->precision)
			p->precision -= 1;
	}
	else if (ft_strchr(p->flags, ' '))
	{
		ft_putchar(' ');
		p->printed += 1;
	}
}

void	check_width_and_print_int(t_parse *p, int n)
{
	if (p->width > p->length && p->width > p->precision)
		p->printed += p->width;
	else
		p->printed += MAX(p->precision, p->length);
	put_sign(n , p);
	while ((p->precision)-- > p->length)
	{
		ft_putchar('0');
		p->width -= 1;
	}
	ft_putnbr(ft_absint(n));
	p->width -= p->length;
}

void	print_int(va_list valist, t_parse *p)
{
	intmax_t	n;

	n = va_arg(valist, intmax_t);
//	add min int processing
//	maybe change ft_putnbr_fd to intmax_t
//	if (n == -2147483648)
//		return (print_min_int(n));
	int_length_and_update(n, p);
	if (ft_strchr(p->flags, '-'))
	{
		check_width_and_print_int(p, n);
		while ((p->width)-- > 0)
			ft_putchar(' ');
	}
	else
	{
		if (p->width > p->precision && p->width > p->length)
		{
			p->printed += p->width;
			if (ft_strchr(p->flags, '0') && !p->precision)
			{
				put_sign(n, p);
				while (p->width-- > p->length)
					ft_putchar('0');
			}
			else
				while (p->width-- > (MAX(p->length, p->precision)))
					ft_putchar(' ');
			if (p->precision > 0)
			{
				put_sign(n, p);
				while (p->precision-- > p->length)
					ft_putchar('0');
			}
			else if (!ft_strchr(p->flags, '0'))
				put_sign(n, p);
			ft_putnbr(ft_absint(n));
		}
		else
		{
			p->printed += MAX(p->length, p->precision);
			put_sign(n, p);
			while (p->precision > p->length)
			{
				ft_putchar('0');
				p->precision--;
			}
			ft_putnbr(ft_absint(n));
		}
	}
}

void	print_float(va_list valist, t_parse *p)
{
	(void) valist;
	(void) p;
}

/*
**	prints one argument
*/

void	print_arg(t_parse *p, va_list valist)
{
	//types = "%diufFeEgGxXoscpaAn";
	if (ft_strchr("diuxX", p->type))
		print_int(valist, p);
	else if ('c' == p->type)
		print_char(valist, p);
	else if ('s' == p->type)
		print_str(valist, p);
	else if ('%' == p->type)
		ft_putchar('%');
	else if (ft_strchr("fFgG", p->type))
		print_float(valist, p);
}

/*
**	checks size of a type, uses flags in p node
*/

void	check_size(t_parse *p, char *tmp)
{
	char	*sizes;
	int		index;

	sizes = "hlLzjt";
	index = ft_strchrn(sizes, *tmp);
	if (ft_strchr(sizes, *tmp) == ft_strchr(sizes, *(tmp + 1)))
	{
		if (index == 0)
			p->is_signed = 1;
		else if (index == 1)
			p->is_longlong = 1;
		tmp++;
	}
	else if (index == 0)
		p->is_short = 1;
	else if (index == 1)
		p->is_long = 1;
	else if (index == 2)
		p->is__int64 = 1;
	else if (index == 3)
		p->is_size_t = 1;
	else if (index == 4)
		p->is_intmax_t = 1;
	else if (index == 5)
		p->is_ptrdiff_t = 1;
}

char	*read_flags(char *tmp, t_parse *p)
{
	char *f;

	p->flags = ft_strnew(5);
	f = p->flags;
	while (ft_strchr("-+ 0#", *tmp))
	{
		if ((*(ft_strchr("-+ 0#", *tmp)) == '0') &&
			(ft_strchr("-+ 0#", *(tmp + 1)) == NULL) &&
			(ft_strchr(p->flags, '0') != NULL))
			return (--tmp);
		if (!ft_strchr(p->flags, *tmp))
		{
			*f = *tmp;
			f++;
		}
		tmp++;
	}
	return (--tmp);
}

/*
**	parses string till type indetifier,
**	returns list node with all parameters and pointer to next symbol
*/

t_parse	*parse_string(char *tmp, t_parse *p, va_list valist)
{
	int		stop;

	stop = 0;
	while (!stop && *tmp)
	{
		if (!p->flags && ft_strchr("-+ 0#", *tmp))				//flag
			tmp = read_flags(tmp, p);
		else if (ft_atoi(tmp))	//width
		{
			p->width = ft_atoi(tmp);
			tmp += int_length(p->width) - 1;
		}
		else if (*tmp == '*')
			p->width = va_arg(valist, int);
		else if (*tmp == '.')						//precision
		{
			if (*(tmp + 1) == '*')
			{
				p->precision = va_arg(valist, int);
				tmp++;
			}
			else if ((p->precision = ft_atoi(tmp + 1)))
				tmp += int_length(p->precision);
		}
		else if (ft_strchr("hlLzjt", *tmp))			//size
			check_size(p, tmp);
		else if (ft_strchr("%diufFeEgGxXoscpaAn", *tmp))		//type
		{
			p->type = *tmp;
			p->next = tmp;
			stop = 1;
		}
		++tmp;
	}
	return (p);
}

int		ft_printf(const char *restrict s, ...)
{
	va_list	valist;
	t_parse	*p;
	char	*tmp;
	int		printed;

	tmp = (char*)s;
	va_start(valist, s);
	p = new_param();
	while (*tmp)
	{
		if (*tmp != '%')
		{
			ft_putchar(*tmp);
			p->printed++;
		}
		else
		{
			tmp++;
			parse_string(tmp, p, valist);
			tmp = p->next;
			print_arg(p, valist);
			clear_param(p);
		}
		tmp++;
	}
	printed = p->printed;
	del_param(p);
	return (printed);
}
