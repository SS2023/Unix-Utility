#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

/**
 * reads input from standard input and prints
 * @param int a flag to conditionally print a statement
 */
void getDataFromStdIn(int flag)
{
	char buffer[1024];
	
	// shows reading from standard input
    	if(flag == 0)
	{
        	printf("==> %s <== \n", "standard input");
	}

	// loops until the end of file.
    	while(1)
	{
        	int bytes = read(STDIN_FILENO, buffer, sizeof(buffer));

		// break at end of file
        	if(bytes == 0)
		{
            		break;
		}

        	buffer[bytes] = '\0';
        	printf("%s",buffer);
    	}

    	printf("\n");
}

/**
 * Prints the lines for each file passed as argv
 * @param argc the count of command line args passed
 * @param argv actual command line arguments passed
 * @param count the number of line to print in the file
 */
void getDataLineByLine(int count, int argc, char** argv)
{
	char line[1024*1024];

	// loops until < count of command line args
    	for(; optind < argc; optind++)
	{
		// standard input if -. getDataFromStdIn().
		if(strcmp("-", argv[optind]) == 0)
		{
            		getDataFromStdIn(0);
            		continue;
        	}

        	int file = open(argv[optind], O_RDONLY);

		// erroe message if file not found.
        	if(file < 0)
		{
            		printf("head: cannot open '%s' for reading: No such file or directory \n", argv[optind]);
            		continue;
        	}

        	printf("==> %s <== \n", argv[optind]);
        	int j =1;
        	int i=0;
        	read(file, line, sizeof(line));

		// loops as long as line is not equal to null
        	while (line[i] != '\0') 
		{
			// increase j if new line.
			// Used to count number of lines.
            		if(line[i] == '\n')
			{
                		j++;
            		}
			
			// stop at a certain line
            		if(j > count)
			{
                		break;
			}

            		printf("%c", line[i]);
            		i++;
        	}

        	if(optind < argc -1)
		{
            		printf("\n\n");
		}

        	memset(line, 0, sizeof(line));
        	close(file);
    	}
}

/**
 * Prints the bytes for each file passed as argv
 * @param argc the count of command line args passed
 * @param argv actual command line arguments passed
 * @param count number of bytes to show
 */
void getDataByBytes(int count, int argc, char** argv)
{
	char line[1024*1024];
	
	// loops until < count of command line args
    	for(; optind < argc; optind++)
	{
        	int file = open(argv[optind], O_RDONLY);

		// error message id file not found.
        	if(file < 0)
		{
            		printf("head: cannot open '%s' for reading: No such file or directory \n" , argv[optind]);
            		continue;
        	}

        	printf("==> %s <== \n", argv[optind]);
        	int i = 0;
        	read(file, line, count);

		// loops until line does not equal null
        	while(line[i] != '\0')
		{
            		if(i > count)
			{
                		break;
			}

            		printf("%c", line[i]);
            		i++;
        	}

        	memset(line, 0, sizeof(line));
        	close(file);
    	}
}

/**
 * The main method for the head utility.
 * The head utility copies their input files to the standard output, ending or starting the
 * output for each file at a designated point. Copying end/begin at the point in 
 * each input file indicated by the -c number or -n number option. Both line and 
 * byte counts start from 1. If no files operand is specified, or when one of the 
 * files is -, then standard input is assumed.
 *
 * @param argc the count of command line args passed
 * @param argv actual command line arguments passed
 */
int main(int argc, char *argv[])
{
	int opt;
	
	// loops to see if bytes or line
    	while((opt = getopt(argc, argv, "n:c:")) != -1)
    	{
        	// switch for bytes ot lines
		switch(opt)
        	{
			// line
            		case 'n':
                		if(atoi(optarg) == 0)
				{
                    			printf("head: %s: invalid number of lines\n", argv[2]);
                    			return 0;
                		}

                		getDataLineByLine(atoi(optarg), argc, argv);
                		break;
			
			// bytes
            		case 'c':
                		if(atoi(optarg) == 0)
				{
                    			printf("head: %s: invalid number of bytes\n", argv[2]);
                    			return 0;
                		}

                		getDataByBytes(atoi(optarg), argc, argv);
                		break;
        	}
   	}	

    	// checks if ./head or ./head - is called
    	if((argc == 2 && strcmp(argv[1], "-") == 0) || argc == 1)
	{
        	getDataFromStdIn(1);
    	}

	else
	{
        	getDataLineByLine(10, argc, argv);
    	}

    	return 0;
}

