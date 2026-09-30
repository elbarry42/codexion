/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elbarry <elbarry@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/29 10:40:16 by elbarry           #+#    #+#             */
/*   Updated: 2026/09/30 14:51:06 by elbarry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	is_burned(t_simulation *sim, t_coder *coder, long long now)
{
	return (!coder_done(sim, coder)
		&& now - coder_last_start(sim, coder) >= sim->time_to_burnout + 5);
}

static t_coder	*find_burnout(t_simulation *sim, long long now)
{
	int	i;

	i = 0;
	while (i < sim->coder_count)
	{
		if (is_burned(sim, &sim->coders[i], now))
			return (&sim->coders[i]);
		i++;
	}
	return (NULL);
}

static long long	bounded_monitor_wait(long long wait_ms)
{
	if (wait_ms < 1)
		return (1);
	if (wait_ms > 5)
		return (5);
	return (wait_ms);
}

void	*monitor_routine(void *arg)
{
	t_simulation	*sim;
	t_coder			*burned;
	long long		now;

	sim = (t_simulation *)arg;
	while (!simulation_stopped(sim))
	{
		now = monotonic_ms();
		burned = find_burnout(sim, now);
		if (burned)
		{
			log_state(sim, burned->id, "burned out");
			request_stop(sim);
			return (NULL);
		}
		monitor_wait(sim, bounded_monitor_wait(next_deadline(sim, now) - now));
	}
	return (NULL);
}
