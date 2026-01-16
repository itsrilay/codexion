/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruisilva <ruisilva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 12:04:34 by ruisilva          #+#    #+#             */
/*   Updated: 2026/01/16 18:47:07 by ruisilva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	coder_actions(t_coder *coder)
{
	if (coder->id % 2 == 0)
	{
		pthread_mutex_lock(&coder->left_dongle->lock);
		print_status("has taken a dongle", coder);
		pthread_mutex_lock(&coder->right_dongle->lock);
		print_status("has taken a dongle", coder);
	}
	else
	{
		pthread_mutex_lock(&coder->right_dongle->lock);
		print_status("has taken a dongle", coder);
		pthread_mutex_lock(&coder->left_dongle->lock);
		print_status("has taken a dongle", coder);
	}
	print_status("is compiling", coder);
	set_last_compile_time(coder, get_time());
	smart_sleep(coder->time_to_compile);
	increment_compile_count(coder);
	pthread_mutex_unlock(&coder->left_dongle->lock);
	pthread_mutex_unlock(&coder->right_dongle->lock);
	print_status("is debugging", coder);
	smart_sleep(coder->time_to_debug);
	print_status("is refactoring", coder);
	smart_sleep(coder->time_to_refactor);
}

static void	*coder_routine(void *arg)
{
	t_coder	*coder;
	int		i;

	coder = (t_coder *)arg;
	i = 0;
	while (i < coder->compiles_required && is_simulation_over(coder->data) == 0)
	{
		coder_actions(coder);
		i++;
	}
	return (NULL);
}

void	create_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->number_of_coders)
	{
		pthread_create(&data->coders[i].thread, NULL, coder_routine,
			&data->coders[i]);
		i++;
	}
	pthread_create(&data->monitor, NULL, monitor_routine, data);
}

void	join_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->number_of_coders)
	{
		pthread_join(data->coders[i].thread, NULL);
		i++;
	}
	pthread_join(data->monitor, NULL);
}
