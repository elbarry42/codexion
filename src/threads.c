/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elbarry <elbarry@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/29 10:40:58 by elbarry           #+#    #+#             */
/*   Updated: 2026/10/02 16:06:08 by elbarry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	start_coder_threads(t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->coder_count)
	{
		if (pthread_create(&sim->coders[i].thread, NULL,
				coder_routine, &sim->coders[i]))
		{
			sim->thread_creation_failed = 1;
			return (1);
		}
		sim->coder_threads_created++;
		i++;
	}
	return (0);
}
