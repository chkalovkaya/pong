#include <math.h>
#include <stdio.h>

const int field_width = 80;
const int field_height = 25;
const int field_center_x = field_width / 2;
const int field_center_y = 13;

int ball_pos_x = field_center_x;
int ball_pos_y = field_center_y;
int ball_dir_x = 1;
int ball_dir_y = 1;

const int p1_x = 1;
int p1_y = field_center_y;
int p1_score = 0;

const int p2_x = field_width - 2;
int p2_y = field_center_y;
int p2_score = 0;

int winner = 0;

void drawScore();
void drawField();
void game();
int isBallPos(int x, int y);
int isPlayerPos(int x, int y);
int isYInPlayerY(int y, int player_y);
char move();
int isMove(char c);
void handleMove(char c);
void movePlayer(char c);
void moveBall();
int isHit();
void handleHit();
int isScore();
void handleScore();
void resetField();
void clearScreen();
void endGame();
void drawWinner();

int main(void) {
  drawScore();
  drawField();
  while (!winner) {
    game();
  }
  endGame();
  return 0;
}

void drawScore() {
  for (int x = 0; x < field_width; x++) {
    if (x == field_center_x - 5) {
      printf("%02d", p1_score);
    } else if (x == field_center_x - 3) {
      printf("S");
    } else if (x == field_center_x - 2) {
      printf("C");
    } else if (x == field_center_x - 1) {
      printf("O");
    } else if (x == field_center_x) {
      printf("R");
    } else if (x == field_center_x + 1) {
      printf("E");
    } else if (x == field_center_x + 3) {
      printf("%02d", p2_score);
    } else if (x == field_width - 1) {
      printf(" \n");
    } else {
      printf(" ");
    }
  }
}

void drawField() {
  for (int y = 0; y < field_height; y++) {
    for (int x = 0; x < field_width; x++) {
      if (isBallPos(x, y)) {
        printf("*");
      } else if (x == field_center_x) {
        char c = isBallPos(x, y) ? '*' : '|';
        printf("%c", c);
      } else if (isPlayerPos(x, y)) {
        printf("|");
      } else if (y == 0 || y == field_height - 1) {
        printf("-");
      } else if (x == 0 || x == field_width - 1) {
        printf("|");
      } else
        printf(" ");

      if (x == field_width - 1) {
        printf("\n");
      }
    }
  }
}

int isBallPos(int x, int y) {
  if (x == ball_pos_x && y == ball_pos_y) {
    return 1;
  }
  return 0;
}

int isPlayerPos(int x, int y) {
  if (((x == p1_x) && isYInPlayerY(y, p1_y)) ||
      ((x == p2_x) && isYInPlayerY(y, p2_y))) {
    return 1;
  }
  return 0;
}

int isYInPlayerY(int y, int player_y) {
  if (y == player_y || y == player_y + 1 || y == player_y - 1) {
    return 1;
  }
  return 0;
}

void game() {
  char c = move();
  handleMove(c);
  clearScreen();
  drawScore();
  drawField();
}

char move() {
  char c;
  do {
    c = getchar();
    while (getchar() != '\n')
      ;
  } while (!isMove(c));
  return c;
}

int isMove(char c) {
  if ((c == 'a' || c == 'z' || c == 'A' || c == 'Z') ||
      (c == 'k' || c == 'm' || c == 'K' || c == 'M') || (c == ' ')) {
    return 1;
  }
  return 0;
}

void handleMove(char c) {
  if (c != ' ') {
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

void movePlayer(char c) {
  if ((c == 'a' || c == 'A') && (p1_y - 2 > 0)) {
    p1_y--;
  }
  if ((c == 'z' || c == 'Z') && (p1_y + 3 < field_height)) {
    p1_y++;
  }
  if ((c == 'k' || c == 'K') && (p2_y - 2 > 0)) {
    p2_y--;
  }
  if ((c == 'm' || c == 'M') && (p2_y + 3 < field_height)) {
    p2_y++;
  }
}

void moveBall() {
  if (ball_pos_y == (field_height - 2) || ball_pos_y == 1) {
    ball_dir_y *= -1;
  }
  ball_pos_x += ball_dir_x;
  ball_pos_y += ball_dir_y;
}

int isHit() {
  int player_pos_y = ball_dir_x == 1 ? p2_y : p1_y;
  int player_pos_x = ball_dir_x == 1 ? p2_x : p1_x;

  if (ball_pos_x == player_pos_x && isYInPlayerY(ball_pos_y, player_pos_y)) {
    return 1;
  }
  return 0;
}

void handleHit() {
  ball_dir_x *= -1;
  ball_pos_x += ball_dir_x * 2;
}

int isScore() {
  if (ball_pos_x <= 1 || ball_pos_x >= field_width - 2) {
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

void clearScreen() {
  for (int i = 0; i < 100; i++) {
    printf("\n");
  }
};

void endGame() {
  clearScreen();
  resetField();
  drawWinner();
  drawField();
}

void drawWinner() {
  int ws = (winner == 1) ? p1_score : p2_score;
  int ls = (winner == 1) ? p2_score : p1_score;
  printf("Поздравляем игрока %d с пободей! Финальный счёт - %d : %d\n", winner,
         ws, ls);
}

