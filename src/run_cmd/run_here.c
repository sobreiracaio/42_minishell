/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_here.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaosilva <joaosilva@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 19:37:49 by crocha-s          #+#    #+#             */
/*   Updated: 2024/06/13 01:22:24 by joaosilva        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Esta função expande o here_doc. Ela percorre a linha e verifica se o caractere atual é um til ou uma variável de ambiente. Se o caractere atual for um til, a função expand é chamada para expandir o til. Se o caractere atual for uma variável de ambiente, a função expand é chamada para expandir a variável de ambiente.
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

// Esta função lê o here_doc. Ela abre o arquivo "here_doc" com o modo de leitura e escrita. Em seguida, ela chama a função dup2 para duplicar o descritor de arquivo de entrada e saída padrão. Em seguida, ela lê uma linha digitada pelo usuário. Se a linha for NULL, é impresso um erro. Se a linha for igual ao delimitador, a linha é liberada e a função é encerrada. Se a linha for diferente do delimitador, a função expand_heredoc é chamada para expandir as variáveis de ambiente e o til. Em seguida, a linha é impressa no arquivo "here_doc" e a memória alocada para a linha é liberada. Após o loop, o descritor de arquivo é fechado e a variável global g_exit é definida como 0. Por fim, a função free_exit é chamada.
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

// Esta função executa um comando de here_doc. Ela cria um processo filho, chama a função signal_handler para definir o sinal SIGHEREDOC, chama a função heredoc_reader para ler o here_doc e executa o comando. Após a execução do comando, o descritor de arquivo é restaurado.
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
