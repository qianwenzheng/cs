/*
 * A curses-based implementation of Quarto.
 * (c) 2017 duane a. bailey
 *
 * See HELP, or type '?' in the running game for help on the puzzle.
 * See man ncurses for help understanding the curses interface.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <ctype.h>
#include "quarto.h"

int  boardTop, boardLeft; // top left of the screen in the current size
int manual = 1;           // manual operation (-c for computer-assist)
long BTime; 	          // seconds you've been alive

// pick a random integer
int randint(int max)
{
  return random() % max;
}

// write a message
void message(char *msg)
{
  move(0,0);
  clrtoeol();
  mvstrclr(0,(COLS-strlen(msg))/2,msg,0);
  move(0,0);
}

// a curses extension: write string at [r,c], clear to end of line
void mvstrclr(int r, int c, char *s, int flags)
{
  mvstr(r,c,s,flags);
  clrtoeol(); // clear the tail of the line
}

void mvstr(int r, int c, char *s, int flags)
{
  move(r,c);
  while (*s) addch(*s++ | flags);
}

// write pieces across bottom of board
void pieces(board *b, int highlight)
{
  char buffer[512];
  char c;
  strcpy(buffer,"[ ");
  char *bp = buffer+strlen(buffer);
  int i;
  for (i = 0; i < 16; i++) {
    if (!(b->freeset & (1<<i))) continue;
    if (i < 10) c = i+'0';
    else c = i-10+'A';
    *bp++ = c;
    *bp++ = ' ';
    *bp = 0;
  }
  *bp++ = ']';
  *bp = 0;
  move(boardTop+14,0);
  clrtoeol();
  bp = buffer;
  mvstrclr(boardTop+14,(COLS-strlen(buffer))/2,buffer,highlight?A_REVERSE:0);
  move(0,0);
  refresh();
}

// initialize the screen; called once, at startup.
void initialize()
{
  struct tm bd[1];
  // curses initialization:
  initscr();
  cbreak();
  noecho();
  nonl();
  intrflush(stdscr, FALSE);
  keypad(stdscr,TRUE);
  srandom(time(0));

  // birthday determination for biometrics
  bd->tm_sec = bd->tm_min = bd->tm_hour = 0;
  bd->tm_mday = BDay[1];
  bd->tm_mon = BDay[0]-1;
  bd->tm_year = BDay[2]-1900;
  BTime = mktime(bd);
  
  message("Welcome to Quarto -- type '?' for help, '^G' to quit.");
}

// shut down curses and prepare for typing into shell
void shutdown()
{
  endwin();
}

// drawBoard the board in its current state
void drawBoard(board *b, int cr, int cc, int active)
{
  int r, c, i;

  // set these for later use: these margins are computed
  boardTop = (MAXROWS-(4*3+1))/2;
  boardLeft = (MAXCOLS-(4*5+1))/2;

  // clear the screen (faster way? see man ncurses)
  for (i = 1; i < MAXROWS; i++) {
    move(i,0); clrtoeol();
  }

  // draw the level
  for (r = 0; r < 4; r++) {
    //int lineLen = 4; //l->cols;
    for (c = 0; c < 4; c++) {
      // update is responsible for determining the correct representation
      update(b,r,c,active&& r == cr && c == cc);
    }
  }
  // update biometric stats
  updateStats(b);
  // repaint screen
  refresh();
}

// convert piece to static 4 character string (hack)
char *b2s(int c)
{
  static char result[5];
  result[0] = ((c>>3)&1) + '0';
  result[1] = ((c>>2)&1) + '0';
  result[2] = ((c>>1)&1) + '0';
  result[3] = ((c>>0)&1) + '0';
  return result;
}

// draw the board at (r,c), highlighting for win or cursor if necessary
void update(board *b, int r, int c, int cursor)
{
  int ch = get(b,r,c);
  int rb = 3*r+boardTop;  // row base
  int cb = 5*c+boardLeft; // column base
  char buffer[20];
  int flags = cursor ? A_REVERSE : 0;
  mvstr(rb,cb,"+----+", 0);
  if (ch != EMPTY) {
    if (ch & HIGHLIGHT) {
      flags ^= A_REVERSE;
    }
    mvstr(rb+1,cb,"|",0);
    sprintf(buffer,"%s",b2s(ch&0xF));
    mvstr(rb+1,cb+1,buffer,flags);
    mvstr(rb+1,cb+5,"|",0);
    mvstr(rb+2,cb,"|",0);
    sprintf(buffer,"  %X ",ch&0xF);
    mvstr(rb+2,cb+1,buffer,flags);
    mvstr(rb+2,cb+5,"|",0);
  } else {
    mvstr(rb+1,cb,"|",0);
    mvstr(rb+1,cb+1,"    ",flags);
    mvstr(rb+1,cb+5,"|",0);
    mvstr(rb+2,cb,"|",0);
    mvstr(rb+2,cb+1,"    ",flags);
    mvstr(rb+2,cb+5,"|",0);
  }
  mvstr(rb+3,cb,"+----+",0);
}

// write important statistics to the screen
void updateStats(board *l)
{
  char buffer[80];
  double days,m,e,p;

  strcpy(buffer,"Current Biometric Indicators");
  mvstrclr(LINES-3,(COLS-strlen(buffer))/2, buffer,0);
  // bioindicators
  days = (time(0)-BTime)/86400.0;
  p = sin(days/23.0*2.0*M_PI);
  m = sin(days/33.0*2.0*M_PI);
  e = sin(days/28.0*2.0*M_PI);
  sprintf(buffer,"Physical: %+8.6lf  Emotional: %+8.6lf  Mental: %+8.6lf",p,e,m);
  mvstrclr(LINES-2,(COLS-strlen(buffer))/2, buffer,0);
  move(LINES-1,0);
}

// Boss on Deck: print an emacs facade and duck for cover
void OMG(board *lv)
{
  FILE *f = fopen(OMGSCREEN,"r");
  int i,j,ch,l;
  char buffer[100];
  char *status = "-uu-:---F1 gdc.c        All L11     (C-wizard Abbrev)---";
  clear();
  // read the workscreen file, painting as many lines on screen as needed
  for (i = 0; i < LINES-2 && (0 != fgets(buffer,100,f)); i++) {
    l = strlen(buffer);
    for (j = 0; j < l; j++) {
      ch = buffer[j];
      if (isspace(ch)) ch = ' ';  // trash tabs and newlines
      mvaddch(i,j,ch);
    }
  }
  // now, the status line
  l = strlen(status);
  for (j = 0; j < COLS; j++) {
    if (j >= l) ch = '-';	// past the end of status line; fill with '-'
    else ch = status[j];	// within status line
    mvaddch(LINES-2,j,ch | A_REVERSE); // paint in reverse video
  }
  fclose(f);
  move(12,0); // move cursor to 13th line.  why not?
  refresh(); // paint facade
  // wait for the dust to clear
  getch();
  // repaint the level as it currently is
  clear();
  drawBoard(lv,0,0,0);
  updateStats(lv);
  refresh();
}

// print help screen
void help(board *lv)
{
  FILE *f = fopen(HELPSCREEN,"r");
  int i,j,l;
  char buffer[100];
  clear();
  // read the help file painting as many lines on screen as possible.
  for (i = 0; i < LINES && (0 != fgets(buffer,100,f)); i++) {
    l = strlen(buffer);
    for (j = 0; j < l; j++) {
      mvaddch(i,j,buffer[j]);
    }
  }
  fclose(f);
  move(0,0);
  refresh();
  getch();
  clear();
  drawBoard(lv,0,0,0);
  updateStats(lv);
  refresh();
}

// play with a starting board position and an initial piece
// go through the motions of play
// read in a key and act on it
// most controls follow the emacs movement keys.
int play(board *b, int cr, int cc, char *you, char *opponent)
{
  int done;
  int placed;
  char buffer[512]; // message work buffer
  int piece = b->piece; // the piece given to you

  // remove piece from the freeset; we're going to play
  b->freeset = b->freeset & ~(1<<piece);
  
  // draw board, highlight cursor
  sprintf(buffer,"Ok, %s, you were given piece %X.  Where do you place it?",you,piece);
  message(buffer);

  placed = 0;
  while (!placed) {
    drawBoard(b,cr,cc,1); // board is highlighted
    pieces(b,0);          // piece pool is not
    refresh();
    int ch = getch();
    switch (ch) {
      // basic motion: emacs
    case CTRL('B'): if (cc > 0) cc--;; break; // move left
    case CTRL('F'): if (cc < 3) cc++; break;  // move right
    case CTRL('N'): if (cr < 3) cr++; break;  // move down
    case CTRL('P'): if (cr > 0) cr--;break;   // move up
    case ' ': // placement
      if (b->mat[RC2P(cr,cc)] != EMPTY) {
	message("There's already a piece there.  Move to place elsewhere.");
	refresh();
	sleep(2);
	sprintf(buffer,"Move to play %X.",piece);
	message(buffer);
	refresh();

      } else {
	b->mat[RC2P(cr,cc)] = piece;
	placed = 1;
      }
      break;
      // the boss key
    case CTRL('M'): OMG(b); break;
	// the help key
    case CTRL('H'): help(b); break;
	// loser key: quit
    case CTRL('G'):
	shutdown();
	exit(0);
	// otherwise, silently do nothing
    default:
	break;
    }
  }

  if (win(b)) {
    // player wins!
    // update the board (user highlighting might have happened)
    sprintf(buffer,"%s Wins! (Type any key quit.)",you);
    message(buffer);
    drawBoard(b,cr,cc,0);
    pieces(b,0);
    updateStats(b);
    refresh();
    getch();
    return 1;
  } else if (b->freeset == 0) {
    // draw! the dishonor
    sprintf(buffer,"%s and %s Draw! (Type any key quit.)",you,opponent);
    message(buffer);
    drawBoard(b,cr,cc,0);
    pieces(b,0);
    updateStats(b);
    refresh();
    getch();
    return 1;
  } else {
    // simple loop to gather piece, or quit
    done = 0;
    while (!done) {
      char c;
      sprintf(buffer,"%s, pick a piece for %s to play.",you,opponent);
      message(buffer);
      drawBoard(b,cr,cc,0);
      pieces(b,1);
      updateStats(b);
      refresh();

      c = getch();
      if (c == CTRL('G')) return 1;
      else if (c == CTRL('M')) OMG(b);
      else if (c == CTRL('H')) help(b);
      else if ((('0' <= c) && (c <= '9')) ||
	  (('a' <= c) && (c <= 'f')) ||
	  (('A' <= c) && (c <= 'F')))
	{
	  if (c <= '9') c = c - '0';
	  else if (c <= 'F') c = (c-'A')+10;
	  else if (c <= 'f') c = (c-'a')+10;
	  if (!(b->freeset & (1<<c))) {
	    message("Not a valid piece.  Try again.");
	    refresh();
	    continue;
	  }
	  b->piece = c;
	  pieces(b,1);
	  refresh();
	  done = 1;
	  if (manual) {
	    return play(b, cr, cc, opponent, you);
	  } else {
	    return cplay(b, cr, cc, opponent, you);
	  }
	}
    }
  }
  return 1;
}

int cplay(board *b, int cr, int cc, char *you, char *opponent)
{
  int done;
  int placed;
  char buffer[512];     // message work buffer
  int piece = b->piece; // computer's piece
  int tempr, tempc;
  
  // remove piece form the freeset
  b->freeset = b->freeset & ~(1<<piece);
  
  // draw the board, highlight cursor
  sprintf(buffer,"Ok, Computer, you were given piece %X.  Where do you place it?",piece);
  message(buffer);
  drawBoard(b,cr,cc,1); // highlight board
  pieces(b,0);          // redraw pieces
  refresh();

  if (tryToWin(b,&tempr,&tempc)) {  // detect a way to win
    cr = tempr;
    cc = tempc;
  } else if (playToNotHelp(b,&tempr,&tempc)) { // detect a way not to lose
    cr = tempr;
    cc = tempc;
  } else {
    // otherwise, play randomly
    placed = 0;
    do {
      cr = randint(4);
      cc = randint(4);
      placed = b->mat[RC2P(cr,cc)] == EMPTY;
    } while (!placed);
    // found a place to play
  }
  b->mat[RC2P(cr,cc)] = piece;

  sleep(1); // don't appear too fast
  if (win(b)) {
    // player wins!
    message("Computer Wins! (Type any key to quit.)");
    drawBoard(b,cr,cc,0);
    pieces(b,0);
    updateStats(b);
    refresh();
    getch();
    return 1;
  } else if (b->freeset == 0) {
    // draw! the dishonor
    sprintf(buffer,"%s and %s Draw! (Type any key quit.)",you,opponent);
    message(buffer);
    drawBoard(b,cr,cc,0);
    pieces(b,0);
    updateStats(b);
    refresh();
    getch();
    return 1;
  } else {
    char c;
    sprintf(buffer,"%s now picks a piece for %s to play.",you,opponent);
    message(buffer);
    drawBoard(b,cr,cc,0);
    pieces(b,1);
    updateStats(b);
    refresh();
    sleep(1); // don't appear too fast

    // pick a random piece
    done = 0;
    do {
      c = randint(16);
      done = b->freeset & (1<<c);
    } while (!done);

    // hand it to other player
    b->piece = c;
    pieces(b,0);
    updateStats(b);
    refresh();
    return play(b, cr, cc, opponent, you);
  }
  return 1;
}


// create a new board for play
board *newBoard()
{
  board *result = (board*)malloc(sizeof(board));
  int i;
  
  result->freeset = ALL16;
  result->piece = EMPTY;
  for (i = 0; i < 16; i++) {
    result->mat[i] = EMPTY;
  }
  return result;
}

// copy the board, including all state
board *copyBoard(board *orig)
{
  board *result = (board*)malloc(sizeof(board));
  int i;
  result->freeset = orig->freeset;
  result->piece = orig->piece;
  for (i = 0; i < 16; i++) {
    result->mat[i] = orig->mat[i];
  }
  return result;
}

// check to see if a board position is valid
int valid(board *b, int r, int c)
{
  int p = RC2P(r,c);
  return b->mat[p] != EMPTY;
}

// get the current encoding of the board position
int get(board *b, int r, int c)
{
  int p = RC2P(r,c);
  return b->mat[p];
}

// highlight the current board at position (r,c)
void highlight(board *b, int r, int c)
{
  b->mat[RC2P(r,c)] |= HIGHLIGHT;
}

// HERE DOWN: COMPUTER LOGIC
// check for move-to-win along dimension
void diddle2(board *b, int alpha, int d, int *rp, int *cp, int *rsp)
{
  int pc = b->piece;
  int win1, win0,v;
  int p = alpha, i;
  int tempr, tempc;
  int count;

  win1 = pc&0xf;
  win0 = (~pc)&0xf;
  count = 0;
  for (p = alpha, i = 0; i<4; i++, p += d) {
    int r,c;
    r = p/4;
    c = p%4;
    if (!valid(b,r,c)) {
      tempr = r;
      tempc = c;
      continue;
    }
    count++;
    v = get(b,r,c);
    win1 &= v;
    win0 &= ~v;
  }
  if (count == 3 && (win1 || win0)) {
    (*rsp) += 1; 
    if (0 == randint(*rsp)) {  // randomize which way we play to win
      *rp = tempr;
      *cp = tempc;
    }
  }
}

// here try to win by placing the current piece on the board
int tryToWin(board *b, int *rp, int *cp)
{
  int wins = 0;
  int p;
  for (p = 0; p < 16; p += 4) diddle2(b, p, 1, rp,cp, &wins);
  for (p = 0; p < 4; p++) diddle2(b, p, 4, rp,cp, &wins);
  diddle2(b, 0, 5, rp,cp, &wins);
  diddle2(b, 3, 3, rp,cp, &wins);
  return wins;
}

// having given up on winning, we make it hard for the opponent.
int playToNotHelp(board *b, int *rp, int *cp)
{
  int r,c;
  int result = 0;
  board *b2 = copyBoard(b);
  int p = b->piece;
  for (r = 0; r < 3; r++) {
    for (c = 0; c < 3; c++) {
      // look at all the board positions.
      // if this position is not used, think about what happens if we play there
      if (!valid(b2,r,c)) {
	int p2;
	// try moving the current piece to that location
	b2->mat[RC2P(r,c)] = p;
	b2->freeset &= ~(1<<p);
	// now, see if any piece will allow the other player to win
	for (p2 = 0; p2 < 16; p2++) {
	  if (b2->freeset & (1<<p2)) {
	    // p2 is free; what happens if we given it to opponent
	    int tempr,tempc;
	    b2->piece = p2;
	    if (!tryToWin(b2,&tempr,&tempc)) {
	      // they can't win; (r,c) is a possible move
	      *rp = r;
	      *cp = c;
	      result++;
	    }
	  }
	}
	// undo our placement of piece at (r,c)
	b2->freeset |= (1<<p);
	b2->mat[RC2P(r,c)] = EMPTY;
      }
    }
  }
  // free up work board
  free(b2);
  return result;
}

// the main method
int main(int argc, char **argv)
{
  // check for computer-assist
  if (argc > 1 && (strcmp(argv[1],"-c") == 0)) manual = 0;
  // start the curses screen manager
  initialize();
  board *b = newBoard();
  b->piece = randint(16);
  help(b);
  if (manual) {
    play(b,randint(4),randint(4),"Alice","Bob");
  } else {
    play(b,randint(4),randint(4),"Human","Computer");    
  }
  shutdown();
  return 0;
}
