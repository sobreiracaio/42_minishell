/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crocha-s <crocha-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 18:49:24 by crocha-s          #+#    #+#             */
/*   Updated: 2024/05/26 18:28:02 by crocha-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ms_pwd(t_shell *shell, t_exec *cmd)
{
	char	*path;

	if (cmd->argv[1] && cmd->argv[1][0] == '-')
	{
		print_error(shell, cmd->argv[0], "flags not supported");
		return ;
	}
	path = getcwd(NULL, 0);
	if (!path)
		print_error(shell, cmd->argv[0], "couldn't get current directory");
	ft_putendl_fd(path, STDOUT_FILENO);
	free(path);
	if (shell->status == CONTINUE)
		g_exit = 0;
}