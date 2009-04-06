/* :: Cpong
 * This code is copyright 2001,2006 by Sam Foster. Anyone may modify or
 * redistribute provided credit is given to both parties.  You may use this
 * for non-commercial use.  Commercial use may be granted through permission
 * from the author.
 * 
 * To compile with gcc:
 * gcc -lcurses -o cpong cpong.c
 * 
 *************************
 * greets: daed, loophole, tsilik, academician, #ansipunx, #kgb
 * 
 * Sam (cmang)
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <curses.h>
#include <poll.h>
#include <err.h>
#include <string.h>

#include "score.h"

#define NUMBALLS 3
#define DEFDELAY 75

int maxline, maxcol, ch, c, edge, score, balls;
char *player;

struct pollfd pfd[1];

struct where
{
    int me;
    int you;
    int meline;
    int youline;
} paddle;

struct moving
{
    int x;
    int y;
    int xdir;
    int ydir;
    int oldx;
    int oldy;
} ball;


/* function prototypes */

int pausegame();
int drawpaddle(int where, int line);
int ponginput();
int drawboard(int maxline, int maxcol);
int moveball(int delay);
int newball();
int endgame();

/* end of function prototypes */


/* BEGIN main() FUNCTION */
int main(int argc, char *argv[])
{
    unsigned int delay = DEFDELAY;
    delay *= 1000;
    score = 0;
    balls = NUMBALLS;

    /* parse command-line arguments */
    while ((ch = getopt(argc, argv, "d:b:h")) != -1)
        switch (ch)
        {
            case 'd':
                delay = strtoul(optarg, NULL, 10);
                if (delay < 1 || delay > 1000) errx(1, "invalid delay (1-1000)");
                delay *= 1000;
                break;
            case 'b':
                balls = strtoul(optarg, NULL, 10);
                if (balls < 1 || balls > 10) errx(1, "Invalid number of balls: must be 1-10)");
                break;
            case 'h':
                printf("Pong by cmang\n");
                (void)fprintf(stderr, "usage: pong [-d delay] [-b balls]\nDefault delay %i, default number of balls 3\n", DEFDELAY);
                exit(1);
            default:
                printf("Pong by cmang\n");
                (void)fprintf(stderr, "usage: pong [-d delay] [-b balls]\nDefault delay %i, default number of balls 3\n", DEFDELAY);
                exit(1);
        }
    initscr(); clear(); cbreak(); refresh();                        
    newball();
    drawboard(maxline, maxcol);
    paddle.me = (maxcol /2);
    paddle.meline = (maxline -2);
    drawpaddle(paddle.me, paddle.meline);
    move(0, 5);
    printw("Balls: %d  Score: %d", balls, score);

/* OMG the the Game Loop */

    do
    {
        c = ponginput();
        if (c == 0) moveball(delay);
        if (c == 'j')
        {         /* move left */ /* should comment this block
                                                   someday? ... maybe.  these blocks suck,
                                                   but they're the meat of the paddle action */
            if (paddle.me <= 4) (edge = 2);
            if (paddle.me > 4) (edge = 1);
            if (edge == 1)
            {
                paddle.me = (paddle.me -1);
                drawpaddle(paddle.me, paddle.meline);
                move(paddle.meline, (paddle.me +4));
                printw(" ");
                refresh();
                if (paddle.me >4)
                {
                    paddle.me -= 1;
                    drawpaddle (paddle.me, paddle.meline);  
                    move(paddle.meline, (paddle.me +4));
                    printw(" ");    
                    refresh();
                    move(0,0); 
                }
            }
        }
        if (c == 'k')
        {         /* move right */
            if (paddle.me >= (maxcol -5)) (edge = 2);
            if (paddle.me < (maxcol -5)) (edge = 1);
            if (edge == 1)
            {
                paddle.me = (paddle.me +1);
                drawpaddle(paddle.me, paddle.meline);
                move(paddle.meline, (paddle.me -4));
                printw(" ");
                refresh();
                if (paddle.me < (maxcol -5))
                {
                    paddle.me += 1;
                    drawpaddle(paddle.me, paddle.meline);
                    move(paddle.meline, (paddle.me -4));
                    printw(" ");
                    refresh();
                    move(0,0);
                }
            }
        }
        if (c == 'p') pausegame();
        if (c == 'q')
        {         /* quit */
            endgame();
        }
    }
    while (balls > 0);
    endgame();              /* we run the endgame function, so the
                               rest */
    nocbreak(); endwin();   /* never gets executed */
    printf("\n");
    return 0;
}

/* end main */

int endgame()
{
    nocbreak();
    //fflush(stdout);
    refresh();
    endwin();
    fflush(stdout);
    loadScores();
    printf("\n%s quit with score of %d.\n", player, score); 
    if (testScore(score) == TRUE)       /* Score stuff.. test, add, etc. */
    {
        char scoreplayer[40];
        printf("A new high score!\n");
        printf("Please enter your name, or press Enter to use \"%s\": ", player);
        fflush(stdout);
        fflush(stdin);
        fgets(scoreplayer, 40, stdin);
        if (scoreplayer[strlen(scoreplayer) - 1] == '\n')
        scoreplayer[strlen(scoreplayer) - 1] = '\0';
        if (strlen(scoreplayer) == 0) strncpy(scoreplayer, player, sizeof(player));
        //fscanf(stdin, "%39s", scoreplayer);
        addScore(scoreplayer, score);
        saveScores();
    }
    else printf("You did not acheive a high score.\n");
    printScores();
    fflush(stdin);
    free(player);
    exit(0);
}

int newball()
{
    getmaxyx(stdscr, maxline, maxcol);
    ball.x = (maxline /2);
    ball.y = (maxcol /2);
    ball.xdir = 1;
    ball.ydir = 1;
    return 0;
}

int moveball(int delay)
{
    move(ball.x, ball.y);
    printw("O");
    move(0, 0);
    refresh();
    if (ball.xdir == 1) { ball.oldx = ball.x; ball.x--; }
    if (ball.ydir == 1) { ball.oldy = ball.y; ball.y--; }
    if (ball.xdir == 2) { ball.oldx = ball.x; ball.x++; }
    if (ball.ydir == 2) { ball.oldy = ball.y; ball.y++; }
    usleep(delay);
    /*      usleep(20000); */
    move (ball.oldx, ball.oldy);
    printw(" ");
    move(0, 0);
    printw(" ");
    refresh();
    if (ball.x == 2) ball.xdir = 2;
    /*      if (ball.x == (maxline -3)) ball.xdir = 1; */
    if (ball.x >= (maxline -3))
    {
        /*              if ((ball.y > (paddle.me +4)) || (ball.y < (paddle.me -4))) {
                        miss();   
                        } */
        /* if ball hits paddle */
        if ((ball.y <= (paddle.me +4)) && (ball.y >= (paddle.me -4)))
        {
            score += 50;
            move(0, 5);
            printw("Balls: %d  Score: %d", balls, score);
            ball.xdir = 1;
        }
    }
    if (ball.x >= (maxline -2))
    {
        /*        if ((ball.y > (paddle.me +4)) || (ball.y < (paddle.me -4))) { */
        move(0, (maxcol -17));
        printw("Missed the ball");
        refresh();
        sleep(1);
        move(ball.x, ball.y);
        printw(" ");
        move(0, (maxcol -17));
        printw("               ");
        balls--;
        move(0, 5);
        printw("Balls: %d  Score: %d", balls, score);
        newball();
        /*        } */
    }

    if (ball.y == 1) ball.ydir = 2;
    if (ball.y == (maxcol -2)) ball.ydir = 1;
    return 0;
}

int drawboard(int maxline, int maxcol)
{
    int a, b;
    const char helpshit[] = "j - left, k - right, p - pause, q - quit";
    player = malloc(strlen(getenv("USER")));
    strcpy(player, getenv("USER"));
    move(0,1);
    move(1,1);
    a = 0;
    while (a != maxcol)
    {
        move(1, a);
        printw("_");
        a++;
    }
    move(maxline -1, 2);
    printw("%s", player);
    move(maxline -1, ((maxcol - sizeof(helpshit)) -2) );
    printw("%s", helpshit); 
    a = 2;
    while (a != maxline)
    {
        getmaxyx(stdscr, maxline, maxcol);
        move(a, 0);
        printw("|");
        a++;
    }
    a = 2;
    b = maxcol;
    while (a != maxline)
    {
        getmaxyx(stdscr, maxline, maxcol);
        b = maxcol;
        move(a, maxcol -1);
        printw("|");
        a++;
    }
    return 0;
}

int ponginput()
{
    char c, d;
    d = 0;
    pfd[0].fd = 0;
    pfd[0].events = POLLIN; 
    if ( (poll(pfd, 1, 0)) > 0)
    {
        if (pfd[0].events & POLLIN)
        {
            move(0,0);
            c = (getch()); 
            move(0,0);
            printw(" ");
        }
        return((int)c);
    }
    /*      if (read(0, &c, 1) != 1) {
            printw("Received EOF, uncool.");
            endgame(); }
            */
    return(0);
}

int drawpaddle(int where, int line)
{
    move(line, (where -3));
    /*      printw("---+---"); */
    /*      printw("MMMXMMM"); */
    attron(A_REVERSE);
    printw("       ");
    attroff(A_REVERSE);
    return 0;
}

int pausegame()
{
    move(0, (maxcol -36));
    printw("Game Paused, press any key to resume");
    move(ball.x, ball.y);
    printw("O");
    move(0,0);
    getch();
    move(ball.x, (ball.y +1));
    printw(" ");
    move(0, (maxcol -36));
    printw("                                    ");
    move(0,0);
    return 0;
}
