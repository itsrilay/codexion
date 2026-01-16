/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruisilva <ruisilva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 16:46:17 by ruisilva          #+#    #+#             */
/*   Updated: 2026/01/16 13:41:23 by ruisilva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	init_coders_dongles(t_data *data);

int	parse_args(t_data *data, char **argv)
{
	data->number_of_coders = atoi(argv[1]);
	data->time_to_burnout = atoi(argv[2]);
	data->time_to_compile = atoi(argv[3]);
	data->time_to_debug = atoi(argv[4]);
	data->time_to_refactor = atoi(argv[5]);
	data->compiles_required = atoi(argv[6]);
	data->dongle_cooldown = atoi(argv[7]);
	if (strcmp(argv[8], "fifo") == 0)
		data->scheduler = 1;
	else if (strcmp(argv[8], "edf") == 0)
		data->scheduler = 2;
	else
		return (1);
	return (0);
}

int	init_data(t_data *data)
{
	int	i;

	i = 0;
	data->coders = malloc(sizeof(t_coder) * data->number_of_coders);
	if (!data->coders)
		return (1);
	data->dongles = malloc(sizeof(t_dongle) * data->number_of_coders);
	if (!data->dongles)
		return (free(data->coders), 1);
	init_coders_dongles(data);
	pthread_mutex_init(&data->print_lock, NULL);
	pthread_mutex_init(&data->sim_lock, NULL);
	return (0);
}

static void	init_coders_dongles(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->number_of_coders)
	{
		pthread_mutex_init(&data->dongles[i].lock, NULL);
		data->coders[i].id = i + 1;
		data->coders[i].time_to_burnout = data->time_to_burnout;
		data->coders[i].time_to_compile = data->time_to_compile;
		data->coders[i].time_to_debug = data->time_to_debug;
		data->coders[i].time_to_refactor = data->time_to_refactor;
		data->coders[i].compiles_required = data->compiles_required;
		data->coders[i].left_dongle = &data->dongles[i];
		data->coders[i].right_dongle = &data->dongles[(i + 1)
			% data->number_of_coders];
		data->coders[i].compile_count = 0;
		data->coders[i].last_compile_time = 0;
		data->coders[i].data = data;
		i++;
	}
}
