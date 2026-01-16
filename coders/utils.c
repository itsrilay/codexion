/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruisilva <ruisilva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 11:52:14 by ruisilva          #+#    #+#             */
/*   Updated: 2026/01/16 18:17:12 by ruisilva         ###   ########.fr       */
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
