// Definitions important to quarto.
// (c) 2017 duane a. bailey
#ifndef QUARTO_H
#define QUARTO_H
#include <curses.h>

// encoding of the play state
typedef struct board {
  int freeset;
  int piece;
  int mat[16];
} board;

// Dimensions of screen, as reported by curses
#define MAXROWS LINES
#define MAXCOLS COLS
#define CTRL(ch) (ch&~(128|64|32))

// Location of important files:
#define HELPSCREEN "HELP"
#define OMGSCREEN "WORK"

// global variables:
extern int BDay[];     // date you were born

// various defines
#define RC2P(r,c) (4*r+c)
#define ALL16 0xFFFF
#define EMPTY -1
#define HIGHLIGHT 0x10

// routines that concern students:
extern int get(board *b, int r, int c);
extern void highlight(board *b, int r, int c);
extern int win(board *b);

// (see documentation in quarto.c)
extern board *copyBoard(board *orig);
extern int cplay(board *b, int cr, int cc, char *you, char *opponent);
extern void drawBoard(board *l, int cr, int cc, int highlight);
extern int go(board *l, int direction);
extern void help(board *l);
extern void initialize();
extern void message(char *msg);
extern void mvstr(int r, int c, char *s, int highlight);
extern void mvstrclr(int r, int c, char *s, int highlight);
extern board *newBoard();
extern void OMG(board *l);
extern void pieces(board *b, int highlight);
extern int play(board *b, int cr, int cc, char *you, char *opponent);
extern int playToNotHelp(board *b, int *rp, int *cp);
extern int tryToWin(board *b, int *rp, int *cp);
extern void shutdown();
extern int undo(board *l);
extern void update(board*b, int r, int c, int highlight);
extern void updateStats(board *l);
extern int valid(board *b, int r, int c);
#endif
