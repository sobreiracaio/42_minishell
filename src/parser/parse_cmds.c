/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmds.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaosilva <joaosilva@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 19:10:56 by joaosilva         #+#    #+#             */
/*   Updated: 2024/06/13 01:00:53 by joaosilva        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Token é o argumento. O argumento é o que vem depois do operador de 
//redirecionamento "<" ou ">".
// O while verifica se o próximo token é "<" ou ">".
// O type atribui um toke type que pode ser "<", ">", 
// "<<, ">>", "|", ou "a".
// O segundo if verifica se o token não for argumento ou se o 
//token for "<" e o próximo token for ">".
static t_cmd	*parseredir(t_cmd *cmd, t_shell *shell)
{
	int		type;
	char	*token;

	while (peek(shell, "<>"))
	{
		type = gettoken(shell, NULL); // Aqui chamamos a função gettoken para obter o tipo do token que estamos a analisar. Passamos-lhe null como segundo argumento pq apenas queremos correr a função para obter o tipo do token e não queremos guardar o token em lado nenhum.
		if (gettoken(shell, &token) != 'a')// A get token pode dar-nos duas coisas: o tipo de token e o próprio token (o resto do comando basicamente), apesar de só retornar o tipo.
		{
			if (type != '<' || (type == '<' && ft_strcmp(token, ">"))) // Aqui verificamos se o token é diferente de "<" ou se o token é "<" e o próximo token é diferente de ">", o que significa que o token é "<" e o próximo token é >, o que é um erro de sintaxe.
				return (print_error_syntax(shell, token, 2), cmd);
			else if (gettoken(shell, &token) != 'a') // Aqui verificamos se o token é diferente de "a", ou seja, se o token não é um argumento. O token a seguir a < tem de ser argumento (tem de ser um nome de ficheiro), logo isto dá erro.
				return (print_error_syntax(shell, token, 2), cmd);
		}
		if (type == '<')
			cmd = redir_cmd(cmd, token, O_RDONLY, 0);
		else if (type == '>')
			cmd = redir_cmd(cmd, token, O_WRONLY | O_CREAT | O_TRUNC, 1);
		else if (type == APPEND)
			cmd = redir_cmd(cmd, token, O_WRONLY | O_CREAT | O_APPEND, 1);
		else if (type == HERE_DOC)
			cmd = here_cmd(cmd, token);
	}
	return (cmd);
}
// Esta função é chamada pela função parsepipe e serve para criar o node_exec e o node_redir (através da parseredir que é chamada dentro da parse_exec).
static t_cmd	*parseexec(t_shell *shell)
{
	t_cmd	*ret;
	t_exec	*cmd;
	char	*token;
	int		type;

	ret = exec_cmd(); // Aqui criamos um comando global que contem um node executável (criado através da função de construção de nodes no ficheiro node_construtors.c. Neste caso a exec_cmd). O cmd na linha abaixo será igual ao cast para exec deste ret, logo o cmd é igual ao (t_exec*)exec_cmd e esse exec_cmd é passado para a função parse_redir, que dps o transforma num redir_cmd caso exista um redir na linha de comando (e o qual já contém a parte executável (o exec_cmd)). Mas como? Fácil. Continua explicação nas duas linhas abaixo.
	cmd = (t_exec *)ret;// Aqui é onde fazemos o cast de comando global para exec_cmd. Este comando irá passar como argumento para a função parseredir. Se não houver redirecionamento, o cmd é devolvido intacto. Se houver o comando é devolvido como redir_cmd, do qual o exec_cmd fará parte, uma vez que é passado como argumento na criação do cmd_redir (exemplo: cmd = here_cmd(cmd, token); ). Explicação continua na linha seguinte.
	ret = parseredir(ret, shell); // Vejamos melhor: Se não houver redirecionamento, a função parseredir não deteta nenhum redir, logo não faz nada. Logo o comando permanece igual. O ret continua a não conter nenhuma redireção, logo apesar de entrarmos na parseredir, o ret é igual ao cmd. Por exemplo se tivermos ls -l, o ret é igual ao cmd, logo o cmd é igual ao ret, logo o cmd é igual ao ret. Ou se tivermos cat < file1. Como o primeiro argumento é um executável, apesar de entrarmos na parseredir, a função não reconhece nenhum token, logo o ret é igual ao cmd, logo o cmd é igual ao ret. Nesse caso a função devolve o cmd_exec tal como referido na linha de cima. Só no caso de ser < file1 cat é que entra a primeira vez aqui e deteta logo o token <, logo o ret passa a ser um redir_cmd e o cmd (que é o exec_cmd que provém da cmd = (t_exec *)ret; ) passa a ser um redir_cmd. E tá feito!
	while (!peek(shell, "|"))
	{
		type = gettoken(shell, &token);// Aqui passamos o token pq queremos que a gettoken nos devolva o token que estamos a analisar. O token é o argumento que vem depois do token, ou seja suponhamos ls -l < file1 | wc -l, o token é o < e o que vem dps do token é o file1, o seu argumento. Nós queremos que a função gettoken nos devolva o tipo de token encontrado, mas tb o próprio token que vem a seguir que é o file1 | wc -l.
		if (!type)
			break ;
		if (type != 'a' && shell->status == CONTINUE)
			return (print_error_syntax(shell, token, 2), ret);
		if (cmd->argv[0]) // Aqui verificamos se o argv[0] do comando já tem algo, ou seja, se já temos um comando. Se já tivermos um comando, então quer dizer que o comando já tem um executável, logo o token que encontramos é um argumento e não um executável. Por exemplo, se tivermos ls -l, o ls é o executável e o -l é o argumento. Se tivermos ls -l < file1, o ls é o executável, o -l é o argumento e o < é um token, logo o token é o argumento que vem a seguir ao token, que é o <.
			cmd->argv[0] = ft_strjoin_free_s1(cmd->argv[0], " "); // Se a condição de cima se verificar, Aqui é onde o espaço é adicionado ao argv[0] do comando e ao ls, respetivamente, ficando o -l separado do ls.
		cmd->argv[0] = ft_strjoin_free_s1(cmd->argv[0], token); // Aqui é onde juntamos novamente o resto do comando/o seu argumento (-l) ao ls -l, ficando novamente com ls -l, mas desta vez com espaço no meio dos dois e não com um nulo (que colocamos na função insert_null_char no ficheiro process_line). Na função run_exec voltamos a remover o espaço e a substituir por um nulo.
		ret = parseredir(ret, shell);
	}
	return (ret);
}
// A função parsepipe é chamada pela função parse_cmd e serve para criar um novo comando com o comando atual e o próximo comando, que é o que vem depois do pipe/do seu lado direito. Do lado esquerdo já fica o cmd que advém da parseexec.
t_cmd	*parsepipe(t_shell *shell)
{
	t_cmd	*cmd;

	if (peek(shell, "|") && shell->status == CONTINUE) // Da segunda vez que passa verificamos se tem outra vez |. Se tiver é pq é um ||, e como no nosso caso (mandatório) não verificamos isso, tem obrigatoriamente de dar erro.
		return (print_error_syntax(shell, shell->ps, 2), NULL);
	cmd = parseexec(shell);
	if (cmd && peek(shell, "|"))
	{
		gettoken(shell, NULL); // Aqui chamamos a gettoken com NULL para que a possamos correr até encontrar o próximo argumento, uma vez que os argumentos estão separados por null (o que foi feito na função insert_null_char no ficheiro process_line).
		cmd = pipe_cmd(cmd, parsepipe(shell)); // Aqui chamamos a função pipe_cmd que vai criar um novo comando com o comando atual e o próximo comando, que é o que vem depois do pipe/do seu lado direito. Do lado esquerdo já fica o cmd que advém da parseexec.
	}
	return (cmd);
}
