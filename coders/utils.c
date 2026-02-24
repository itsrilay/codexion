/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruisilva <ruisilva@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 11:52:14 by ruisilva          #+#    #+#             */
/*   Updated: 2026/02/24 15:11:11 by ruisilva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	smart_sleep(long long time)
{
	long long	start_time;

	start_time = get_time();
	while (get_time() - start_time < time)
		usleep(100);
}

void	print_status(char *str, t_coder *coder)
{
	long long	timestamp;

	timestamp = get_time() - coder->data->start_time;
	pthread_mutex_lock(&coder->data->print_lock);
	if (coder->data->is_over)
	{
		pthread_mutex_unlock(&coder->data->print_lock);
		return ;
	}
	printf("%lld %d %s\n", timestamp, coder->id, str);
	pthread_mutex_unlock(&coder->data->print_lock);
}

struct timespec	ms_to_timespec(long long timestamp_in_ms)
{
	struct timespec	ts;

	ts.tv_sec = timestamp_in_ms / 1000;
	ts.tv_nsec = (timestamp_in_ms % 1000) * 1000000;
	return (ts);
}

struct timespec	get_next_available_timespec(t_coder *coder)
{
	long long	max_available_time;
	long long	left_time;
	long long	right_time;

	left_time = coder->left_dongle->next_available_time;
	right_time = coder->right_dongle->next_available_time;
	if (left_time > right_time)
		max_available_time = coder->left_dongle->next_available_time;
	else
		max_available_time = coder->right_dongle->next_available_time;
	return (ms_to_timespec(max_available_time));
}
