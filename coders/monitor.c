/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruisilva <ruisilva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 17:14:59 by ruisilva          #+#    #+#             */
/*   Updated: 2026/01/16 18:06:12 by ruisilva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	monitor_cycle(t_data *data);

void	*monitor_routine(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	while (!is_simulation_over(data))
	{
		if (monitor_cycle(data) == data->number_of_coders)
		{
			pthread_mutex_lock(&data->print_lock);
			data->is_over = 1;
			pthread_mutex_unlock(&data->print_lock);
		}
		usleep(1000);
	}
	return (NULL);
}

static int	monitor_cycle(t_data *data)
{
	int		i;
	int		finished_coders;
	t_coder	*coder;

	i = 0;
	finished_coders = 0;
	while (i < data->number_of_coders)
	{
		coder = &data->coders[i];
		if ((get_time() - get_last_compile_time(coder) > data->time_to_burnout)
			&& get_compile_count(coder) < data->compiles_required)
		{
			pthread_mutex_lock(&data->print_lock);
			printf("%lld %d %s\n", get_time() - data->start_time, coder->id,
				"burned out");
			data->is_over = 1;
			pthread_mutex_unlock(&data->print_lock);
			break ;
		}
		if (get_compile_count(coder) == data->compiles_required)
			finished_coders++;
		i++;
	}
	return (finished_coders);
}
