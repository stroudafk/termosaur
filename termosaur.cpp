/**
 *
 *  A terminal game:
 *
 *   | |_ ___ _ __ _ __ ___   ___  ___  __ _ _   _ _ __
 *   | __/ _ \ '__| '_ ` _ \ / _ \/ __|/ _` | | | | '__|
 *   | ||  __/ |  | | | | | | (_) \__ \ (_| | |_| | |
 *    \__\___|_|  |_| |_| |_|\___/|___/\__,_|\__,_|_|
 *
 *
 * Home
 *    https://github.com/b37t1td/termosaur
 *
 * Author
 *    Lady Int3gra
 *
 * Made with
 *    Love
 *
 *                     GNU GENERAL PUBLIC LICENSE
 *                        Version 3, 29 June 2007
 *
 *  Copyright (C) Lady Int3gra <https://github.io/int3gra>
 *
 *  Everyone is permitted to copy and distribute verbatim copies
 *  of this license document, but changing it is not allowed.
 *
 */

#include <iostream>
#include <curses.h>
#include <stdlib.h>

#include "termosaur.hpp"

int main() {
  setlocale(LC_ALL, "");

  Termosaur *termo = new Termosaur();

  termo->start();

  delete termo;

  return 0;
}

void Termosaur::start() {
  char key;

  while(1) {
    timer = (timer >= TIMER_RANGE ? 0 : timer + 1);
    key = getch();

    switch(key) {
      case ' ':
        if (debounceTimer != 0)
          break;

        isJump = true;
        jumpTimer = TIMER_RANGE * 2.4;
        break;
      case 'q':
        return;
    }

    draw();

    if (key != -1) {
      debounceTimer = TIMER_RANGE;
    } else {
      debounceTimer = (debounceTimer == 0 ? 0 : debounceTimer - 1);
    }
  }
}

void Termosaur::draw() {
  clear();
  drawTerrain();
  drawDino();
  drawBush();
}

void Termosaur::drawBush() {
  bushPos -= 3;

  if (bushPos <= -5) {
    srand(timer);
    bushPos = winSize.x + (rand() % (winSize.x * 2));
  }

  if (bushPos <= (winSize.x - 5)) {
    int y = winSize.y - 1;

    attron(COLOR_PAIR(2));

      mvprintw(y - 3, bushPos, "▄█ █▄");
      mvprintw(y - 2, bushPos, "██ ██");
      mvprintw(y - 1, bushPos + 1, "███");
      mvprintw(y, bushPos + 1,     "███");

    attroff(COLOR_PAIR(2));
  }

  if (bushPos < 23 && bushPos < 6 && !isJump) {
    mvprintw(2, 2, "gameover");
  }
}

void Termosaur::drawTerrain() {
  int i;

  attron(COLOR_PAIR(5));

  for (i = 0; i < winSize.x; i++) {
    mvprintw(winSize.y - 1, i, "▄");

    if (i == 3 && !isJump) {
      i = 23;
    }
  }

  attroff(COLOR_PAIR(5));
}

void Termosaur::drawDino() {
  Point pos = { 5,  winSize.y - 12 };

  if (isJump) {
    pos.y -= 4;
    jumpTimer--;

    if (jumpTimer <= 0) {
      isJump = false;
    }
  }

  attron(COLOR_PAIR(4));

  mvprintw(  pos.y, pos.x, "         ▄███████▄");
  mvprintw(++pos.y, pos.x, "         ██▄██████");
  mvprintw(++pos.y, pos.x, "         █████████");
  mvprintw(++pos.y, pos.x, "         ██████▄▄ ");
  mvprintw(++pos.y, pos.x, "        ██████   ");
  mvprintw(++pos.y, pos.x, " ▌     ███████▄▄▄");
  mvprintw(++pos.y, pos.x, " ██▄  ████████  █");
  mvprintw(++pos.y, pos.x, "  ████████████   ");
  mvprintw(++pos.y, pos.x, "   █████████     ");

  if (isJump) {
    mvprintw(++pos.y, pos.x, "   ██▄   ██▄");
  } else {
    if (timer < (TIMER_RANGE / 2)) {
      mvprintw(++pos.y, pos.x, "   ██    ██▄   ");
      mvprintw(++pos.y, pos.x, "   █▄▄      ");
    } else {
      mvprintw(++pos.y, pos.x, "    ██▄ ██  ");
      mvprintw(++pos.y, pos.x, "         █▄▄  ");
    }
  }

  attroff(COLOR_PAIR(4));
}

void Termosaur::clear() {
  int i;

  if (!emptyLine) {
    emptyLine = new char[winSize.x];

    for (i = 0; i < winSize.x; i++) {
      emptyLine[i] = ' ';
    }
  }

  for (i = 0; i <= winSize.y; i++) {
    mvprintw(i, 0, "%s", emptyLine);
  }
}

Termosaur::Termosaur() {
  startCurses();
  bushPos = 0;
}

Termosaur::~Termosaur() {
  delete emptyLine;
  stopCurses();
}

void Termosaur::startCurses() {
  initscr();
  start_color();
  noecho();
  cbreak();
  curs_set(false);

  init_pair(1, COLOR_RED, COLOR_BLACK);
  init_pair(2, COLOR_GREEN, COLOR_BLACK);
  init_pair(3, COLOR_BLUE, COLOR_BLACK);
  init_pair(4, COLOR_CYAN, COLOR_BLACK);
  init_pair(5, COLOR_MAGENTA, COLOR_BLACK);

  getmaxyx(stdscr, winSize.y, winSize.x);
  keypad(stdscr, true);
  timeout(FPS_TIMEOUT);
}

void Termosaur::stopCurses() {
  endwin();
}
