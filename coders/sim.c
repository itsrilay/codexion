/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruisilva <ruisilva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 12:04:34 by ruisilva          #+#    #+#             */
/*   Updated: 2026/01/16 14:26:18 by ruisilva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*coder_routine(void *arg)
{
	t_coder	*coder;
	int		i;

	coder = (t_coder *)arg;
	printf("Coder %d is awake.\n", coder->id);
	i = 0;
	while (i < coder->compiles_required)
	{
		pthread_mutex_lock(&coder->left_dongle->lock);
		print_status("has taken a dongle", coder);
		pthread_mutex_lock(&coder->right_dongle->lock);
		print_status("has taken a dongle", coder);
		print_status("is compiling", coder);
		smart_sleep(coder->time_to_compile);
		pthread_mutex_unlock(&coder->left_dongle->lock);
		pthread_mutex_unlock(&coder->right_dongle->lock);
		print_status("is debugging", coder);
		smart_sleep(coder->time_to_debug);
		print_status("is refactoring", coder);
		smart_sleep(coder->time_to_refactor);
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
}
