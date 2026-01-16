/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruisilva <ruisilva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 18:07:12 by ruisilva          #+#    #+#             */
/*   Updated: 2026/01/16 18:47:22 by ruisilva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	cleanup(t_data *data);

int	main(int argc, char **argv)
{
	t_data	data;
	int		i;

	(void)argc;
	i = 0;
	if (parse_args(&data, argv) == 1)
		return (fprintf(stderr, "ERROR: Parsing Error"), 1);
	if (init_data(&data) == 1)
		return (fprintf(stderr, "ERROR: Malloc Error"), 1);
	data.start_time = get_time();
	i = 0;
	while (i < data.number_of_coders)
	{
		data.coders[i].last_compile_time = data.start_time;
		i++;
	}
	create_threads(&data);
	join_threads(&data);
	cleanup(&data);
}

static void	cleanup(t_data *data)
{
	int i;

	i = 0;
	while (i < data->number_of_coders)
	{
		pthread_mutex_destroy(&data->dongles[i].lock);
		i++;
	}
	pthread_mutex_destroy(&data->print_lock);
	pthread_mutex_destroy(&data->sim_lock);
	free(data->coders);
	free(data->dongles);
}