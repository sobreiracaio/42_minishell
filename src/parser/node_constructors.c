/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_constructors.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaosilva <joaosilva@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 19:32:08 by joaosilva         #+#    #+#             */
/*   Updated: 2024/06/12 16:06:44 by joaosilva        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
// Aqui criamos o node here_cmd que é um comando global que contem um node do tipo here_cmd.
t_cmd	*here_cmd(t_cmd *cmd, char *eof)
{
	t_here	*here;
	t_cmd	*tmp;
	t_cmd	*tmp2;

	here = (t_here *)ft_calloc(1, sizeof(t_here));
	here->type = HERE_DOC;
	here->eof = ft_strdup(eof);
	here->mode = O_WRONLY | O_CREAT | O_TRUNC;
	here->fdin = dup(STDIN_FILENO);
	here->fdout = dup(STDOUT_FILENO);
	if (cmd->type == EXEC || cmd->type == REDIR)
		here->cmd = cmd;
	else
	{
		tmp = cmd;
		while (tmp->type != EXEC && tmp->type != REDIR)
		{
			tmp2 = tmp;
			tmp = ((t_redir *)tmp)->cmd;
		}
		((t_redir *)tmp2)->cmd = (t_cmd *)here;
		here->cmd = tmp;
		return (cmd);
	}
	return ((t_cmd *)here);
}

// Aqui criamos o node redir_cmd que é um comando global que contem um node do tipo redir_cmd.
t_cmd	*redir_cmd(t_cmd *cmd, char *file, int mode, int fd)
{
	t_redir	*redir;
	t_cmd	*tmp;
	t_cmd	*tmp2;

	redir = (t_redir *)ft_calloc(1, sizeof(t_redir));
	redir->type = REDIR;
	redir->file = ft_strdup(file);
	redir->mode = mode;
	redir->fd = fd;
	if (cmd->type == EXEC)
		redir->cmd = cmd;
	else
	{
		tmp = cmd;
		while (tmp->type != EXEC)
		{
			tmp2 = tmp;
			tmp = ((t_redir *)tmp)->cmd;
		}
		((t_redir *)tmp2)->cmd = (t_cmd *)redir;
		redir->cmd = tmp;
		return (cmd);
	}
	return ((t_cmd *)redir);
}

// Aqui criamos o node pipe_cmd que é um comando global que contem um node do tipo pipe_cmd.
t_cmd	*pipe_cmd(t_cmd *left, t_cmd *right)
{
	t_pipe	*cmd;

	cmd = (t_pipe *)ft_calloc(1, sizeof(t_pipe));
	cmd->type = PIPE;
	cmd->left = left;
	cmd->right = right;
	return ((t_cmd *)cmd);
}

// Aqui criamos o node exec_cmd que é um comando global que contem um node do tipo exec_cmd.
t_cmd	*exec_cmd(void)
{
	t_exec	*cmd;

	cmd = (t_exec *)ft_calloc(1, sizeof(t_exec)); // No vídeo do MIT, o velho faz malloc com bzero, mas aqui fazemos direto. Fazemos logo calloc, que é o mesmo que malloc com bzero.
	cmd->type = EXEC;
	return ((t_cmd *)cmd);
}
