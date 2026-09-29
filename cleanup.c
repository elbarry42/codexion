/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elbarry <elbarry@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/29 10:39:22 by elbarry           #+#    #+#             */
/*   Updated: 2026/09/29 10:39:23 by elbarry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	destroy_coder_conds(t_simulation	*sim)
{
	int	i;

	i = 0;
	while (i < sim->coder_count)
	{
		if (sim->coders[i].cond_initialized)
			pthread_cond_destroy(&sim->coders[i].cond);
		i++;
	}
}

static void	destroy_dongles(t_simulation	*sim)
{
	int	i;

	i = 0;
	while (i < sim->coder_count)
	{
		if (sim->dongles[i].mutex_initialized)
			pthread_mutex_destroy(&sim->dongles[i].mutex);
		i++;
	}
}

void	destroy_simulation(t_simulation	*sim)
{
	if (sim->coders)
		destroy_coder_conds(sim);
	if (sim->dongles)
		destroy_dongles(sim);
	free(sim->coders);
	free(sim->dongles);
	heap_destroy(&sim->queue);
	if (sim->state_cond_initialized)
		pthread_cond_destroy(&sim->state_cond);
	if (sim->queue_cond_initialized)
		pthread_cond_destroy(&sim->queue_cond);
	if (sim->queue_mutex_initialized)
		pthread_mutex_destroy(&sim->queue_mutex);
	if (sim->output_mutex_initialized)
		pthread_mutex_destroy(&sim->output_mutex);
	if (sim->state_mutex_initialized)
		pthread_mutex_destroy(&sim->state_mutex);
}
