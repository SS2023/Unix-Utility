#include <stdio.h>

/**
 * The env utility prints all of the currently set environmental 
 * variables to standard output.
 * @param argc is the command line args count
 * @param argv holds argument passed on command line
 * @param envp is given by the operating systems
 */
int main(int argc, char **argv, char **envp)
{
	// looping over each entry of envp
	for (char **env = envp; *env != 0; env++)
	{
        	char *thisEnv = *env;
        	printf("%s\n", thisEnv);
    	}

    	return 0;
}
