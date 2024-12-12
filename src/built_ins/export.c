/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaosilva <joaosilva@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 00:32:13 by crocha-s          #+#    #+#             */
/*   Updated: 2024/06/13 02:05:12 by joaosilva        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// A função print_envp_sorted imprime as variáveis de ambiente ordenadas. Se export for verdadeiro, a função imprime as variáveis de ambiente com "declare -x". Se export for falso, a função imprime as variáveis de ambiente sem "declare -x".
static void	print_envp_sorted(t_shell *shell, int export)
{
	t_env	*tmp;

	tmp = shell->env_list_sorted;
	while (tmp)
	{
		if (export)
		{
			if (tmp->visible)
				ft_printf("declare -x %s=\"%s\"\n", tmp->key, tmp->value);
			else if (!tmp->visible)
				ft_printf("declare -x %s\n", tmp->key);
		}
		else
		{
			if (tmp->visible)
				ft_printf("%s=\"%s\"\n", tmp->key, tmp->value);
		}
		tmp = tmp->next;
	}
	tmp = shell->env_list_sorted;
}

// Aqui é verificado se as variáveis de ambiente são válidas. Se as variáveis de ambiente forem válidas, a função retorna true. Se as variáveis de ambiente não forem válidas, a função retorna false.
static bool	valid_var(t_shell *shell, char *arg)
{
	int	i;

	i = 0;
	if (!ft_isalpha(arg[i]) && arg[i] != '_') // Se o primeiro caractere do argumento não for uma letra ou um "_".
		return (error_inside(shell, "export: ", arg, 1));
	while (arg[++i]) // Enquanto houver caracteres no argumento.
	{
		if (arg[i] == '=') // Se o caractere for um "=".
			break ;
		if (!ft_isalnum(arg[i]) && arg[i] != '_') // Se o caractere não for um alfanumérico ou um "_".
			return (error_inside(shell, "export: ", arg, 1)); // Imprime um erro.
	}
	return (true);
}
// Verifica se os argumentos são válidos. Se os argumentos forem válidos, a função retorna true. Se os argumentos não forem válidos, a função retorna false.
static bool	valid_args(char **argv)
{
	int		i;
	bool	flag;

	if (!argv[1])
		return (false);
	i = 0;
	flag = false;
	while (argv[++i])
	{
		if (*argv[i])
			flag = true;
	}
	return (flag);
}

// Esta função exporta as variáveis de ambiente. Se o comando não tiver argumentos, a função imprime as variáveis de ambiente. Se o comando tiver argumentos, a função exporta as variáveis de ambiente.
static void	run_export(t_shell *shell, t_exec *cmd)
{
	int		i;
	char	*value;
	char	**split;

	if (!valid_args(cmd->argv)) // Se os argumentos não forem válidos.
		print_envp_sorted(shell, 1); // Imprime as variáveis de ambiente.
	else
	{
		i = 0;
		while (cmd->argv[++i]) // Enquanto houver argumentos.
		{
			if (!*cmd->argv[i] || !valid_var(shell, cmd->argv[i])) // Se o argumento for vazio ou inválido,
				continue ;
			if (ft_strchr(cmd->argv[i], '=')) // Se o argumento tiver um "=".
			{
				split = ft_split(cmd->argv[i], '='); // Separa o argumento em duas partes.
				value = ft_strdup(ft_strchr(cmd->argv[i], '=') + 1); // Atribui a segunda parte do argumento a value.
				env_export(shell, split[0], value, 1); // Exporta a primeira parte do argumento e o value.
				ft_free_array(split);
				free(value);
			}
			else
				env_export(shell, cmd->argv[i], "", 0); // Se o argumento não tiver um "=", exporta o argumento, ou seja, a chave sem valor.
		}
	}
}

// A função ms_export verifica se o comando tem argumentos. Se o comando tiver argumentos, a função chama a função run_export. Se o comando não tiver argumentos, a função imprime as variáveis de ambiente.
void	ms_export(t_shell *shell, t_exec *cmd)
{
	run_export(shell, cmd);
	if (shell->status == CONTINUE)
		g_exit = 0;
}
