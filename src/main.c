/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaosilva <joaosilva@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 17:07:27 by joaosilva         #+#    #+#             */
/*   Updated: 2024/06/13 01:20:35 by joaosilva        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int			g_exit;

// Função para mostrar o prompt do shell
static char	*show_prompt(void)
{
	char	*cwd;
	char	*prompt;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("Error getting current working directory");
		return (NULL);
	}
	prompt = ft_strjoin(PROMPT_BEGIN, cwd);
	free(cwd);
	cwd = ft_strjoin(prompt, PROMPT_END);
	free(prompt);
	if (!cwd)
	{
		perror("Error creating shell prompt");
		return (NULL);
	}
	return (cwd);
}

// Função para obter o prompt do shell
static char	*get_prompt(t_shell *shell)
{
	shell->prompt = show_prompt();
	if (!shell->prompt)
	{
		perror("Error creating shell prompt");
		return (NULL);
	}
	shell->line = readline(shell->prompt);
	free(shell->prompt);
	return (shell->line);
}

// Função para correr o loop principal
static int	to_run(t_shell *shell)
{
	signal_handler(SIGRESTORE); // O sinal é pre-definido a 1 (SIGRESTORE). Porquê? Pq foi a pré-definição que escolhemos. Porquê? Porque é um sinal que não é usado no sistema. Se usássemos um sinal que já estivesse a ser usado, poderíamos ter problemas com o código. Por isso, escolhemos um sinal que não estivesse a ser usado. O sinal é usado na função signal_handler no ficheiro signal_handler.c. O sinal é usado para restaurar o sinal anterior. Porquê? Porque se não restaurássemos o sinal anterior, poderíamos ter problemas com o código. Por isso, restauramos o sinal anterior. O sinal é restaurado na função signal_handler no ficheiro signal_handler.c.
	shell->status = STOP; // Aqui também fizemos o mesmo raciocínio do signal_handler. Pré-definimos o estado para 0 (STOP). Porque precisamos deste status? Para parar o loop do main. Outras abordagens foram colocar while(1) - abordagem do Walter. Nós tentamos fazer diferente. Vimos esta abordagem e decidimos implementá-la. Qual a diferença entre CONTINUE E RESTORE? O status RESTORE é usado para restaurar o prompt após um sinal SIGINT. O status CONTINUE é usado para continuar a execução do shell. Em termos práticos o que acontece? Se o utilizador carregar ctrl+c, o shell continua a correr. Se o utilizador carregar ctrl+d, o shell fecha. Porquê? Porque o ctrl+d é um sinal de EOF. O que é EOF? EOF é o fim do ficheiro. Se o utilizador carregar ctrl+d, o shell fecha. Se o utilizador carregar ctrl+c, o shell continua a correr. Porquê? Porque o ctrl+c é um sinal de interrupção. Se o utilizador carregar ctrl+c, o shell continua a correr. 
	shell->line = get_prompt(shell); // Esta função obtém a prompt. O que é a prompt? É o sinal de $ a piscar à espera de input (à espera de um comando na linha de comando.)
	if (shell->line && process_line(shell)) // Aqui inicia-se verdadeiramente o Minishell. Primeiro tem de haver uma linha de comando para correr. Em segundo, essa linha tem de passar pelo primeiro processamento (na função process_line, que retira os espaços no início e final da linha de comando - caso existam, verifica se as quotes são unmatched ou não, e verifica se temos pipe no início ou final da linha de comando - o que não pode acontecer, uma vez que o pipe necessita sempre de informação antes e deps do pipe. Logo se estiver no início não tem info antes. Se tiver no fim, não tem info dps). A função process_line ainda faz outra coisa: inclui um nulo (\0) entre cada argumento que encontrar (exemplo: no comando ls -l, mete um nulo entre o ls e -l, ficando assim ls\0-l). Para que isto serve? Para que possamos percorrer o peek usando o nulo e não o espaço. Pa mais info ver a função peek. Esta função process_line basicamente processa/trata a linha de comando para que possamos trabalhar com ela nas funções mais à frente/nas funções que se seguem, nomeadamente na parte do Parser.
	{
		if (parse_cmd(shell)) // Se tudo tiver ok, inicia-se o Parse. A função parse_cmd define uma variável para o início da linha (ps) e uma para o final da linha (es). A seguir passa para o parsepipe, e é aí que começa a criação da árvore recursiva. Vamos assumir que estamos dentro da pasta do minishell, e que usamos como exemplo o comando: ""ls -l""" << eof | grep Makefile > output.txt. A função parsepipe começa por percorrer a linha de comando fazendo um peek (	if (peek(shell, "|") && shell->status == CONTINUE)), o qual só é feito para verificar se existem 2 pipes (||), o que não estamos a testar uma vez q não fizemos bonús (no bónus é que é esperado que tenhamos feito operadores lógicos: &&, ||, e wildcard...além das aspas que se as tratarmos/considerarmos de forma tb estarão bem no bónus). Após esse primeiro peek (que só entra da segunda vez. da primeira não entra pq não temos pipe, mas sim aspas duplas - que vêm do "ls -l"), a função entra no parse_exec, começando a construir o cmd. No parse_exec, a função constroí o comando executável (cmd = (t_exec *)ret;) e é neste ponto que começa a contrução do primeiro node da árvore. Neste ponto a função entra no primeiro ret = parseredir(ret, shell);, mas verifica que não existe nenhum token de redirect (< > << >>) uma vez que o carater é uma aspa, saindo da parse_redir sem que tenha alterado alguma coisa. Entra sim no while, pq o peek verifica que não é um pipe. Aqui verifica o tipo de token (no caso a, q é o token respetivo ao que não é redirect...isto na função get_token_type, onde o valor de token é definido. A função get_token está separada da get_token_type devido ao limite de 25 linhas) e se for diferente de a é pq é um redirect ou um pipe (o que não pode acontecer, uma vez que o redirect deveria ter sido "apanhado" na função get_token_type e na função parse_redir...na comparação que é lá feita, e o pipe nem deveria ter permitido entrar no while). Após isso entra no if (cmd->argv[0]) e começa a separar o argumento (apenas se estiver fora de aspas. No exemplo ls -l está dentro de aspas...isto só será tratado no run_exec que chama a função trim_quotes, que retira as quotes e manda o comando para execução)...mas assumindo que o comando ls -l está fora de aspas, é neste if -> if (cmd->argv[0]) <- que é separado o ls do -l e colocado um nulo entre os dois (libertando seguidamente a ls -l, uma vez que já temos uma cópia desse argumento com o nulo no meio ls\0-l, o que foi conseguido através da função ft_strjoin_free_s1). Seguimos para o próximo carater até chegarmos ao primeiro redirect, que é o <. Aqui é chamada a função parseredir ret = parseredir(ret, shell); dentro do loop que cria o node redir_cmd dentro do if (type == '<'). Como a função redir_cmd leva como primeiro argumento o cmd, o comando exec criado na parseexec, é incluído no node da redir_cmd, fazendo agora parte dele (particularidade: caso o comando fosse < file1 ls -l, entraríamos no primeiro et = parseredir(ret, shell); e aqui era reconhecido o < e criado o redir_cmd, que teria dentro um comando exec vazio que havia sido criado anteriormente na ret = exec_cmd(); e na cmd = (t_exec *)ret; da função parseexec. Esse comando apenas seria criado dentro do while loop qd a função atingisse o argumento ls -l). De seguida voltamos ao parsepipe, agora já com o ret (o node do redir_cmd com o exec_cmd lá dentro), que verifica que temos um pipe e entra no if if (cmd && peek(shell, "|")), que volta a chamar o parsepipe cmd = pipe_cmd(cmd, parsepipe(shell));. A info à esquerda do pipe agora é o cmd retornado do parseexec(o ret), e a info da direita volta a refazer todo o processo mas para o lado direito. No final o parsepipe devolve o cmd (que é a raíz da árvore). Porquê a raíz? Pq estes nodes comportam-se como uma linked list, e o left e right do pipe será como um previous/next ou next/next se preferirmos, linkando toda a linked list. Assim ao retornar a raíz da árvore, conseguimos aceder a toda a árvore, percorrendo-a como se fosse uma linked list, o que acontece no run_exec, quando a árvore começa a ser executada. No caso de não haver pipe, a raíz da árvore passa a ser o comando da redir (caso exista um redirecionamento), o qual contém o exec (ver mais info na parseexec). Se não existir nem pipe nem redir (por exemplo se só tivermos o comando: ls -l), o nó criado é o do exec, que é devolvido como ret no final da parseexec. Se não houver redir, esta função apesar de entrar dentro da parseredir duas vezes (imediatamente antes do while na ret = parseredir(ret, shell); e dentro do while na última linha do mesmo), ela devolve o node do comando exec que foi criado na ret = exec_cmd();, passado/feito o cast para comando tipo exec na cmd = (t_exec *)ret; e preenchido dentro do loop do while quando separamos os argumentos do comando do comando na parte do if (cmd->argv[0]). 
		{
			signal_handler(SIGPIPE); // O sinal com o qual entramos na função signal_handler é o SIGPIPE. Porquê? Porque é o sinal que é enviado quando um pipe é fechado. Porquê é que precisamos deste sinal? Porque se não tivermos este sinal, poderemos ter problemas com o código. Por isso, precisamos deste sinal. O sinal é restaurado na função signal_handler no ficheiro signal_handler.c.
			run_cmd(shell, shell->cmd); // Após a arvóre montada, começamos a executá-la. Aqui entra o cmd que veio do Parser, começando a ser executado o comando pela sua raíz. Se o comando tiver pipe a sua raíz é um pipe_cmd. Se não tiver pipe, a sua raíz é um redir_cmd se existir um redirecionamento (pq a função parseexec chama a função parseredir que verifica que tem um redir lá dentro e cria um redir_cmd). Caso não exista nem um pipe nem um redir, a função parseexec cria o cmd_exec cmd = (t_exec *)ret;, e dps chama a função parseredir (que nada faz pq não deteta nenhum token) que devolve o comando intato. Assim, a função parseredir retorna o cmd_exec igual pq não reconhece nenhum redirecionamento. Ou seja, o nó criado e devolvido é só um node executável (exec_cmd que é criado na função t_cmd	*exec_cmd(void) no ficheiro node_construtors.c).
		}
		free_cmd(shell->cmd);
	}
	free(shell->line);
	return (shell->status);
}

// Função para inicializar as variáveis do shell
static int	init_shell_variables(t_shell *shell, char **envp)
{
	*shell = (t_shell){0}; // Desta forma conseguimos inicializar todas as variáveis dentro da nossa struct. 
	convert_envp_to_linked_lists(envp, shell); // Esta função converte as env nas linked lists que falamos no main.
	convert_envp_to_char(shell); // Esta função cria a array de strings que falamos no main.
	return (1);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argv;
	if (!init_shell_variables(&shell, envp)) // Antes de avançarmos no código, temos de verificar se conseguimos iniciar as variáveis do shell. Se não conseguirmos, retornamos 0. Porquê? Porque se não conseguirmos iniciar as variáveis do shell, não conseguimos correr o shell. Logo, não faz sentido continuar o código. Por isso, retornamos 0.
		return (0);
	if (argc != 1)
	{
		printf("Minishell need no arguments. Try './minishell'.\n");
		return (0);
	}
	else
		printf("\nMinishell 1.0: Authors: jode-jes, crocha-s.\n");
	while (to_run(&shell))
		;
	clear_history();
	ft_envlstclear(shell.env_list_unsorted, free); // Depois de usarmos a lista sempre que precisamos e no código todo, damos free qd deixamos de precisar dela. Precisamos da lista para as variáveis de ambiente, assim como para chamar qqer comando que tenha a ver com as variáveis de ambiente, e cujo path vai verificaar a estas (na função get_path no ficheiro run_exec, por exemplo)
	ft_envlstclear(shell.env_list_sorted, free); // Temos duas listas. uma unsorted, outra sorted. Porquê? Pq o comando env apresenta a lista por ordem de criação e o comando export por ordem alfabética. Logo optamos pela abordagem de ter duas listas. Essas listas são criadas na pasta envp1 e são usadas em todo o código. A lista sorted é usada para o comando export, e a unsorted para o comando env. Ambas as listas são convertidas (ou seja, vamos buscar as env ao sistema) na função convert_envp_to_linked_lists no ficheiro envp1_convert.c, mas são efetivamente criadas na função create_update_envp_lists no ficheiro envp2_create.c. As listas são atualizadas na função create_update_envp_lists que chama as funções add_node_to_envp_unsorted_list e add_node_to_envp_sorted_list no ficheiro envp2_create.c.  A unsorted list é criada criando os nodes (com a list_new) e adicionando-os ao final da lista (com a list_back). Quer a list_new, quer a list_back, são funções do libft. A sorted_list é criada através usando a list_new, mas depois esse node entra logo para o local/posição correta na lista (tal é feito tb usando uma função que já usamos...no caso no exame rank 2, a sort_list. Contudo esta função tem uma particularidade: usamos um ponteiro de ponteiro para a ordenar e não o valor da key ou da value...uma vez que teríamos que ordenar os dois. Mais info na própria função). 
	if (shell.envp_char) // Criamos também um array de strings. Porquê? Pq o execve precisa que lhe seja passado um array de strings. Essa array é criada na função init_shell_variables, e é atualizada na função add_node_to_envp_unsorted_list no ficheiro envp2_create.c., (quando é preciso adicionar uma variável) e na função env_rm_unsorted (qd é preciso remover). Este array de strings é usado em todo o código, mas principalmente na função run_exec no ficheiro run_exec.c, onde é passado como argumento ao execve.
		ft_free_array(shell.envp_char); // Após terminarmos o uso deste array (ponteiro de pointeiros), damos free ao mesmo para evitarmos leaks na memória.
	if (isatty(STDIN_FILENO)) // Se o stdin for um terminal, damos free ao shell.prompt. Porquê? Pq o prompt é criado na função show_prompt no ficheiro main.c, e é usado em todo o código. Assim, damos free ao mesmo para evitarmos leaks na memória. Em que casos isto acontece? Quando o stdin é um terminal, ou seja, quando o programa é corrido diretamente no terminal. Se o programa for corrido com um ficheiro como argumento, o stdin não é um terminal, e o prompt não é criado.
		ft_putendl_fd("exit", 2);
	return (g_exit); // Temos de retornar sempre a variável de saída. O que ela faz? Como próprio nome indica, ela retorna o código de saída. Porque só usamos uma? Para não haver conflitos a nível do código de saída. Se tivessemos várias variáveis de saída, possivelmente teríamos conflitos com o código de saída, podendo entrar em conflito se existissem códigos diferentes gerados para a mesma variável em funções diferentes. Porque só usamos uma? Porque só temos de controlar o fluxo do código de saída. Logo, só precusamos de uma variável de saída. Se por exemplo fosse preciso saber qual o status de outra coisa na Minishell, deveríamos criar outra variável para isso.
}
