/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elbarry <elbarry@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/29 10:40:10 by elbarry           #+#    #+#             */
/*   Updated: 2026/10/02 17:24:13 by elbarry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	start_error(t_simulation *sim)
{
	stop_and_wake(sim);
	join_threads(sim);
	if (sim->thread_creation_failed)
		fprintf(stderr, "Error: unable to create all coder threads\n");
	destroy_simulation(sim);
}

int	main(int argc, char **argv)
{
	t_simulation	sim;
	int				rc;

	memset(&sim, 0, sizeof(sim));
	if (parse_args(argc, argv, &sim))
		return (1);
	if (init_simulation(&sim))
	{
		destroy_simulation(&sim);
		return (1);
	}
	if (start_threads(&sim))
	{
		start_error(&sim);
		return (1);
	}
	join_threads(&sim);
	rc = sim.initialization_failed;
	destroy_simulation(&sim);
	return (rc);
}
