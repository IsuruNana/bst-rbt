#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include <string.h>

#include "bst.h"
#include "rbt.h"
#include "gt.h"
#include "da.h"
#include "cda.h"
#include "string.h"
#include "scanner.h"

static void trim(char *s);
static void cleanString2(char*);
static char *readData(FILE *fp);
static void readDataIntoRBT(FILE *, RBT *);
static void readDataIntoGT(FILE *, GT *);
static void processRBTCommands(FILE *, FILE *, RBT *);
static void processGTCommands(FILE *, FILE *, GT *);


int main(int argc, char **argv)
{
	int treeType = 1; //default is RBT
	RBT *rbt = NULL;
	GT *gt = NULL;

	if(argv[1][1] == 'v')
	{
		printf("Isuru Nanayakkara\n");
		return 0;
	}

	if(argv[1][1] == 'g') treeType = 0;
	FILE *dataFile = fopen(argv[2], "r");
	FILE *commandsFile = fopen(argv[3], "r");

	if(treeType)
	{
		rbt = newRBT(displaySTRING, compareSTRING);
		readDataIntoRBT(dataFile, rbt);
		processRBTCommands(commandsFile, stdout, rbt);
	}

	else
	{
		gt = newGT(displaySTRING, compareSTRING);
		readDataIntoGT(dataFile, gt);
		processGTCommands(commandsFile, stdout, gt);
	}

	return 0;
}

static void cleanString2(char* s)
{
	//trim(s);

	char *i = s;
	char *j = s;

	while (*i)
	{
		if(isspace((unsigned char)*i))
		{
          while(isspace((unsigned char)*i) || isdigit((unsigned char)*i) || ispunct((unsigned char)*i))
					{
							i++;
					}
			//add a space add the end
			*j++ = ' ';
		}

        //if not alpha
        else if (!isalpha((unsigned char)*i))
            i++;
        //if uppercase make lower
        else if (isupper((unsigned char)*i)) {
            *j++ = tolower((unsigned char)*i);
            i++;
        }
        //if the same then incriment both and move forward
        else if (i == j) {
            i++;
            j++;
        }
        //copy character
        else
            *j++ = *i++;
    }
    //set last value null
    *j = '\0';

	trim(s); //Clean final string
}

static char *readData(FILE *fp)
{
	char *string;

	if (stringPending(fp)) string =  readString(fp);
	else string = readToken(fp);

	return string;
}


static void readDataIntoRBT(FILE *fp, RBT *rbt)
{
	char *data;

	while( (data =readData(fp)) )
	{
		cleanString2(data);
		if(strcmp(data,"") != 0) insertRBT(rbt, newSTRING(data));
	}
}

static void readDataIntoGT(FILE *fp, GT *gt)
{
	char *data;

	while( (data =readData(fp)) )
	{
		cleanString2(data);
		if(strcmp(data,"") != 0) insertGT(gt, newSTRING(data));
	}
}

static void processRBTCommands(FILE *fp, FILE *outputFile, RBT *rbt)
{
	char *info = readData(fp);

	while(!feof(fp))
	{
		cleanString2(info);

		if(strcmp(info, "i") == 0)
		{
			info = readData(fp);
			cleanString2(info);
			if (strcmp(info, "") != 0) insertRBT(rbt, newSTRING(info));
		}

		else if (strcmp(info, "d") == 0)
		{
            info = readData(fp);
            cleanString2(info);
            deleteRBT(rbt, newSTRING(info));
		}

		else if (strcmp(info, "f") == 0)
		{
            info = readData(fp);
            cleanString2(info);
            fprintf(outputFile, "Frequency of %s: %d\n", info, findRBT(rbt, newSTRING(info)));
		}

		else if (strcmp(info, "s") == 0) displayRBT(outputFile, rbt);
		else if (strcmp(info, "r") == 0) statisticsRBT(outputFile, rbt);

        info = readData(fp);
	}
}

static void processGTCommands(FILE *fp, FILE *outputFile, GT *gt)
{
	char *info = readData(fp);

	while(!feof(fp))
	{
		cleanString2(info);

		if(strcmp(info, "i") == 0)
		{
			info = readData(fp);
			cleanString2(info);
			if (strcmp(info, "") != 0) insertGT(gt, newSTRING(info));
		}

		else if (strcmp(info, "d") == 0)
		{
            info = readData(fp);
            cleanString2(info);
            deleteGT(gt, newSTRING(info));
		}

		else if (strcmp(info, "f") == 0)
		{
            info = readData(fp);
            cleanString2(info);
            fprintf(outputFile, "Frequency of %s: %d\n", info, findGT(gt, newSTRING(info)));
		}

		else if (strcmp(info, "s") == 0) displayGT(outputFile, gt);
		else if (strcmp(info, "r") == 0) statisticsGT(outputFile, gt);

        info = readData(fp);
	}
}

static void trim(char * str)
{
    int index, i;
    //Trim leading white spaces

    index = 0;
    while(str[index] == ' ' || str[index] == '\t' || str[index] == '\n')
    {
        index++;
    }

    /* Shift all trailing characters to its left */
    i = 0;
    while(str[i + index] != '\0')
    {
        str[i] = str[i + index];
        i++;
    }
    str[i] = '\0'; // Terminate string with NULL

    /*
     * Trim trailing white spaces
     */
    i = 0;
    index = -1;
    while(str[i] != '\0')
    {
        if(str[i] != ' ' && str[i] != '\t' && str[i] != '\n')
        {
            index = i;
        }

        i++;
    }

    /* Mark the next character to last non white space character as NULL */
    str[index + 1] = '\0';
}
