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

#define SCOREFILE "/tmp/pong-scores.dat"

scoreEntry gameScore[10];

BOOL loadScores()
{
    int ncount, nscore;
    char *cusername, cscore[40];

    /* does the file exist? */
    fp = fopen(SCOREFILE, "rw");
    if (fp == NULL)
    {
        //printf("Score file not found.\n");
        makeBoard();
        fopen(SCOREFILE, "rw");
    }
    //else printf("Score file found.\n");
    if (fp == NULL)
    {
        perror("Error opening scores.dat."); 
        return FALSE;
    }
    else
    {
        for (ncount = 0; ncount < 10; ncount++)
        {
            cusername = malloc(sizeof(char) * 80);
            //fscanf(fp, "%s\n%i\n", cusername, &nscore);
            fgets(cusername, 50, fp);
            if (cusername[strlen(cusername) - 1] == '\n')
                cusername[strlen(cusername) - 1] = '\0';
            fgets(cscore, 20, fp);
            if (cscore[strlen(cscore) - 1] == '\n')
                cscore[strlen(cscore) - 1] = '\0';

            //use strtol or atoi() to convert cscore into nscore, kthx.
            nscore = strtol(cscore, NULL, 10);
            gameScore[ncount].score = nscore;     // copy score to struct entry.
            strncpy(gameScore[ncount].username, cusername, 39);
            
            free(cusername);
        }
        return TRUE;
    }
}

BOOL printScores()
{
    int ncount;
    printf("High Scores:\n");
    for (ncount = 0; ncount < 10; ncount++)
        printf("%s: %i\n", gameScore[ncount].username, gameScore[ncount].score);
    return TRUE;
}


BOOL testScore(int score)   /* See if the score qualifies for top10 */
{
    if (score <= gameScore[9].score)
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

    // This should be done repeatedly until it goes through the entire list without
    // any re-ordering.  This is the sorter!
    //printf("Sorting scores.\n");
    pass = FALSE;
    while (pass == FALSE)
    {
        pass = TRUE;    // If pass doesn't get set to FALSE, list is sorted
        for (ncount=9; ncount > 0; ncount--)
        {
            if (gameScore[ncount].score > gameScore[ncount-1].score) // If the lower score
            // is higher than the score above it, switch them around.
            {
                cusername = malloc(sizeof(char)*80);                // Make temp storage
                strncpy(cusername, gameScore[ncount].username, 39); // low.name -> temp
                nscore = gameScore[ncount].score;                   // low.score -> temp
                strncpy(gameScore[ncount].username, gameScore[ncount-1].username, 39); // high.name -> low
                gameScore[ncount].score = gameScore[ncount-1].score; // high.score -> low
                strncpy(gameScore[ncount-1].username, cusername, 39); // temp.name -> high
                gameScore[ncount-1].score = nscore;     // temp.score -> high
                free(cusername);        // free temp storage
                pass = FALSE;           // Try agian - might need further sorting.
            }
        }
    }
    //printf("Done.\n");
    return TRUE;
}
    

BOOL addScore(char *player, int score)
{
    /* Add our score to the list.    */
    /* My thoughts: Make sure it's higher than the bottom score.
       If it is, replace bottom score with it.  Then run 
       sortScores(). */
    //printf("Checking score.\n");
    if (score > gameScore[9].score)
    {
        //printf("Adding score.\n");
        gameScore[9].score = score;
        strncpy(gameScore[9].username, player, 39);
        sortScores();
        saveScores();
        return TRUE;
    }
    else return FALSE;
}

BOOL saveScores()
{
    int ncount;
    //printf("Writing to scores.dat.\n");
    fp = fopen(SCOREFILE, "w");
    if (fp == NULL)
    {
        fclose(fp);
        perror("scores.dat could not be opened for writing..\n");
    }
    else
    {   
        for (ncount = 0; ncount < 10; ncount++)
        {
            fprintf(fp, "%s\n%i\n", gameScore[ncount].username, gameScore[ncount].score);
        }
    }
    fclose(fp);
    //printf("new scores.dat written.\n");
    return TRUE;
}

BOOL makeBoard()
{
    int ncount;
    /* Create a new file containing blank scoreboard records. */
    //printf("Creating scores.dat.\n");
    fp = fopen(SCOREFILE, "w");
    if (fp == NULL)
    {
        fclose(fp);
        perror("scores.dat could not be opened for writing..\n");
    }
    else
    {
        //printf("scores.dat created.\n");
        for (ncount = 0; ncount < 10; ncount++)
        {
            fprintf(fp, "%s\n%i\n", "Nobody", 0);
        }
    }
    fclose(fp);
    return TRUE;
}


