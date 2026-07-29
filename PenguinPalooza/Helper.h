#ifndef HELPER_H
#define HELPER_H

const int MAXROWS = 20;                  // max number of rows in the valley
const int MAXCOLS = 25;                  // max number of columns in the valley
const int MAXPENGUINS = 100;             // max number of penguins allowed
const int MAXSPECIES = 3;                // max number of species
const int INITIAL_PENGUIN_HEALTH = 3;    // initial penguin health
const double PROTRUSION_DENSITY = 0.11;  // density of protrusions

const int NORTH = 0;
const int EAST  = 1;
const int SOUTH = 2;
const int WEST  = 3;
const int NUMDIRS = 4;
const int BADDIR = -1;

class Valley;

int decodeDirection(char ch);
int randInt(int lowest, int highest);
bool attemptMove(const Valley& v, int dir, int& r, int& c);
bool recommendMove(const Valley& v, int r, int c, int& bestDir);
void clearScreen();

#endif
