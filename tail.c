#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

/**
 * reads input from standard input and prints
 * @param int a flag to conditionally print statement
 */
void getDataFromStdIn(int flag)
{
	// if standard input print reading from standard input.
	if(flag == 0)
	{
        	printf("==> %s <== \n", "standard input");
	}

    	char buffer[1024*1024];		// store data
    	int totalRead = 0;

	// loops as long as it is true. Breaks at end of file.
    	while(1)
	{
        	char temp[1024];
        	int bytes = read(STDIN_FILENO, temp, sizeof(temp));

		// Break at end of file.
        	if(bytes == 0)
		{
            		break;
		}

        	int i = 0;

        	while (i < bytes)
		{
            		buffer[totalRead] = temp[i];
            		i++;
            		totalRead++;
        	}
    	}

    	totalRead++;
    	buffer[totalRead] = '\0';
    	printf("%s\n",buffer);
}


/**
 * Prints the lines for each file passed as argv
 * @param argc the count of command line args passed
 * @param argv actual command line arguments passed
 * @param count number of lines to show
 */
void getDataLineByLine(int count, int argc, char** argv)
{
	char line [1024*1024];

	// Loops as long as count of command args is >
    	for(; optind < argc; optind++)
	{
        	memset(line,0 ,sizeof(line));
        	int counter = count;
	
		// if - its standard input. getDataFromStdIn().
        	if(strcmp("-", argv[optind]) == 0)
		{
            		getDataFromStdIn(0);
            		continue;
        	}

        	int file = open(argv[optind], O_RDONLY);
		
		// if file does not exist.
        	if(file < 0)
		{
            		printf("tail: cannot open '%s' for reading: No such file or directory \n", argv[optind]);
            		continue;
        	}

        	printf("==> %s <== \n", argv[optind]);

        	int val = read(file, line, sizeof(line)) - 1;
        	int previous = val;

		// loops as long as line is not equal to null.
        	while (line[val] != '\0' && counter > -1) 
		{
			// if new line
            		if(line[val] == '\n')
			{
                		// to print specific number of lines.
				for(int j = val; j <= previous; j++)
				{
                    			previous = val - 1;
				}

                		counter--;
            		}

            		val--;
        	}

        	val++;

		// loops as long as line is not equal to null.
        	while(line[val] != '\0')
		{
            		printf("%c", line[val]);
            		val++;
        	}

        	if(optind < argc -1)
		{
            		printf("\n");
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
    	char line [1024*1024];

	 // Loops as long as count of command args is >
    	for(; optind < argc; optind++)
	{
        	memset(line,0 ,sizeof(line));
        	int counter = count;
        	int file = open(argv[optind], O_RDONLY);

		// error message if file does not exist.
        	if(file < 0)
		{
            		printf("tail: cannot open '%s' for reading: No such file or directory \n", argv[optind]);
            		continue;
        	}

        	printf("==> %s <== \n", argv[optind]);
        	int val = read(file, line, sizeof(line));
        	val = val - counter;

        	if(val < 0)
		{
            		val = 0;
		}

		// loops as long as line is not equal to null.
        	while (line[val] != '\0') 
		{
            		printf("%c",line[val]);
            		val++;
        	}

        	if(optind < argc -1)
		{
            		printf("\n\n");
		}

        	close(file);
    	}
}

/**
 * The tail utility copies their input files to the standard output, 
 * ending or starting the output for each file at a designated point. 
 * Copying end/begin at the point in each input file indicated
 * by the -c number or -n number. The option-argument number is counted 
 * in units of lines or bytes, according to the options -n and -c. 
 * Both line and byte counts start from 1.
 * @param argc the count of command line args passed
 * @param argv actual command line arguments passed
 */
int main(int argc, char *argv[])
{
	int opt;

	// while not false. for -n -c
    	while((opt = getopt(argc, argv, "n:c:")) != -1)
    	{
		// switch for bytes and lines
        	switch(opt)
        	{
			// case for number of lines. error message if incorrect number specified.
            		case 'n':
                		if(atoi(optarg) == 0)
				{
                    			printf("tail: %s: invalid number of lines\n", argv[2]);
                    			return 0;
                		}

                		getDataLineByLine(atoi(optarg), argc, argv);
                		break;

			// case for number of bytes. error message if incorrect number specified.
            		case 'c':
                		if(atoi(optarg) == 0)
				{
                    			printf("tail: %s: invalid number of bytes\n", argv[2]);
                    			return 0;
                		}

                		getDataByBytes(atoi(optarg), argc, argv);
                		break;
        	}
    	}

    	// checks if ./tail or ./tail - is called
    	if((argc == 2 && strcmp(argv[1], "-") == 0) || argc == 1)
	{
        	getDataFromStdIn(1);
    	}

	else
	{
        	getDataLineByLine(10,argc,argv);
    	}

    	return 0;
}

