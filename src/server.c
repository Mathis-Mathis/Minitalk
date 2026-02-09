/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmousli <mmousli@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 01:10:00 by mmousli           #+#    #+#             */
/*   Updated: 2026/01/23 15:50:13 by mmousli          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <unistd.h>
#include "minitalk.h"

static void	flush_buf(char *buf, int *idx)
{
	if (*idx > 0)
	{
		write(1, buf, *idx);
		*idx = 0;
	}
}

static void	handle_signal(int sig, siginfo_t *info, void *context)
{
	static unsigned char	c = 0;
	static int				bit = 0;
	static pid_t			client_pid = 0;
	static char				buf[1024];
	static int				idx = 0;

	(void)context;
	if (client_pid != info->si_pid)
	{
		client_pid = info->si_pid;
		c = 0;
		bit = 0;
		idx = 0;
	}
	if (sig == SIGUSR2)
		c |= (1 << (7 - bit));
	bit++;
	if (bit == 8)
	{
		if (c == '\0')
		{
			buf[idx++] = '\n';
			flush_buf(buf, &idx);
		}
		else
		{
			buf[idx++] = (char)c;
			if (idx == (int)sizeof(buf))
				flush_buf(buf, &idx);
		}
		c = 0;
		bit = 0;
	}
	kill(info->si_pid, SIGUSR1);
}

static void	setup_signals(void)
{
	struct sigaction	sa;

	sa.sa_sigaction = handle_signal;
	sigemptyset(&sa.sa_mask);
	sigaddset(&sa.sa_mask, SIGUSR1);
	sigaddset(&sa.sa_mask, SIGUSR2);
	sa.sa_flags = SA_SIGINFO | SA_RESTART;
	if (sigaction(SIGUSR1, &sa, NULL) == -1)
		write(2, "sigaction SIGUSR1 failed\n", 25);
	if (sigaction(SIGUSR2, &sa, NULL) == -1)
		write(2, "sigaction SIGUSR2 failed\n", 25);
}

int	main(void)
{
	int	pid;

	pid = getpid();
	write(1, "PID: ", 5);
	ft_putnbr(pid);
	write(1, "\n", 1);
	setup_signals();
	while (1)
		pause();
	return (0);
}
