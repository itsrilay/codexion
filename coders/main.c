/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruisilva <ruisilva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 18:07:12 by ruisilva          #+#    #+#             */
/*   Updated: 2026/01/20 17:40:23 by ruisilva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	cleanup(t_data *data);
static int	create_threads(t_data *data);
static void	join_threads(t_data *data);

int	main(int argc, char **argv)
{
	t_data	data;
	int		i;

	if (argc != 9)
		return (fprintf(stderr, "ERROR: Wrong number of arguments\n"), 1);
	if (parse_args(&data, argv))
		return (fprintf(stderr, "ERROR: Parsing Error\n"), 1);
	if (init_data(&data))
		return (fprintf(stderr, "ERROR: Malloc Error\n"), 1);
	data.start_time = get_time();
	i = 0;
	while (i < data.number_of_coders)
	{
		data.coders[i].last_compile_time = data.start_time;
		i++;
	}
	if (create_threads(&data))
	{
		join_threads(&data);
		return (fprintf(stderr, "ERROR: Thread Creation Error\n"), 1);
	}
	join_threads(&data);
	cleanup(&data);
}

static int	create_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->number_of_coders)
	{
		if (pthread_create(&data->coders[i].thread, NULL, coder_routine,
				&data->coders[i]) != 0)
		{
			data->number_of_coders = i;
			return (1);
		}
		i++;
	}
	if (pthread_create(&data->monitor, NULL, monitor_routine, data) != 0)
		return (1);
	data->monitor_created = 1;
	return (0);
}

static void	join_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->number_of_coders)
	{
		pthread_join(data->coders[i].thread, NULL);
		i++;
	}
	if (data->monitor_created == 1)
		pthread_join(data->monitor, NULL);
}

static void	cleanup(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->number_of_coders)
	{
		pthread_mutex_destroy(&data->dongles[i].lock);
		pthread_cond_destroy(&data->coders[i].wait_cond);
		i++;
	}
	pthread_mutex_destroy(&data->print_lock);
	pthread_mutex_destroy(&data->sim_lock);
	free(data->coders);
	free(data->dongles);
}
