/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruisilva <ruisilva@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 12:04:34 by ruisilva          #+#    #+#             */
/*   Updated: 2026/02/24 15:11:05 by ruisilva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	return_dongles(t_data *data, t_coder *coder);
static void	coder_actions(t_coder *coder);
static int	are_dongles_unavailable(t_coder *coder);
static int	wait_for_dongles(t_data *data, t_coder *coder);

void	*coder_routine(void *arg)
{
	t_coder	*coder;
	int		i;

	coder = (t_coder *)arg;
	if (coder->data->number_of_coders == 1)
	{
		pthread_mutex_lock(&coder->left_dongle->lock);
		usleep((coder->time_to_burnout * 1000) + 1000);
		pthread_mutex_unlock(&coder->left_dongle->lock);
		return (NULL);
	}
	i = 0;
	while (i < coder->compiles_required && is_simulation_over(coder->data) == 0)
	{
		if (wait_for_dongles(coder->data, coder))
			return (NULL);
		coder_actions(coder);
		i++;
	}
	return (NULL);
}

static void	return_dongles(t_data *data, t_coder *coder)
{
	t_wait_node	*node;

	pthread_mutex_lock(&data->sim_lock);
	coder->left_dongle->is_available = 1;
	coder->right_dongle->is_available = 1;
	pthread_mutex_unlock(&coder->left_dongle->lock);
	pthread_mutex_unlock(&coder->right_dongle->lock);
	coder->left_dongle->next_available_time = get_time()
		+ data->dongle_cooldown;
	coder->right_dongle->next_available_time = get_time()
		+ data->dongle_cooldown;
	node = data->queue_head;
	while (node)
	{
		pthread_cond_broadcast(&node->coder->wait_cond);
		node = node->next;
	}
	pthread_mutex_unlock(&data->sim_lock);
}

static void	coder_actions(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;

	first = coder->left_dongle;
	second = coder->right_dongle;
	if (coder->left_dongle < coder->right_dongle)
	{
		first = coder->right_dongle;
		second = coder->left_dongle;
	}
	pthread_mutex_lock(&first->lock);
	print_status("has taken a dongle", coder);
	pthread_mutex_lock(&second->lock);
	print_status("has taken a dongle", coder);
	print_status("is compiling", coder);
	set_last_compile_time(coder, get_time());
	smart_sleep(coder->time_to_compile);
	increment_compile_count(coder);
	return_dongles(coder->data, coder);
	print_status("is debugging", coder);
	smart_sleep(coder->time_to_debug);
	print_status("is refactoring", coder);
	smart_sleep(coder->time_to_refactor);
}

static int	are_dongles_unavailable(t_coder *coder)
{
	if (coder->left_dongle->is_available && coder->right_dongle->is_available
		&& get_time() >= coder->left_dongle->next_available_time
		&& get_time() >= coder->right_dongle->next_available_time)
		return (0);
	return (1);
}

static int	wait_for_dongles(t_data *data, t_coder *coder)
{
	struct timespec	ts;

	pthread_mutex_lock(&data->sim_lock);
	if (enqueue_coder(data, coder))
		return (pthread_mutex_unlock(&data->sim_lock), 1);
	while (are_dongles_unavailable(coder) && !is_simulation_over(data))
	{
		if (coder->left_dongle->is_available
			&& coder->right_dongle->is_available)
		{
			ts = get_next_available_timespec(coder);
			pthread_cond_timedwait(&coder->wait_cond, &data->sim_lock, &ts);
		}
		else
			pthread_cond_wait(&coder->wait_cond, &data->sim_lock);
	}
	if (!is_simulation_over(data))
	{
		coder->left_dongle->is_available = 0;
		coder->right_dongle->is_available = 0;
	}
	dequeue_coder(coder->data, coder);
	pthread_mutex_unlock(&data->sim_lock);
	return (is_simulation_over(data));
}
