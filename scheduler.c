/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elbarry <elbarry@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/29 10:40:40 by elbarry           #+#    #+#             */
/*   Updated: 2026/09/30 14:50:25 by elbarry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static t_request	*find_ready_request(t_simulation *sim,
	t_request **requests, size_t request_count)
{
	t_request	*best;
	long long	now;
	size_t	i;

	best = NULL;
	now = monotonic_ms();
	i = 0;
	while (i < request_count)
	{
		if (requests[i] && dongles_ready(sim, requests[i]->coder, now)
			&& (!best || heap_request_before(&sim->queue, requests[i], best)))
			best = requests[i];
		i++;
	}
	return (best);
}

static int	scheduler_step(t_simulation *sim, t_request **requests)
{
	t_request	*request;
	size_t		request_count;

	request_count = snapshot_requests(sim, requests);
	if (!request_count)
		return (0);
	request = find_ready_request(sim, requests, request_count);
	if (!request)
		return (0);
	if (!remove_request(sim, request))
		return (1);
	if (take_pair(sim, request->coder))
		return (grant_request(sim, request));
	requeue_request(sim, request);
	return (0);
}

static void	scheduler_loop(t_simulation *sim, t_request **requests)
{
	while (!simulation_stopped(sim))
	{
		if (!scheduler_step(sim, requests))
			scheduler_wait(sim);
	}
}

void	*scheduler_routine(void	*arg)
{
	t_simulation	*sim;
	t_request		**requests;

	sim = (t_simulation *)arg;
	requests = malloc((size_t)sim->coder_count * sizeof(*requests));
	if (!requests)
	{
		request_stop(sim);
		return (NULL);
	}
	scheduler_loop(sim, requests);
	free(requests);
	return (NULL);
}
