/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruisilva <ruisilva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 18:07:12 by ruisilva          #+#    #+#             */
/*   Updated: 2026/01/16 13:58:08 by ruisilva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

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
	while (i < data.number_of_coders)
	{
		printf("--- Coder %d ---\n", data.coders[i].id);
		printf("  Compile Count: %d\n", data.coders[i].compile_count);
		printf("  Time to Burnout: %lld\n", data.coders[i].time_to_burnout);
		printf("  Time to Compile: %d\n", data.coders[i].time_to_compile);
		printf("  Time to Debug: %d\n", data.coders[i].time_to_debug);
		printf("  Time to Refactor: %d\n", data.coders[i].time_to_refactor);
		printf("  Required Compiles: %d\n", data.coders[i].compiles_required);
		printf("  Last Compile Time: %lld\n", data.coders[i].last_compile_time);
		printf("  Left Dongle Addr: %p\n", (void *)data.coders[i].left_dongle);
		printf("  Right Dongle Addr: %p\n\n",
			(void *)data.coders[i].right_dongle);
		i++;
	}
	data.start_time = get_time();
	printf("Current time: %lld ms\n", data.start_time);
	create_threads(&data);
	join_threads(&data);
}
