/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   numeric.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elbarry <elbarry@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/29 10:40:22 by elbarry           #+#    #+#             */
/*   Updated: 2026/10/02 14:31:47 by elbarry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	is_digits(const char	*s)
{
	size_t	i;

	if (!s || !*s)
		return (0);
	i = 0;
	while (s[i])
	{
		if (s[i] < '0' || s[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

long long	parse_nonnegative_ll(const char	*s, int	*ok)
{
	unsigned long long	value;
	unsigned long long	digit;
	size_t				i;

	*ok = 0;
	if (!is_digits(s))
		return (0);
	value = 0;
	i = 0;
	while (s[i])
	{
		digit = (unsigned long long)(s[i] - '0');
		if (value > ((unsigned long long)LLONG_MAX - digit) / 10ULL)
			return (0);
		value = value * 10ULL + digit;
		i++;
	}
	*ok = 1;
	return ((long long)value);
}

long long	parse_positive_ll(const char	*s, int	*ok)
{
	long long	value;

	value = parse_nonnegative_ll(s, ok);
	if (!*ok || value == 0)
		*ok = 0;
	return (value);
}

int	parse_positive_int(const char	*s, int	*ok)
{
	long long	value;

	value = parse_positive_ll(s, ok);
	if (!*ok || value > INT_MAX)
	{
		*ok = 0;
		return (0);
	}
	return ((int)value);
}
