/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmousli <mmousli@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 00:00:00 by mmousli           #+#    #+#             */
/*   Updated: 2026/01/21 00:33:25 by mmousli          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

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
				exit(1);
		}
		else
		{
			if (kill(pid, SIGUSR1) == -1)
				exit(1);
		}
		usleep(500);
		bit--;
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
	pid = ft_atoi_pid(argv[1]);
	if (pid <= 0)
	{
		write(2, "Error: invalid PID\n", 19);
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
