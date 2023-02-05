#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

/**
 * This method prints bytes, word count, and line 
 * count read from the standard input.
 * @param int* result pointer to an array to store data
 */
void getDataFromStdIn(int* result) 
{
	char buffer[1024];	// store data
    	int wordCount = 0;	// stores number of words
    	int lineCount = 0;	// stores number of lines
    	int totalBytes = 0;	// stores number of bytes

	// loops while true. Breaks at end of file.
	// this loops counts words, bytes, and lines.
    	while (1) 
	{
        	int bytes = read(STDIN_FILENO, buffer, sizeof(buffer));
        	
		if (bytes == 0)
		{
            		break;
		}

        	buffer[bytes] = '\0';
        	int i = 0;
        	totalBytes += bytes;
        	char previous = '\n';
		
		// this loops as long as buffer isn't null.
		// this loops counts words, bytes, and lines.
        	while (buffer[i] != '\0') 
		{
			// increase number of words after space, or new line.
            		if ((buffer[i] == ' ' || buffer[i] == '\n') && previous != '\n' && previous != ' ')
			{
                		wordCount++;
			}

			// increase number of lines after a new line
            		if (buffer[i] == '\n')
			{
                		lineCount++;
			}

            		previous = buffer[i];
            		i++;
        	}
    	}
 	
	// prints for standard input   
    	printf("\t%d\t%d\t%d\t -\n",lineCount, wordCount, totalBytes);
    	result[0] = wordCount;
    	result[1] = lineCount;
    	result[2] = totalBytes;
}

/**
 * This method returns the number of words.
 * @param int the file handle
 * @return int count of all words.
 */
int getWordsCount(int file) 
{
	char line[1024 * 1024];
    	memset(line, 0, sizeof(line));
    	char previous = ' ';
    	int count = 0;
    	int i = 0;
    	int val = read(file, line, sizeof(line)); 

	// loops as long as line is not equal to null.
    	while (line[i] != '\0') 
	{
		// increases the number of words if space ot new line.
        	if ((line[i] == ' ' || line[i] == '\n') && previous != '\n' && previous != ' ')
		{
            		count++;
		}

        	previous = line[i];
        	i++;
    	}

    	return count;
}

/**
 * This method returns the number of bytes.
 * @param int the file handle.
 * @return int cout of the total number of bytes read.
 */
int getBytesCount(int file) 
{
	char line[1024 * 1024];
    	memset(line, 0, sizeof(line));
    	int bytes = read(file, line, sizeof(line));
    	return bytes;
}

/**
 * This method returns the number of lines.
 * @param int the file handle.
 * @return an int count of the total number of lines.
 */
int getLinesCount(int file) 
{
	char line[1024 * 1024];
    	memset(line, 0, sizeof(line));
    	int count = 0;
    	int i = 0;
    	int val = read(file, line, sizeof(line));
	
	// loops until line is not equal to null.
    	while (line[i] != '\0') 
	{
		// increase the number of lines if new line.
        	if (line[i] == '\n')
		{
            		count++;
		}

        	i++;
    	}

    return count;
}

/**
 * The main method for the wc utility.
 * The wc utility reads one or more input files and 
 * write the number of newlines, words, and bytes.
 * -c: Write to the standard output the number of bytes in each input file.
 * -l: Write to the standard output the number of newlines in each input file.
 * -w: Write to the standard output the number of words in each input file.
 * @param argc the count of command line args passed
 * @param argv actual command line arguments passed
 */
int main(int argc, char* argv[])
{
	int opt;
    	int c_flag = 0, l_flag = 0, w_flag = 0;

	// loops as long as the parameters aren't false.
    	while ((opt = getopt(argc, argv, "lcw")) != -1)
    	{
		// determine if number of words, lines, or bytes are needed,
        	switch (opt)
        	{
            		case 'l':
                		l_flag = 1;
                		break;
            		case 'c':
                		c_flag = 1;
                		break;
            		case 'w':
                		w_flag = 1;
                		break;
        	}
    	}

    	int arg_count = optind;
    	int wordCount = 0;
    	int lineCount = 0;
    	int byteCount = 0;
    	int count = 0;
    	int data[3] = {0,0,0};		// data to store number of words, lines, and bytes.

	// getDataFromStdIn() if the count of command line arg is true.
    	if(argc == 1)
	{
        	getDataFromStdIn(data);
    	}

	// loops until the count is less than count command line arg 
    	for (int i=arg_count; i < argc; i++) 
	{
		// if - its standard input so getDataFromStdIn().
        	if(strcmp("-",argv[i]) == 0)
		{
            		getDataFromStdIn(data);
            		wordCount += data[0];
            		lineCount += data[1];
            		byteCount += data[2];
            		continue;
        	}

        	int file = open(argv[i], O_RDONLY);
		
		// error message if file does not exist.
        	if (file < 0) 
		{
            		printf("wc: %s: No such file or directory \n", argv[i]);
            		continue;
        	}

        	if(!l_flag && !c_flag && !w_flag)
		{
            		l_flag = 1;
            		w_flag = 1;
            		c_flag = 1;
        	}
		
		// if true from the switch. print the number of lines.
        	if(l_flag)
		{
            		count = getLinesCount(file);
            		printf("\t%d\t", count);
            		lineCount += count;
        	}

        	close(file);
        	file = open(argv[i], O_RDONLY);

		// if true from the switch. print the number of words.
        	if (w_flag) 
		{
            		count = getWordsCount(file);
            		printf("%d\t", count);
            		wordCount += count;
        	}

        	close(file);
        	file = open(argv[i], O_RDONLY);
		
		// if true from the switch. print the number of bytes.
        	if (c_flag) 
		{
            		count = getBytesCount(file);
            		printf("%d\t", count);
            		byteCount += count;
        	}

        	printf("%s\n", argv[i]);
        	close(file);
    	}

	// prints for multiple files and commands.
    	if(argc - optind > 1)
	{
        	if(lineCount > 0 )
		{
            		printf("\t%d", lineCount);
        	}

        	if(wordCount > 0 )
		{
            		printf("\t%d", wordCount);
        	}

        	if(byteCount > 0 )
		{
            		printf("\t%d\t", byteCount);
        	}

        	printf("total\n");
    	}

    	return 0;
}
