/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elbarry <elbarry@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/29 10:39:28 by elbarry           #+#    #+#             */
/*   Updated: 2026/09/29 12:59:41 by elbarry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	sleep_phase(t_simulation *sim, long long duration)
{
	long long	end;
	long long	remaining;

	end = monotonic_ms() + duration;
	while (!simulation_stopped(sim))
	{
		remaining = end - monotonic_ms();
		if (remaining <= 0)
			return (1);
		if (remaining > 5)
			remaining = 5;
		usleep((useconds_t)(remaining * 1000LL));
	}
	return (0);
}

static int	run_compile(t_coder	*coder)
{
	t_simulation	*sim;

	sim = coder->sim;
	pthread_mutex_lock(&sim->queue_mutex);
	coder->last_compile_start = monotonic_ms();
	pthread_mutex_unlock(&sim->queue_mutex);
	log_compile_start(sim, coder->id);
	if (!sleep_phase(sim, sim->time_to_compile))
	{
		release_pair(sim, coder);
		return (0);
	}
	release_pair(sim, coder);
	coder->compile_count++;
	return (1);
}

static int	run_after_compile(t_coder	*coder)
{
	t_simulation	*sim;

	sim = coder->sim;
	if (coder->compile_count >= sim->required_compiles)
	{
		mark_coder_complete(sim, coder);
		return (0);
	}
	log_state(sim, coder->id, "is debugging");
	if (!sleep_phase(sim, sim->time_to_debug))
		return (0);
	log_state(sim, coder->id, "is refactoring");
	return (sleep_phase(sim, sim->time_to_refactor));
}

static int	run_cycle(t_coder	*coder)
{
	if (!request_dongles(coder))
		return (0);
	if (!run_compile(coder))
		return (0);
	return (run_after_compile(coder));
}

void	*coder_routine(void	*arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	while (!simulation_stopped(coder->sim))
	{
		if (!run_cycle(coder))
			break ;
	}
	return (NULL);
}
