/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crocha-s <crocha-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 19:39:14 by crocha-s          #+#    #+#             */
/*   Updated: 2024/06/09 21:05:51 by crocha-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static bool	check_exit_code(char *str)
{
	int	is_negative;
	int	len;

	is_negative = 0;
	while (*str == '+' || *str == '-' || *str == ' ')
	{
		if (*str == '-')
			is_negative = 1;
		str++;
	}
	while (*str == '0')
		str++;
	if (!ft_isdigit(*str))
		return (false);
	while (*str && ft_isdigit(*str))
		str++;
	len = str - str;
	if (len > 19)
		return (false);
	if (is_negative)
		return (ft_strncmp(str, "9223372036854775808", len) <= 0);
	else
		return (ft_strncmp(str, "9223372036854775807", len) <= 0);
}

void	ms_exit(t_shell *shell, t_exec *cmd)
{
	char	*err;

	err = NULL;
	if (cmd->argv[2])
		print_error(shell, cmd->argv[0], "too many arguments", 2);
	if (cmd->argv[1])
	{
		if (!check_exit_code(cmd->argv[1]) && ft_strcmp(cmd->argv[1], "0"))
		{
			err = ft_strjoin(cmd->argv[1], ": numeric argument required");
			print_error(shell, cmd->argv[0], err, 2);
			free(err);
		}
		else
			g_exit = ft_atoi(cmd->argv[1]) % 256;
	}
	shell->status = STOP;
}
