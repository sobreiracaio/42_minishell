/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaosilva <joaosilva@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 00:27:21 by crocha-s          #+#    #+#             */
/*   Updated: 2024/06/13 01:24:42 by joaosilva        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Esta função define perante que tipo de built-in estamos presentes. Se o built-in for echo, a função ms_echo é chamada. Se o built-in for cd, a função ms_cd é chamada. Se o built-in for pwd, a função ms_pwd é chamada. Se o built-in for export, a função ms_export é chamada. Se o built-in for unset, a função ms_unset é chamada. Se o built-in for env, a função ms_env é chamada. Se o built-in for exit, a função ms_exit é chamada.
int	run_builtin(t_shell *shell, t_exec *cmd)
{
	if (!ft_strcmp(cmd->argv[0], "echo"))
		return (ms_echo(cmd), 1);
	else if (!ft_strcmp(cmd->argv[0], "cd"))
		return (ms_cd(shell, cmd), 1);
	else if (!ft_strcmp(cmd->argv[0], "pwd"))
		return (ms_pwd(shell, cmd), 1);
	else if (!ft_strcmp(cmd->argv[0], "export"))
		return (ms_export(shell, cmd), 1);
	else if (!ft_strcmp(cmd->argv[0], "unset"))
		return (ms_unset(shell, cmd), 1);
	else if (!ft_strcmp(cmd->argv[0], "env"))
		return (ms_env(shell, cmd), 1);
	else if (!ft_strcmp(cmd->argv[0], "exit"))
		return (ms_exit(shell, cmd), 1);
	return (0);
}
