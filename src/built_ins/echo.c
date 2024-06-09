/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crocha-s <crocha-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 00:36:24 by crocha-s          #+#    #+#             */
/*   Updated: 2024/06/09 17:12:39 by crocha-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	check_flag(t_exec *cmd, int *arg_index, char *characters)
{
	int		flag;
	char	*str;
	char	*argv;
	char	*temp;

	argv = cmd->argv[*arg_index];
	flag = 0;
	while ((argv[++*arg_index]))
	{
		if (argv[0] != '-')
			break ;
		str = &argv[1];
		while (*str)
		{
			temp = characters;
			while (*temp && *temp != *str)
				temp++;
			if (!*temp)
				return (flag);
			str++;
		}
		flag = 1;
	}
	return (flag);
}

void	ms_echo(t_exec *cmd)
{
	int	i;
	int	flag;

	i = 0;
	flag = check_flag(cmd, &i, "n");
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
