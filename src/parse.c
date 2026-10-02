/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elbarry <elbarry@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/29 10:40:27 by elbarry           #+#    #+#             */
/*   Updated: 2026/10/02 14:31:47 by elbarry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	parse_times(int argc, char **argv, t_simulation	*sim)
{
	int	ok;

	if (argc != 9)
		return (1);
	sim->time_to_burnout = parse_nonnegative_ll(argv[2], &ok);
	if (!ok)
		return (1);
	sim->time_to_compile = parse_nonnegative_ll(argv[3], &ok);
	if (!ok)
		return (1);
	sim->time_to_debug = parse_nonnegative_ll(argv[4], &ok);
	if (!ok)
		return (1);
	sim->time_to_refactor = parse_nonnegative_ll(argv[5], &ok);
	if (!ok)
		return (1);
	sim->dongle_cooldown = parse_nonnegative_ll(argv[7], &ok);
	return (!ok);
}

int	parse_args(int argc, char **argv, t_simulation	*sim)
{
	int	ok;

	if (argc != 9)
	{
		fprintf(stderr, "Usage: ./codexion number_of_coders time_to_burnout "
			"time_to_compile time_to_debug time_to_refactor "
			"number_of_compiles_required dongle_cooldown scheduler\n");
		return (1);
	}
	sim->coder_count = parse_positive_int(argv[1], &ok);
	if (!ok || parse_times(argc, argv, sim))
		return (fprintf(stderr, "Invalid numeric argument\n"), 1);
	sim->required_compiles = parse_positive_int(argv[6], &ok);
	if (!ok)
		return (fprintf(stderr, "Invalid number_of_compiles_required\n"), 1);
	if (!strcmp(argv[8], "fifo"))
		sim->scheduler = CODEXION_FIFO;
	else if (!strcmp(argv[8], "edf"))
		sim->scheduler = CODEXION_EDF;
	else
		return (fprintf(stderr, "Invalid scheduler: use fifo or edf\n"), 1);
	return (0);
}
