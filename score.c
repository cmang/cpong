/* This should contain some functions for testing a score as an integer value to see if it beats the top 
if it does, add it.  If not, diregard it. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "score.h"

/* Let's try to define our interface more clearly, or at least bounce around ideas.

Myabe we should load the entire scoreboard into an array of struct pointers?
struct gameScore {
    char username[40];
    int score;
}

typedef scoreEntry struct gameScore;

 ^ Ended up doing something similar.  Look in score.h

BOOL checkScore(int score);
BOOL addScore(char username[40], int score);
scoreEntry * getScores();           // return pointer to entire scoreboard?


*/

FILE *fp;
char a[40];

#define MYSCORE 3250

scoreEntry gameScore[10];

BOOL loadScores()
{
    int ncount, nscore;
    char *cusername;

    /* does the file exist? */
    fp = fopen("scores.dat", "rw");
    if (fp == NULL)
    {
        printf("Score file not found.\n");
        makeBoard();
        fopen("scores.dat", "rw");
    }
    else printf("Score file found.\n");
    if (fp == NULL) perror("Error opening scores.dat."); 
    else
    {
        for (ncount = 0; ncount < 10; ncount++)
        {
            cusername = malloc(sizeof(char) * 80);
            fscanf(fp, "%s\n%i\n", cusername, &nscore);
            gameScore[ncount].score = nscore;     // copy score to struct entry.
            strncpy(gameScore[ncount].username, cusername, 39);
            
            free(cusername);
        }
    }
    return TRUE;
}

BOOL printScores()
{
    int ncount;
    for (ncount = 0; ncount < 10; ncount++)
        printf("%s: %i\n", gameScore[ncount].username, gameScore[ncount].score);
    return TRUE;
}


BOOL testScore(int score)   /* See if the score qualifies for top10 */
{
    /* Somewhere the scores should be sorted - here might be a good place? */
    if (score < gameScore[9].score)
       return FALSE;
    else
        return TRUE;
}

BOOL sortScores()
{
    /* Quickie-sort, what-have-you? Uses two temporary storage variables .. */
    /* Could have used a temporary struct instead, since the struct is there */ 
    int ncount, nscore;
    char *cusername;
    BOOL pass;

    pass = FALSE;
    printf("Sorting scores.\n");
    // This should be done repeatedly until it goes through the entire list without
    // any re-ordering.  This is the sorter!
    while (pass == FALSE)
    {
        pass = TRUE;
        for (ncount=9; ncount > 0; ncount--)
        {
            if (gameScore[ncount].score > gameScore[ncount-1].score) // If the lower score
            // is higher than the score above it, switch them around.
            {
                cusername = malloc(sizeof(char)*80);
                strncpy(cusername, gameScore[ncount].username, 39);
                nscore = gameScore[ncount].score;
                strncpy(gameScore[ncount].username, gameScore[ncount-1].username, 39);
                gameScore[ncount].score = gameScore[ncount-1].score;
                strncpy(gameScore[ncount-1].username, cusername, 39);
                gameScore[ncount-1].score = nscore;
                free(cusername);
                pass = FALSE;
            }
        }
    }
    printf("Done.\n");
    return TRUE;
}
    

BOOL addScore(char *player, int score)
{
    /* Add our score to the list.    */
    /* My thoughts: Make sure it's higher than the bottom score.
       If it is, replace bottom score with it.  Then run 
       sortScores(). */
    
    if (score > gameScore[9].score)
    {
        gameScore[9].score = score;
        strncpy(gameScore[9].username, player, 39);
        sortScores();
        saveScore();
        return TRUE;
    }
    else return FALSE;
}

BOOL saveScore()
{
    int ncount;
    printf("Writing to scores.dat.\n");
    fp = fopen("scores.dat", "w");
    if (fp == NULL)
    {
        fclose(fp);
        printf("scores.dat could not be opened for writing..\n");
    }
    else
    {   
        for (ncount = 0; ncount < 10; ncount++)
        {
            fprintf(fp, "%s\n%i\n", gameScore[ncount].username, gameScore[ncount].score);
        }
    }
    fclose(fp);
    printf("new scores.dat written.\n");
    return TRUE;
}

BOOL makeBoard()
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


