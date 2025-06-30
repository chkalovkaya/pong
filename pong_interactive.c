#include <math.h>
#include <ncurses.h>
#include <stdio.h>

const int field_width = 80;
const int field_height = 25;
int field_start_x, field_start_y;
int field_center_x, field_center_y;

int p1_x, p1_y;
int p1_score = 0;

int p2_x, p2_y;
int p2_score = 0;

int ball_pos_x, ball_pos_y;
int ball_dir_x = 1;
int ball_dir_y = 1;

int winner = 0;

void initNcurses();
void initParams(int col, int row);
void game(int col, int row);
void resetScore();
void handleMove(char c);
int isMove(char c);
void movePlayer(char c);
void moveBall();
int isHit();
int isBallInPlayerY(int y, int player_y);
void handleHit();
int isScore();
void handleScore();
void resetField();
void draw();
void drawScore();
void drawWinner();
void drawEnd();
void drawField();
void drawBall();
void draw_p(int p_x, int p_y);
void endGame(int col, int row);

int main(void) {
  initNcurses();

  int row, col;
  getmaxyx(stdscr, row, col);

  initParams(col, row);
  game(col, row);

  return 0;
}

void initNcurses() {
  initscr();
  cbreak();
  noecho();
  halfdelay(1);
  curs_set(0);
  keypad(stdscr, true);
}

void initParams(int col, int row) {
  field_start_x = col / 2 - field_width / 2;
  field_start_y = row / 2 - field_height / 2;
  field_center_x = col / 2;
  field_center_y = row / 2;

  p1_x = field_start_x + 1;
  p1_y = field_center_y;

  p2_x = field_start_x + field_width - 2;
  p2_y = field_center_y;

  ball_pos_x = col / 2;
  ball_pos_y = row / 2;
}

void game(int col, int row) {
  resetScore();
  while (!winner) {
    char c = getch();
    handleMove(c);
    refresh();
    clear();
    draw();
  }
  endGame(col, row);
}

void resetScore() {
  winner = 0;
  p1_score = 0;
  p2_score = 0;
}

void handleMove(char c) {
  if (isMove(c) && c != ' ') {
    movePlayer(c);
  }
  moveBall();

  if (isHit()) {
    handleHit();
  }
  if (isScore()) {
    handleScore();
    resetField();
  }
}

int isMove(char c) {
  if ((c == 'a' || c == 'z' || c == 'A' || c == 'Z') ||
      (c == 'k' || c == 'm' || c == 'K' || c == 'M') || (c == ' ')) {
    return 1;
  }
  return 0;
}

void movePlayer(char c) {
  if ((c == 'a' || c == 'A') && (p1_y - 2 > field_start_y)) {
    p1_y--;
  }
  if ((c == 'z' || c == 'Z') && (p1_y + 3 < field_start_y + field_height)) {
    p1_y++;
  }
  if ((c == 'k' || c == 'K') && (p2_y - 2 > field_start_y)) {
    p2_y--;
  }
  if ((c == 'm' || c == 'M') && (p2_y + 3 < field_start_y + field_height)) {
    p2_y++;
  }
}

void moveBall() {
  if (ball_pos_y == (field_start_y + field_height - 2) ||
      ball_pos_y == field_start_y + 1) {
    ball_dir_y *= -1;
  }
  ball_pos_x += ball_dir_x;
  ball_pos_y += ball_dir_y;
}

int isHit() {
  int player_pos_y = ball_dir_x == 1 ? p2_y : p1_y;
  int player_pos_x = ball_dir_x == 1 ? p2_x : p1_x;

  if (ball_pos_x == player_pos_x && isBallInPlayerY(ball_pos_y, player_pos_y)) {
    return 1;
  }
  return 0;
}

int isBallInPlayerY(int y, int player_y) {
  if (y == player_y || y == player_y + 1 || y == player_y - 1) {
    return 1;
  }
  return 0;
}

void handleHit() {
  ball_dir_x *= -1;
  ball_pos_x += ball_dir_x * 2;
}

int isScore() {
  if (ball_pos_x <= field_start_x + 1 ||
      ball_pos_x >= field_start_x + field_width - 2) {
    return 1;
  }
  return 0;
}

void handleScore() {
  ball_dir_x == 1 ? p1_score++ : p2_score++;
  ball_dir_x *= -1;

  if (p1_score == 21 || p2_score == 21) {
    winner = (p1_score == 21) ? 1 : 2;
  }
}

void resetField() {
  ball_pos_x = field_center_x;
  ball_pos_y = field_center_y;
  p1_y = field_center_y;
  p2_y = field_center_y;
}

void draw() {
  if (winner) {
    drawWinner();
  }

  drawScore();
  drawField();
  drawBall();
  draw_p(p1_x, p1_y);
  draw_p(p2_x, p2_y);

  if (winner) {
    drawEnd();
  }
}

void drawWinner() {
  mvprintw(field_start_y - 3, field_center_x - 6, "PLAYER %d WIN!", winner);
}

void drawEnd() {
  mvprintw(field_center_y, field_center_x - 26, "[R] - restart");
  mvprintw(field_center_y, field_center_x + 16, "[Q] - quit");
}
void drawScore() {
  mvprintw(field_start_y - 1, field_center_x - 5, "%02d", p1_score);
  mvprintw(field_start_y - 1, field_center_x - 2, "SCORE");
  mvprintw(field_start_y - 1, field_center_x + 4, "%02d", p2_score);
}

void drawField() {
  for (int y = field_start_y; y < field_start_y + field_height; y++) {
    for (int x = field_start_x; x < field_start_x + field_width; x++) {
      if (x == field_center_x) {
        mvprintw(y, x, "|");
      } else if (y == field_start_y || y == field_start_y + field_height - 1) {
        mvprintw(y, x, "-");
      } else if (x == field_start_x || x == field_start_x + field_width - 1) {
        mvprintw(y, x, "|");
      } else
        mvprintw(y, x, " ");
    }
  }
}

void drawBall() { mvprintw(ball_pos_y, ball_pos_x, "*"); }

void draw_p(int p_x, int p_y) {
  for (int i = -1; i <= 1; i++) {
    mvprintw(p_y + i, p_x, "|");
  }
}

void endGame(int col, int row) {
  refresh();
  clear();
  draw();

  char c;
  do {
    c = getch();
  } while (!(c == 'r' || c == 'R' || c == 'q' || c == 'Q'));

  if (c == 'r' || c == 'R') {
    game(col, row);
  } else if (c == 'q' || c == 'Q') {
    endwin();
  }
}