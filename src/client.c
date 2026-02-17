/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmousli <mmousli@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 09:36:12 by mmousli           #+#    #+#             */
/*   Updated: 2026/02/17 16:40:50 by mmousli          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

static volatile sig_atomic_t	g_ack = 0;

static void	ack_handler(int signum)
{
	if (signum == SIGUSR1)
		g_ack = 1;
}

static void	init_client(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_handler = ack_handler;
	sa.sa_flags = 0;
	sigaction(SIGUSR1, &sa, NULL);
}

static void	send_char_ack(int pid, unsigned char c)
{
	int	bit;

	bit = 7;
	while (bit >= 0)
	{
		g_ack = 0;
		if (((c >> bit) & 1) == 1)
			kill(pid, SIGUSR2);
		else
			kill(pid, SIGUSR1);
		while (g_ack == 0)
			pause();
		bit--;
	}
}

static void	send_msg(int pid, char *msg)
{
	int	i;

	i = 0;
	while (msg[i])
	{
		send_char_ack(pid, (unsigned char)msg[i]);
		i++;
	}
	send_char_ack(pid, '\0');
}

int	main(int argc, char **argv)
{
	int		pid;

	if (argc != 3)
		return (write(2, "need to : ./client <PID> <message>\n", 36), 1);
	pid = ft_atoi(argv[1]);
	if (pid <= 0)
		return (write(2, "Error : Invalid PID\n", 21), 1);
	init_client();
	send_msg(pid, argv[2]);
	return (0);
}
