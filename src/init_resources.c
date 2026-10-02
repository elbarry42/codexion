/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_resources.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elbarry <elbarry@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/29 10:40:00 by elbarry           #+#    #+#             */
/*   Updated: 2026/10/02 14:31:47 by elbarry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	init_sync(t_simulation *sim)
{
	if (pthread_mutex_init(&sim->state_mutex, NULL))
		return (1);
	sim->state_mutex_initialized = 1;
	if (pthread_mutex_init(&sim->output_mutex, NULL))
		return (1);
	sim->output_mutex_initialized = 1;
	if (pthread_mutex_init(&sim->queue_mutex, NULL))
		return (1);
	sim->queue_mutex_initialized = 1;
	if (pthread_cond_init(&sim->queue_cond, NULL))
		return (1);
	sim->queue_cond_initialized = 1;
	if (pthread_cond_init(&sim->state_cond, NULL))
		return (1);
	sim->state_cond_initialized = 1;
	return (0);
}

static int	init_dongle(t_simulation *sim, int i)
{
	sim->dongles[i].id = i;
	if (pthread_mutex_init(&sim->dongles[i].mutex, NULL))
		return (1);
	sim->dongles[i].mutex_initialized = 1;
	return (0);
}

static int	init_coder(t_simulation *sim, int i)
{
	sim->coders[i].id = i + 1;
	sim->coders[i].sim = sim;
	sim->coders[i].request.coder = &sim->coders[i];
	if (pthread_cond_init(&sim->coders[i].cond, NULL))
		return (1);
	sim->coders[i].cond_initialized = 1;
	if (pthread_mutex_init(&sim->coders[i].cond_mutex, NULL))
		return (1);
	sim->coders[i].cond_mutex_initialized = 1;
	return (0);
}

int	init_resources(t_simulation *sim)
{
	int	i;

	sim->dongles = malloc(sim->coder_count * sizeof(*sim->dongles));
	sim->coders = malloc(sim->coder_count * sizeof(*sim->coders));
	if (sim->dongles)
		memset(sim->dongles, 0, sim->coder_count * sizeof(*sim->dongles));
	if (sim->coders)
		memset(sim->coders, 0, sim->coder_count * sizeof(*sim->coders));
	if (!sim->dongles || !sim->coders)
		return (1);
	i = 0;
	while (i < sim->coder_count)
	{
		if (init_dongle(sim, i) || init_coder(sim, i))
			return (1);
		i++;
	}
	return (0);
}
