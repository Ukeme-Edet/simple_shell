#ifndef MAIN_H
#define MAIN_H /* MAIN_H */

extern char **environ;

int _putchar(char c);
char *read_line(void);
char **split_line(char *line);
int execute(char **args);
int command_exists(char *command);

#endif /*MAIN_H */
