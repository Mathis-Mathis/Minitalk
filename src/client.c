/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmousli <mmousli@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 01:09:07 by mmousli           #+#    #+#             */
/*   Updated: 2026/01/23 15:49:56 by mmousli          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

static volatile sig_atomic_t	g_ack = 0;

static void	ack_handler(int sig)
{
	(void)sig;
	g_ack = 1;
}

static void	wait_ack(void)
{
	while (!g_ack)
		pause();
	g_ack = 0;
}

static int	ft_atoi_pid(const char *s)
{
	long	n;
	int		i;

	n = 0;
	i = 0;
	if (!s || !s[0])
		return (-1);
	while (s[i] == ' ' || (s[i] >= 9 && s[i] <= 13))
		i++;
	if (s[i] == '+')
		i++;
	if (s[i] < '0' || s[i] > '9')
		return (-1);
	while (s[i] >= '0' && s[i] <= '9')
	{
		n = n * 10 + (s[i] - '0');
		if (n > 2147483647)
			return (-1);
		i++;
	}
	if (s[i] != '\0')
		return (-1);
	return ((int)n);
}

static void	send_char(int pid, unsigned char c)
{
	int	bit;

	bit = 7;
	while (bit >= 0)
	{
		if ((c >> bit) & 1)
		{
			if (kill(pid, SIGUSR2) == -1)
			{
				write(2, "Error: kill failed\n", 19);
				exit(1);
			}
		}
		else
		{
			if (kill(pid, SIGUSR1) == -1)
			{
				write(2, "Error: kill failed\n", 19);
				exit(1);
			}
		}
		wait_ack();
		bit--;
	}
}

static void	setup_client_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = ack_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGUSR1, &sa, NULL) == -1)
	{
		write(2, "Error: sigaction failed\n", 24);
		exit(1);
	}
}

int	main(int argc, char **argv)
{
	int			pid;
	const char	*msg;
	int			i;

	if (argc != 3)
	{
		write(2, "Usage: ./client <server_pid> <message>\n", 39);
		return (1);
	}
	setup_client_signals();
	pid = ft_atoi_pid(argv[1]);
	if (pid <= 0)
	{
		write(2, "Error: invalid PID\n", 19);
		return (1);
	}
	if (kill(pid, 0) == -1)
	{
		write(2, "Error: server PID not reachable\n", 32);
		return (1);
	}
	msg = argv[2];
	i = 0;
	while (msg[i])
	{
		send_char(pid, (unsigned char)msg[i]);
		i++;
	}
	send_char(pid, '\0');
	return (0);
}
