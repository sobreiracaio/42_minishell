/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crocha-s <crocha-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 22:57:29 by crocha-s          #+#    #+#             */
/*   Updated: 2024/06/11 00:41:19 by crocha-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static bool	ft_isnumber(char *str)
{
	int	i;

	i = 0;
	if (!str[i])
		return (false);
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i++]))
			return (false);
	}
	return (true);
}

static bool	islonglong(char *str)
{
	int	is_negative;
	int	len;

	if (*str == '-')
	{
		is_negative = 1;
		str++;
	}
	while (*str == '0' || *str == ' ')
		str++;
	if (!ft_isnumber(str))
		return (false);
	len = ft_strlen(str);
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
		if (!islonglong(cmd->argv[1]) && ft_strcmp(cmd->argv[1], "0"))
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
