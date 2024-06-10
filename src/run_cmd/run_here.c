/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_here.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crocha-s <crocha-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 19:37:49 by crocha-s          #+#    #+#             */
/*   Updated: 2024/06/10 19:28:16 by crocha-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	expand_heredoc(t_shell *shell, char **line)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while ((*line)[i])
	{
		if ((*line)[i] == '$' && (*line)[i + 1] == '?')
		{
			tmp = ft_itoa(g_exit);
			expand(tmp, i, i + 2, line);
			free(tmp);
		}
		else if ((*line)[i] == '$' && ft_isalpha((*line)[i + 1]))
		{
			j = i + 1;
			while (ft_isalnum((*line)[j]) || (*line)[j] == '_')
				j++;
			tmp = ft_substr(*line, i + 1, j - i - 1);
			expand(env_get(tmp, shell), i, j, line);
			free(tmp);
		}
		i++;
	}
}

static void	heredoc_reader(t_shell *shell, t_here *here, int fd)
{
	char	*line;

	fd = open("here_doc", here->mode, 0644);
	dup2(here->fdin, STDIN_FILENO);
	dup2(here->fdout, STDOUT_FILENO);
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			ft_printf(ERROR_TITLE ERROR_HERE_DOC "%s'\n", here->eof);
			break ;
		}
		if (ft_strcmp(line, here->eof) == 0)
		{
			free(line);
			break ;
		}
		expand_heredoc(shell, &line);
		ft_putendl_fd(line, fd);
		free(line);
	}
	close(fd);
	g_exit = 0;
	free_exit(shell);
}

void	run_heredoc(t_shell *shell, t_here *here)
{
	int		fd;
	int		len;
	pid_t	pid;

	len = ft_strlen(here->eof);
	trim_quotes(here->eof, &len);
	pid = check_fork();
	if (pid == 0)
	{
		signal_handler(SIGHEREDOC);
		heredoc_reader(shell, here, 0);
	}
	waitpid(pid, &g_exit, 0);
	g_exit = WEXITSTATUS(g_exit);
	fd = open("here_doc", O_RDONLY);
	dup2(fd, STDIN_FILENO);
	close(fd);
	if (g_exit == 0)
		run_cmd(shell, here->cmd);
	dup2(here->fdin, STDIN_FILENO);
	unlink("here_doc");
}