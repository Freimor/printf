/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akraig <akraig@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/27 21:07:11 by akraig            #+#    #+#             */
/*   Updated: 2019/12/05 21:06:16 by akraig           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdio.h>
# include <stdlib.h>
# include <stdarg.h>
# include "libft.h"

/*
**	type:
**	a - 0	hex in form 0xh.hhhhp+d (только C99)
**	A - 1	hex in form 0Xh.hhhhP+d (только C99)
**	c - 2	symbol	c
**	d - 3	decimal int	42
**	i - 4	decimal int 42
**	e - 5	exponent 2.21e4
**	E - 6	exponent 2.21E4
**	f - 7	float decimal
**	F - 8	float decimal, 
**	g - 9	shortest of %e or %f (truncates 0 at the end
**				and looks like rounds to 2 signs after decimal point)
**				C99: INF, INFINITY in case of infinity 
**				or NAN in case of not a number
**	G - 10	shortest of %E or %F (truncates 0 at the end
**				and looks like rounds to 2 signs after decimal point)
**	o - 11	octal unsigned
**	s - 12	sting (putstr)
**	u - 13	decimal unsigned
**	x - 14	hex unsigned (small letters)
**	X - 15	hex unsigned (capital letters)
**	p - 16	pointer
**	n - 17	pointer to store number of alredy outputed symbols before %n
**	% - 18	just % sign
**	< - BONUS ADD: repeat last argument
*/

typedef struct		s_parse
{
	char 			*type;
	char 			length;
	char			flag;
	int				width;
	int				precision;
	int				align;
	char			*next;
}					t_parse;

/*
**	types
**  1     %с	Символ типа char
**  2     %d	Десятичное число целого типа со знаком
**  3     %i	Десятичное число целого типа со знаком
**  4     %е	Научная нотация (е нижнего регистра)
**  5     %Е	Научная нотация (Е верхнего регистра)
**  6     %f	Десятичное число с плавающей точкой
**  7     %g	Использует код %е или %f — тот из них, который
**              короче (при использовании %g используется е нижнего регистра)
**  8     %G	Использует код %Е или %f — тот из них, который
**              короче (при использовании %G используется Е верхнего регистра)
**  9     %о	Восьмеричное целое число без знака
**  10    %s	Строка символов
**  11    %u	Десятичное число целого типа без знака
**  12    %х	Шестнадцатиричное целое число без знака (нижний регистр)
**  13    %Х	Шестнадцатиричное целое число без знака (верхний регистр)
**  14    %р	Выводит на экран значение указателя
**  15    %n	указатель на переменную целого типа, в которую 
**              помещено количество символов, записанных на данный момент
*/

typedef struct		s_lst
{
	char			*type;
	int 			mod;
	char			*s;
	char			c;
	int				n;
	float			f;
	struct s_lst	*next;
	struct s_lst	*prev;
}					t_node;

int		ft_printf(const char * restrict format, ...);
int		ft_sum(int num, ...);
t_parse	*parse_string(char *s, t_parse *params);

#endif