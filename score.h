/* This should contain some functions for testing a score as an integer value to see if it beats the top 
if it does, add it.  If not, diregard it. */

typedef int BOOL;

#define FALSE 0
#define TRUE 1

typedef struct
{
    char username[40];
    int score;
} scoreEntry;

BOOL printScores();

BOOL testScore(int score);

BOOL addScore(char *player, int score);

BOOL makeBoard(void);

BOOL saveScore();

BOOL sortScores();

