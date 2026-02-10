/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmousli <mmousli@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 09:37:21 by mmousli           #+#    #+#             */
/*   Updated: 2026/02/10 09:37:53 by mmousli          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

static char	bits_to_char(int bits[8])
{
	char	c;
	int		i;

	c = 0;
	i = 0;
	while (i < 8)
	{
		c = c * 2 + bits[i];
		i++;
	}
	return (c);
}

static void	handler(int signum, siginfo_t *info, void *context)
{
	static int	bits[8];
	static int	index = 0;
	char		c;

	(void)context;
	if (signum == SIGUSR1)
		bits[index] = 0;
	else if (signum == SIGUSR2)
		bits[index] = 1;
	index++;
	if (index == 8)
	{
		c = bits_to_char(bits);
		if (c == '\0')
			write(1, "\n", 1);
		else
			write(1, &c, 1);
		index = 0;
	}
	kill(info->si_pid, SIGUSR1);
}

static void	init_server(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_sigaction = handler;
	sa.sa_flags = SA_SIGINFO;
	sigaction(SIGUSR1, &sa, NULL);
	sigaction(SIGUSR2, &sa, NULL);
}

int	main(void)
{
	int	pid;

	pid = getpid();
	write(1, "PID : ", 6);
	ft_putnbr(pid);
	write(1, "\n", 1);
	init_server();
	while (1)
		pause();
	return (0);
}
