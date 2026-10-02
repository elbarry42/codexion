/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elbarry <elbarry@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/29 10:40:10 by elbarry           #+#    #+#             */
/*   Updated: 2026/10/02 14:31:47 by elbarry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

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
		stop_and_wake(&sim);
		join_threads(&sim);
		destroy_simulation(&sim);
		return (1);
	}
	join_threads(&sim);
	rc = sim.initialization_failed;
	destroy_simulation(&sim);
	return (rc);
}
