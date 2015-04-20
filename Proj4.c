#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//Still having issues with the dynamically allocated string
//Force compilation: gcc -x c proj4.c -o proj4.exe

#define max_length 20 //Max length of a word

struct Word_setup {
	char word[max_length];
	int count;
};

#define SIZE (sizeof (struct Word_setup))

int repeat_word(int marker, struct Word_setup *phrase);
void add_count(int marker, struct Word_setup *phrase);
void highest(struct Word_setup *phrase);
int check_file(FILE *fp, int marker, struct Word_setup *phrase);

int repeat_marker, final_marker, total_num_words;

int main(void)
{
	int i, marker;
	char ch;
	FILE *fp;
//****************THIS IS WHERE THE ERROR OCCURS*****************************************************
	printf("Pre-Allocation Test"); //When I run it on cmd, itll print this line, and then freeze, so I'm not sure how malloc is failing, because it doenst even output NULL
	struct Word_setup *phrase;
	phrase = (struct Word_setup *) malloc(SIZE); //For some reason the compiler thinks that this syntax is incorrect. Try it on a different computer possibly.
	if (phrase == NULL)
		{printf("Failure allocating memory"); return 0;}
//****************THIS IS THE ERROR END*************************************************************
	fp = fopen("C:\\Users\\Nicolas\\Desktop\\UF\\Semester 2\\COP 3275\\Projects\\Project 4\\input.txt", "r"); //Edit to the location of the file

	marker = 0;
	check_file(fp, marker, phrase); //check file to see if its reached the end, also works to create a word!
	add_count(marker, phrase);

	for (i = 1;;i++)
	{
		marker = i;
		total_num_words = i + 1;
		
		phrase = (struct Word_setup *) realloc(phrase, SIZE * (size_t) total_num_words);

		ch = check_file(fp, marker, phrase);

		if (repeat_word(marker, phrase))
			add_count(repeat_marker, phrase);
		else if (ch)
			break;
	}

	fclose(fp);
	highest(phrase);

	printf("The word with the highest number of appearances is: %s\n", phrase[final_marker].word);
	printf("Number of appearances: %d", phrase[final_marker].count);

	free(phrase);
	return 0;
}


char create_word(FILE *fp, int marker, struct Word_setup *phrase);
void clean_word(int marker, struct Word_setup *phrase);

int check_file(FILE *fp, int marker, struct Word_setup *phrase)
{
	if (create_word(fp, marker, phrase) == EOF)
		return 1;
	else 
		clean_word(marker, phrase);
	return 0;
}


char create_word(FILE *fp, int marker, struct Word_setup *phrase) //scans the file, and creates the member array
{
	char ch;
	int i;

	for (i = 0; i < max_length; i++)
	{
		ch = fgetc(fp);

		if (ch == 39 || isalpha(ch))
			phrase[marker].word[i] = ch;
		else if ((ch > 31) && (ch < 65))
			break;
		else if ((ch > 90) && (ch < 97))
			break;
		else if ((ch > 122) && (ch < 127))
			break;
		else if (ch == EOF)
			return EOF;
	}

	add_count(marker, phrase);

	return 0;
}


void clean_word(int marker, struct Word_setup *phrase) //Gets rid of any characters that arent either in the alphabet, or an apostrophe
{	
	int i;

	for (i = 0; i < max_length; i++)
	{
		if (phrase[marker].word[i] == 39 || isalpha(phrase[marker].word[i])); //39 is the ASCII value for: '
		else
			phrase[marker].word[i] = '\0';
	}
}

int repeat_word(int marker, struct Word_setup *phrase)
{
	int i;

	for (i = 0; i < total_num_words; i++)
	{
		repeat_marker = i;
		if (strcmp(phrase[marker].word, phrase[i].word) == 0)
			return 1;
	}

	return 0;
}


void add_count(int marker, struct Word_setup *phrase) //Adds a count to the current word if it already exists, obviously not needed, but I though it'd be easier to understand
{
	phrase[marker].count++;
}


void highest(struct Word_setup *phrase) //Determines the max number of appearances, as well as giving the slot in the array which we'll need to call (marker)
{
	int i, max;

	for (i = 0, max = 0; i < total_num_words; i++)
	{
		if (phrase[i].count > max)
		{
			final_marker = i;
			max = phrase[i].count;
		}
	}
}


/*
I finished my whole program, with 0 compilation errors, yet for some reason it doesn't seem to accomplish my goal. I've had a 2 other friends in my class look over it, and they don't see a flaw in my thinking. Obviously we are still new to programming, so there may be errors we have no idea about.

The goal of this program is to read a file, and output the word with the max appearances, as well as the number of appearances. I did this by creating an array of structures, where each structure stores the word, as well as the count. I would make the original array dynamically changing, because there can be a ton of different types of words, but for now I figured that there can't be more than 200 different types of words. I have a universal marker so that I can keep track of the array slot which I'm creating the word at. **BTW** it outputs no word, as well as 0 for the number of appearances.
*/
