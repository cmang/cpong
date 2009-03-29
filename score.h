/* This should contain some functions for testing a score as an integer value to see if it beats the top 
if it does, add it.  If not, diregard it. */

typedef int BOOL;

#define FALSE 0
#define TRUE 1

typedef struct {
    char username[40];
    int score;
} scoreEntry;

/* Pong game will ask newscore, "is this score a high score?" .. Newscore adds it if it is */
BOOL printScores();

BOOL testScore(int score);
    /* Do SQL stuff here to find out if the score is in the top 10. */
    /* For now let's just say they're all true. */

BOOL addScore(int score);

BOOL makeBoard(void);

