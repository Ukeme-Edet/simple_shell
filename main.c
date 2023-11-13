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
	char *line;
	char **args;
	int status;

	status = 1;
	while (status)
	{
		printf("> ");
		line = read_line();
		args = split_line(line);
		status = execute(args);
		free(line);
		free(args);
	}
	return (status);
	return (0);
}

/**
 * read_line - The read_line function for the simple shell project
 * Return: 0 if successful
 */
char *read_line(void)
{
	char *line = NULL;
	size_t bufsize = 0;

	if (getline(&line, &bufsize, stdin) == -1)
	{
		free(line);
		if (feof(stdin))
		{
			exit(EXIT_SUCCESS);
		}
		else
		{
			perror("hsh");
			exit(EXIT_FAILURE);
		}
	}
	return (line);
}

/**
 * split_line - The split_line function for the simple shell project
 * @line: The line passed to the function
 * Return: 0 if successful
 */
char **split_line(char *line)
{
	int bufsize = 64, position = 0;
	char **tokens = malloc(bufsize * sizeof(char *));
	char *token;

	if (!tokens)
	{
		fprintf(stderr, "hsh: allocation error\n");
		exit(EXIT_FAILURE);
	}
	token = strtok(line, " \t\r\n\a");
	while (token != NULL)
	{
		tokens[position] = token;
		position++;
		if (position >= bufsize)
		{
			bufsize += 64;
			tokens = realloc(tokens, bufsize * sizeof(char *));
			if (!tokens)
			{
				fprintf(stderr, "hsh: allocation error\n");
				exit(EXIT_FAILURE);
			}
		}
		token = strtok(NULL, " \t\r\n\a");
	}
	tokens[position] = NULL;
	return (tokens);
}

/**
 * execute - The execute function for the simple shell project
 * @args: The arguments passed to the function
 * Return: 1 if successful
 */
int execute(char **args)
{
	pid_t pid, wpid;
	int status;

	if (args[0] && (command_exists(args[0]) || access(args[0], F_OK) == 0))
	{
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
			do {
				wpid = waitpid(pid, &status, WUNTRACED);
				if (wpid == -1)
				{
					perror("hsh");
				}
			} while (!WIFEXITED(status) && !WIFSIGNALED(status));
		}
	}
	else if (args[0])
		perror("hsh");
	return (1);
}

/**
 * command_exists - The command_exists function for the simple shell project
 * @command: The command passed to the function
 * Return: 1 if successful
 */
int command_exists(char *command)
{
	char *path = getenv("PATH");
	char *path_copy = strdup(path);
	char *token = strtok(path_copy, ":");
	char *command_path;

	while (token != NULL)
	{
		command_path = malloc(strlen(token) + strlen(command) + 2);
		sprintf(command_path, "%s/%s", token, command);
		if (access(command_path, F_OK || X_OK) == 0)
		{
			free(command_path);
			free(path_copy);
			return (1);
		}
		free(command_path);
		token = strtok(NULL, ":");
	}
	free(path_copy);
	return (0);
}
