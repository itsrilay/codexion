/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_set.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruisilva <ruisilva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 17:41:56 by ruisilva          #+#    #+#             */
/*   Updated: 2026/01/19 16:47:17 by ruisilva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long long	get_last_compile_time(t_coder *coder)
{
	long long	last_compile_time;

	pthread_mutex_lock(&coder->data->sim_lock);
	last_compile_time = coder->last_compile_time;
	pthread_mutex_unlock(&coder->data->sim_lock);
	return (last_compile_time);
}

void	set_last_compile_time(t_coder *coder, long long time)
{
	pthread_mutex_lock(&coder->data->sim_lock);
	coder->last_compile_time = time;
	pthread_mutex_unlock(&coder->data->sim_lock);
}

int	get_compile_count(t_coder *coder)
{
	int	compile_count;

	pthread_mutex_lock(&coder->data->sim_lock);
	compile_count = coder->compile_count;
	pthread_mutex_unlock(&coder->data->sim_lock);
	return (compile_count);
}

void	increment_compile_count(t_coder *coder)
{
	pthread_mutex_lock(&coder->data->sim_lock);
	coder->compile_count++;
	pthread_mutex_unlock(&coder->data->sim_lock);
}

int	is_simulation_over(t_data *data)
{
	int	is_over;

	pthread_mutex_lock(&data->print_lock);
	is_over = data->is_over;
	pthread_mutex_unlock(&data->print_lock);
	return (is_over);
}
