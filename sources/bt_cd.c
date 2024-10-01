#include "minishell.h"

int bt_cd(t_word *args)
{
    char *path;
    char cwd[1024];

    if (!args->next) // No argument after "cd"
    {
        path = getenv("HOME");
        if (!path)
        {
            perror("cd: HOME not set");
            return (1);
        }
    }
    else
    {
        path = args->next->value; // Get the path from the next argument
    }

    if (chdir(path) != 0) // Change directory
    {
        perror("cd");
        return (1);
    }

    if (getcwd(cwd, sizeof(cwd)) != NULL) // Print new directory
    {
        printf("Directory changed to: %s\n", cwd);
    }
    else
    {
        perror("getcwd");
        return (1);
    }
    return (0);
}
