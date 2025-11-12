#include "shell.h"

/**
 * _strlen - compute string length
 * @s: input string
 *
 * Return: number of characters before '\0', or 0 if s is NULL
 */
int _strlen(const char *s)
{
	int i = 0;

	if (!s)
		return (0);
	while (s[i])
		i++;
	return (i);
}
