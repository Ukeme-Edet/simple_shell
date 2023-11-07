#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

/**
 * main - The main function for the simple shell project
 * Return: 0 if successful
 */
int main(void)
{
	int running = 0;
	while (!running)
	{
		running = shell();
	}
	return (0);
}

/**
 * shell - The shell function for the simple shell project
 * Return: 0 if successful
 */
int shell(void)
{
	char *line;
	char **args;
	int status;

	printf("> ");
	line = read_line();
	args = split_line(line);
	status = execute(args);
	free(line);
	free(args);
	return (status);
}

/**
 * read_line - The read_line function for the simple shell project
 * Return: 0 if successful
 */
char *read_line(void)
{
	char *line;
	size_t bufsize = 0;

	getline(&line, &bufsize, stdin);
	return (line);
}

/**
 * split_line - The split_line function for the simple shell project
 * Return: 0 if successful
 */
char **split_line(char *line)
{
	char **tokens;
	char *token;
	int i = 0;

	tokens = malloc(sizeof(char *) * 64);
	if (!tokens)
	{
		fprintf(stderr, "hsh: allocation error\n");
		exit(EXIT_FAILURE);
	}
	token = strtok(line, " \t\r\n\a");
	while (token)
	{
		tokens[i] = token;
		i++;
		token = strtok(NULL, " \t\r\n\a");
	}
	tokens[i] = NULL;
	return (tokens);
}

/**
 * execute - The execute function for the simple shell project
 * Return: 0 if successful
 */
int execute(char **args)
{
	pid_t pid;
	int status;

	pid = fork();
	if (pid == 0)
	{
		if (execvp(args[0], args) == -1)
		{
			perror("hsh");
		}
		exit(EXIT_FAILURE);
	}
	else if (pid < 0)
	{
		perror("hsh");
	}
	else
	{
		do
		{
			waitpid(pid, &status, WUNTRACED);
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}
	return (1);
}