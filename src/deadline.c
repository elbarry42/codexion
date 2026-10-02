/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deadline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elbarry <elbarry@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/29 10:39:39 by elbarry           #+#    #+#             */
/*   Updated: 2026/10/02 14:31:47 by elbarry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long long	next_deadline(t_simulation *sim, long long now)
{
	int			i;
	long long	deadline;
	long long	next;

	next = now + 5;
	i = 0;
	while (i < sim->coder_count)
	{
		deadline = coder_last_start(sim, &sim->coders[i])
			+ sim->time_to_burnout;
		if (!coder_done(sim, &sim->coders[i]) && deadline < next)
			next = deadline;
		i++;
	}
	return (next);
}

void	monitor_wait(t_simulation *sim, long long wait_ms)
{
	pthread_mutex_lock(&sim->state_mutex);
	if (!sim->stopped)
		cond_wait_ms(&sim->state_cond, &sim->state_mutex, wait_ms);
	pthread_mutex_unlock(&sim->state_mutex);
}
