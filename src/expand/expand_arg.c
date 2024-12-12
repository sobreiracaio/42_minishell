/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_arg.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaosilva <joaosilva@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 10:48:05 by joaosilva         #+#    #+#             */
/*   Updated: 2024/06/13 00:44:14 by joaosilva        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// A função pointt_to_exp_tilde verifica se o caractere til (~) está numa posição válida para expansão (não precedido por caracteres especiais).
// A função expand está a expandir uma variável de ambiente para o seu valor correspondente. A função env_get está a recuperar o valor da variável de ambiente especificada.
// Em resumo, esta função está a lidar com a expansão de til (~)  onde `, - e +` são substituídos pelo valor das variáveis de ambiente HOME, OLDPWD e PWD, respectivamente. 
//Parâmetros:
//sh: Ponteiro para a estrutura t_shell contendo informações do shell.
//point: Posição do caractere '~' na linha de comando original.
//tmp: Ponteiro para o caractere '~' na linha de comando expandida.
//line: Ponteiro para um ponteiro de caractere (char **) que aponta para a linha de comando a ser expandida.

static int	point_to_exp_tilde(t_shell *sh, int point, char *tmp, char **line)
{
	if (!tmp[1] || ft_strchr(NOT_EXP, tmp[1])) // Se tmp[1] não existir (fim da string) ou estiver em NOT_EXP (lista de caracteres não permitidos), expande o til para o valor da variável de ambiente HOME usando a função expand. Exemplo: echo - ou echo - > file1. Outro exemplo é echo ~john. Neste caso, tmp[1] é o caractere j. Porém, j não é um caso especial válido para expansão de tilde (apenas + e - são tratados). Como não é um caso válido, a função expande o tilde para o valor da variável de ambiente HOME.
		return (expand(env_get("HOME", sh), point, point + 1, line));
	else if (tmp[1] == '+' && (!tmp[2] || ft_strchr(NOT_EXP, tmp[2])))// Se tmp[1] for igual a '+' e tmp[2] não existir ou estiver na string NOT_EXP, a função retorna o resultado da função expand com "PWD" como argumento. Exemplo: echo ~+ é disso exemplo. tmp[1] é + e tmp[2] não existe quando a til (~) é seguida por um + e não há mais caracteres após o +. Por exemplo, na string "echo ~ +", tmp[0] seria ~, tmp[1] seria + e tmp[2] não existiria. 
		return (expand(env_get("PWD", sh), point, point + 2, line));
	else if (tmp[1] == '-' && (!tmp[2] || ft_strchr(NOT_EXP, tmp[2])))// Se tmp[1] for igual a '-' e tmp[2] não existir ou estiver na string NOT_EXP, a função retorna o resultado da função expand com "OLDPWD" como argumento. Linha de comando original: echo ~-. Neste caso, tmp[1] é o sinal de subtração (-) e tmp[2] não existe (fim da string). O sinal de - indica que o tilde deve ser expandido para o diretório de trabalho atual (PWD), e dps anda para o diretório anterior a esse. A função chama expand para substituir o tilde pelo caminho do diretório de trabalho atual.
		return (expand(env_get("OLDPWD", sh), point, point + 2,
				line));
	return (0); // Se nenhuma dessas condições for atendida, a função retorna 0.
}
// expand_tilde(t_shell *shell, char **line)
//Parâmetros:
//shell: Ponteiro para a estrutura t_shell contendo informações do shell.
//line: Ponteiro para um ponteiro de caractere (char **) que aponta para a linha de comando a ser expandida.
//O que faz:
//Percorre a linha de comando caractere por caractere.
//Procura por til (~) fora de aspas simples ou duplas.
//Se encontrar um til válido, chama a função point_to_exp_tilde para realizar a expansão.
//Atualiza o ponteiro line para apontar para a linha de comando expandida.
//Retorna 0 após percorrer toda a linha.
//Exemplo:
//Linha de comando original: ls ~
//Após expansão pela função expand_tilde: ls /home/usuario (assumindo /home/usuario é o valor da variável HOME)
static int	expand_tilde(t_shell *shell, char **line)
{
	char	quote;
	char	*tmp;

	quote = 0;
	tmp = *line;
	while (*tmp)
	{
		if (!quote && (*tmp == '"' || *tmp == '\''))
			quote = *tmp;
		else if (quote == *tmp)
			quote = 0;
		if (*tmp == '~' && !quote
			&& (tmp == *line || ft_strchr(SPACES, *(tmp - 1)))) // Se o caratere atual for um ~ e não estiver dentro de aspas, e se estivermos no início da string ou se o caractere anterior for um espaço, então expandimos.
			if (point_to_exp_tilde(shell, tmp - *line, tmp, line)) // 
				tmp = *line;
		if (*tmp)
			tmp++;
	}
	return (0);
}

//Parâmetros:
//sh: Ponteiro para a estrutura t_shell contendo informações do shell.
//point: Posição do caractere '$' na linha de comando original.
//tmp: Ponteiro para o caractere '$' na linha de comando expandida.
//line: Ponteiro para um ponteiro de caractere (char **) que aponta para a linha de comando a ser expandida.
//O que faz:
//Verifica se o caractere dólar ($) está em uma posição válida para expansão (seguido por um caractere válido para nome de variável de ambiente).
//Se for válido, analisa o caractere seguinte (tmp[1]).
//Se tmp[1] for ? (interrogação), expande para o código de saída do último comando executado usando g_exit e a função expand.
//Se tmp[1] existir e for um caractere válido para nome de variável de ambiente, extrai o nome da variável até o fim do nome válido (alfanumérico e underline).
//Utiliza a função env_get para recuperar o valor da variável de ambiente com o nome
static int	point_to_expand_env(t_shell *sh, int point, char *tmp, char **line)
{
	char	*key;
	int		len;

	if (tmp[1] == '?')
		return (expand_free(ft_itoa(g_exit), point, point + 2, line));
	else if (tmp[1]) // este if é para o caso de ser um $ seguido de um caractere que não é um espaço, por exemplo o caso específico echo "|$123USER|" "|$_123USER123|" '"|$_123USER123|"' "|$_123USER?123|" que tem de dar output |23USER| || "|$_123USER123|" |?123|, ou seja, não expande se for número (com exceção do primeiro) e expande se for _. O terceiro exemplo está dentro de aspas, logo apenas printa o que está dentro de aspas.
	{
		len = 1;
		while (ft_isalpha(tmp[len]) || tmp[len] == '_')
			len++;
		len += (ft_isalnum(tmp[len]) > 0);
		while (len > 2 && (ft_isalnum(tmp[len]) || tmp[len] == '_'))
			len++;
		key = ft_substr(tmp, 1, len - 1);
		expand(env_get(key, sh), point, point + len, line);
		return (free(key), 1);
	}
	return (0);
}

static void	env_expand(t_shell *shell, char *tmp, char **line)
{
	char	quote;

	quote = 0;
	while (*(++tmp))
	{
		if (!quote && (*tmp == '"' || *tmp == '\''))
			quote = *tmp;
		else if (quote == *tmp)
			quote = 0;
		if (*tmp == '$' && !ft_strchr(NOT_EXP, *(tmp + 1)) && quote != '\''
			&& !(quote && ft_strchr("\"'", *(tmp + 1)))) // Se o caractere atual for um $ e o próximo não for um espaço, e não estiver dentro de aspas simples (uma vez que as aspas duplas expandem), e se o próximo caractere não for uma aspa, então expandimos.
		{
			if (point_to_expand_env(shell, tmp - *line, tmp, line))
			{
				tmp = *line - 1;
				quote = 0;
			}
		}
	}
}

void	expand_arg(t_shell *shell, char **arg)
{
	expand_tilde(shell, arg);
	env_expand(shell, *arg - 1, arg);
}
