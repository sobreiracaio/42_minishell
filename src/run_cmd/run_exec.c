/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_exec.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaosilva <joaosilva@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 00:23:51 by crocha-s          #+#    #+#             */
/*   Updated: 2024/06/13 01:10:55 by joaosilva        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Esta função verifica se houve erros na execução do comando. Verifica se o comando existe, se tem permissões de execução, se é um diretório ou se não foi encontrado. Se o comando for encontrado mas não tiver permissões de execução, é impresso "Permission denied". Se o comando for encontrado mas for um diretório, é impresso "Is a directory". Se o comando não for encontrado, é impresso "No such file or directory". Se o comando não for encontrado e não for um caminho absoluto, é impresso "command not found". É libertada a memória alocada para o caminho e para o comando e é chamada a função free_exit.
static void	check_execve_errors(t_shell *shell, char *path)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(path, STDERR_FILENO);
	if (!access(path, F_OK) && access(path, X_OK) && ft_strchr(path, '/'))
		ft_putendl_fd(": Permission denied", STDERR_FILENO);
	else if (!access(path, F_OK) && !access(path, X_OK) && path[0] != '.')
		ft_putendl_fd(": Is a directory", STDERR_FILENO);
	else if (ft_strchr(path, '/') || !env_get("PATH", shell))
		ft_putendl_fd(": No such file or directory", STDERR_FILENO);
	else
		ft_putendl_fd(": command not found", STDERR_FILENO);
	g_exit = 127 - ((!access(path, F_OK) && access(path, X_OK)) || !access(path,
				F_OK));
	free(path);
	free_exit(shell);
}

// Esta função retorna o caminho do comando.
static char	*get_path(t_shell *sh, char *cmd)
{
	int		i;
	char	*path;
	char	*path2;
	char	**paths;

	i = 0;
	path = NULL;
	path2 = NULL;
	if (ft_strchr("/.", cmd[0]) || !env_get("PATH", sh) || !ft_strcmp(cmd, "")) // Se o comando começar com uma barra ou ponto, ou se não houver PATH no envp, ou se o comando for uma string vazia.
		return (ft_strdup(cmd)); // Retorna o comando.
	paths = ft_split(env_get("PATH", sh), ':'); // Divide a variável de ambiente PATH em um array de strings. O delimitador é ':'. 
	while (paths[i]) // Enquanto houver caminhos no array.
	{
		path = ft_strjoin(paths[i], "/"); // Junta o caminho com uma barra.
		path2 = ft_strjoin(path, cmd); // Junta o caminho com o comando.
		free(path); // Libera a memória alocada para o caminho.
		path = NULL; // O caminho é NULL. E é null pq foi libertado mas ainda é necessário para a próxima iteração.
		if (!access(path2, F_OK))
			return (ft_free_array(paths), path2);
		free(path2);
		path2 = NULL;
		i++;
	}
	ft_free_array(paths);
	return (ft_strdup(cmd));
}

// Esta função expande os argumentos do comando. 
static void	expand_argv(t_shell *shell, char **argv)
{
	int		len;
	int		i;
	int		expanded;
	char	*tmp;

	if (!argv[0])
		return ;
	expanded = (ft_strchr(argv[0], '$') != 0); // Se houver um cifrão no primeiro argumento, esta função é definida como verdadeira, ou seja, 1.
	expand_arg(shell, &argv[0]); // Esta função expande o tilde e as variáveis de ambiente.
	len = ft_strlen(argv[0]);
	arg_insert_null(argv[0]); // Esta função insere um caractere nulo em cada espaço em branco que não está entre aspas. Se na parseexec retiramos os nulos para colocar espaços, aqui fazemos o contrário.
	trim_quotes(argv[0], &len); // Esta função remove as aspas de uma string.
	i = 1;
	tmp = argv[0];
	while ((tmp < argv[0] + len) && i < (MAXARGS - 1)) // Enquanto não chegarmos ao fim do argumento e o número de argumentos for menor que o máximo de argumentos que foi definido no ficheiro minishell.h, e que é de 50. Ou seja, só podemos ter no máximo 50 comandos executáveis. Caso contrário o programa não entra neste loop.
	{
		if (*(tmp) == '\0' && (ft_strcmp(argv[0], "printf") || i != 2)) // Se o caractere atual for um nulo e o comando não for printf ou o número de argumentos for diferente de 2. Este é o caso "martelado" para o printf só correr com um argumento, uma vez que o printf só pode printa um argumento. Até lhe podemos meter vários, mas ele só printa o primeiro.
			argv[i++] = tmp + 1;
		tmp++;
	}
	if (!argv[0][0] && expanded) // Isto foi feito para exemplos como o $EMPTY_VAR. Neste caso a função expande, mas verifica que a expansão não expande para lado nenhum uma vez que a variável não existe. Nestes casos dá free e coloca o argumento a NULL.
	{
		free(argv[0]);
		argv[0] = NULL;
	}
}

static void	check_exit_status(void)
{
	if (g_exit == 11 || g_exit == 139)
		ft_putendl_fd("Segmentation fault (core dumped)", STDERR_FILENO);
	else if (g_exit == 8 || g_exit == 136)
		ft_putendl_fd("Floating point exception (core dumped)", STDERR_FILENO);
}
// Esta função executa um comando. Se o comando for um comando interno, é executado. Se não for, é chamada a função get_path para obter o caminho do comando. Se o caminho for NULL, é impresso "command not found" e é chamada a função free_exit. Se o caminho for encontrado, é chamada a função execve para executar o comando. Se a execve falhar, é chamada a função check_execve_errors. Esta função executa o comando que advém da árvore binária (a descendent parsing tree), que é uma estrutura de dados que representa a ordem de execução dos comandos.
void	run_exec(t_shell *shell, t_exec *cmd)
{
	pid_t	pid;
	char	*path;

	expand_argv(shell, cmd->argv); // Esta função expande os argumentos do comando.
	if (!cmd->argv[0])
		return (g_exit = 0, (void)0);
	if (run_builtin(shell, cmd)) // Esta função verifica se o comando é um built-in e se for executa-o.
		return ;
	signal_handler(SIGCHILD); // Esta função trata os sinais, que mudam para o modo de execução de um processo filho.
	pid = check_fork(); // Esta função cria um processo filho.
	if (pid == 0) // Se o processo for o filho.
	{
		path = get_path(shell, cmd->argv[0]); // Esta função retorna o caminho do comando.
		execve(path, cmd->argv, shell->envp_char); // Esta função executa o comando.
		check_execve_errors(shell, path); // Esta função verifica se houve erros na execução do comando.
	}
	waitpid(pid, &g_exit, 0); // Esta função espera pelo processo filho.
	if (WIFEXITED(g_exit)) // Se o processo filho terminou normalmente.
		g_exit = WEXITSTATUS(g_exit); // Retorna o status de saída do processo filho.
	else if (WIFSIGNALED(g_exit)) // Se o processo filho foi encerrado por um sinal.
		g_exit = 128 + WTERMSIG(g_exit); // Retorna o sinal que encerrou o processo filho.
	check_exit_status(); // Esta função verifica se houve erros na execução do comando.
	signal_handler(SIGRESTORE); // Esta função trata os sinais, que mudam para o modo de execução de um processo pai.
}
