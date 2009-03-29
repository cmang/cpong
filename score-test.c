/* This should contain some functions for testing a score as an integer value to see if it beats the top 
if it does, add it.  If not, diregard it. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "score.h"

/* game will ask newscore, "is this score a high score?" .. Newscore adds it and returns TRUE if it is */
/* or returns FALSE if it isn't */


/* Let's try to define our interface more clearly, or at least bounce around ideas.

Myabe we should load the entire scoreboard into an array of struct pointers?
struct gameScore {
    char username[40];
    int score;
}

typedef scoreEntry struct gameScore;

BOOL checkScore(int score);
BOOL addScore(char username[40], int score);
scoreEntry * getScores();           // return pointer to entire scoreboard?


*/

FILE *fp;
char a[40];

scoreEntry gameScore[10];

BOOL printScores()
{
    int ncount;
    char *cusername;
    //char cscore[20];     // temporary score holder
    int nscore;

    /* does the file exist? */
    fp = fopen("scores.dat", "r");
    if (fp == NULL)
    {
        //fclose(fp);
        printf("Score file not found.\n");
        makeBoard();
    }
    else printf("Score file found.\n");
    if (fp == NULL) fp = fopen("scores.dat", "r");      // fopen returns pointer to null
    if (fp == NULL) perror("Error opening scores.dat."); 
    else
    {
        fseek(fp, 0, 0);
        for (ncount = 0; ncount < 10; ncount++)
        //while (*fp != EOF)
        {
            cusername = malloc(sizeof(char) * 80);
            fscanf(fp, "%s\n%i\n", cusername, &nscore);
            gameScore[ncount].score = nscore;     // copy score to struct entry.
            strncpy(gameScore[ncount].username, cusername, 39);
            
            printf("%s: %i, %s: %i\n", cusername, nscore, gameScore[ncount].username, gameScore[ncount].score);
            free(cusername);
        }
    }
    //if (fp != NULL) fclose(fp);     /* If the file hasn't been closed yet */
    //fclose(fp);
    return TRUE;
}

BOOL testScore(int score)
{
    /* Do SQL stuff here to find out if the score is in the top 10. */
    /* For now let's just say they're all true. */
    return TRUE;
}

BOOL addScore(int score)
{
    /* Add our score to the list.    */
    return TRUE;
}

BOOL makeBoard(void)
{
    int ncount;
    /* Create a new file containing blank scoreboard records. */
    printf("Creating scores.dat.\n");
    fp = fopen("scores.dat", "w");
    if (fp == NULL)
    {
        fclose(fp);
        printf("scores.dat could not be opened for writing..\n");
    }
    else
    {
        printf("scores.dat created.\n");
        for (ncount = 0; ncount < 10; ncount++)
        {
            fprintf(fp, "%s\n%i\n", "Nobody", 0);
        }
    }
    fclose(fp);
    return TRUE;
}


int main(void)
{
    if (testScore(3200) == TRUE)
    {
        addScore(3200);
        printf("Your score is high!\n");
    }
    else
    {   
        printf("Your score is not high.\n");
    }
    printScores();
    return(0);
}

