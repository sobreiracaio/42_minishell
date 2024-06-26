/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crocha-s <crocha-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 00:36:24 by crocha-s          #+#    #+#             */
/*   Updated: 2024/06/11 01:08:07 by crocha-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	check_flag(t_exec *cmd, int *arg_index)
{
	int		flag;
	int		i;
	char	*arg;

	flag = 0;
	arg = cmd->argv[++*arg_index];
	while (arg)
	{
		if (arg[0] == '-' && arg[1] == 'n')
		{
			i = 1;
			while (arg[i])
			{
				if (arg[i] != 'n')
					return (flag);
				i++;
			}
			flag = 1;
		}
		else
			break ;
		arg = cmd->argv[++*arg_index];
	}
	return (flag);
}

void	ms_echo(t_exec *cmd)
{
	int	i;
	int	flag;

	i = 0;
	flag = check_flag(cmd, &i);
	if (cmd->argv[i])
	{
		ft_putstr_fd(cmd->argv[i], STDOUT_FILENO);
		while (cmd->argv[++i])
		{
			ft_putchar_fd(' ', STDOUT_FILENO);
			ft_putstr_fd(cmd->argv[i], STDOUT_FILENO);
		}
	}
	if (!flag)
		ft_putchar_fd('\n', STDOUT_FILENO);
	g_exit = 0;
}
