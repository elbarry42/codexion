/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sync.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elbarry <elbarry@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/29 10:40:51 by elbarry           #+#    #+#             */
/*   Updated: 2026/09/29 12:43:09 by elbarry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	append_number(char *buffer, int pos, long long value)
{
	char	digits[24];
	int		i;

	i = 0;
	if (!value)
		digits[i++] = '0';
	while (value > 0)
	{
		digits[i++] = (char)('0' + value % 10);
		value /= 10;
	}
	while (i > 0)
		buffer[pos++] = digits[--i];
	return (pos);
}

static int	append_text(char *buffer, int pos, const char *text)
{
	int	i;

	i = 0;
	while (text[i])
		buffer[pos++] = text[i++];
	return (pos);
}

static int	append_log_prefix(char *buffer, int pos, long long timestamp,
	int coder_id)
{
	pos = append_number(buffer, pos, timestamp);
	buffer[pos++] = ' ';
	pos = append_number(buffer, pos, coder_id);
	buffer[pos++] = ' ';
	return (pos);
}

void	log_state(t_simulation *sim, int coder_id, const char *message)
{
	char	buffer[128];
	int		len;

	len = append_log_prefix(buffer, 0, elapsed_ms(sim), coder_id);
	len = append_text(buffer, len, message);
	buffer[len++] = '\n';
	pthread_mutex_lock(&sim->output_mutex);
	write(1, buffer, len);
	pthread_mutex_unlock(&sim->output_mutex);
}

void	log_compile_start(t_simulation *sim, int coder_id)
{
	char		buffer[256];
	long long	timestamp;
	int			len;

	timestamp = elapsed_ms(sim);
	len = append_log_prefix(buffer, 0, timestamp, coder_id);
	len = append_text(buffer, len, "has taken a dongle");
	buffer[len++] = '\n';
	len = append_log_prefix(buffer, len, timestamp, coder_id);
	len = append_text(buffer, len, "has taken a dongle");
	buffer[len++] = '\n';
	len = append_log_prefix(buffer, len, timestamp, coder_id);
	len = append_text(buffer, len, "is compiling");
	buffer[len++] = '\n';
	pthread_mutex_lock(&sim->output_mutex);
	write(1, buffer, len);
	pthread_mutex_unlock(&sim->output_mutex);
}
