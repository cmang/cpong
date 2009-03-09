/* This should contain some functions for testing a score as an integer value to see if it beats the top 
if it does, add it.  If not, diregard it. */

#include "score.h"

/* Pong game will ask newscore, "is this score a high score?" .. Newscore adds it and returns TRUE if it is */
/* or returns FALSE if it isn't */

BOOL newScore(int score)
{
    /* Open SQLite DB for scores */
    BOOL isHigh = testScore(score);     /* Is testScore high? */
    if (isHigh == TRUE) addScore(score);
    return isHigh;
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


