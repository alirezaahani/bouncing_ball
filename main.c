#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include <unistd.h>

#define WIDTH 64
#define HEIGHT 64

#define RADIUS 5 // m

typedef struct {
  float x;
  float y;
} vec2d;

#define FPS 60
#define GRAVITY 50.0f // n
#define DT (1.0f / FPS) 
#define DAMPER 0.5f // m

vec2d c;
vec2d vel;
static char screen[HEIGHT * WIDTH];

void reset(void) {
  c.x = RADIUS + 10;
  c.y = RADIUS;

  vel.x = 20.0f;
  vel.y = GRAVITY;
}

void fill(void) { memset(screen, ' ', HEIGHT * WIDTH); }

void back(void) {
  printf("\x1b[%dD", WIDTH);
  printf("\x1b[%dA", HEIGHT / 2);
}

void circle(vec2d c) {
  vec2d b = {c.x - RADIUS, c.y - RADIUS};
  vec2d e = {c.x + RADIUS, c.y + RADIUS};

  for (int i = b.y; i <= e.y; ++i) {
    for (int j = b.x; j <= e.x; ++j) {
      vec2d p = {j + 0.5f, i + 0.5f};
      vec2d d = {c.x - p.x, c.y - p.y};

      if (d.x * d.x + d.y * d.y <= RADIUS * RADIUS) {
        if (0 <= j && j < WIDTH && 0 <= i && i < HEIGHT) {
          screen[i * HEIGHT + j] = '#';
        }
      }
    }
  }
}

void show(void) {
  static char row[WIDTH];

  for (int i = 0; i < HEIGHT / 2; i++) {
    for (int j = 0; j < WIDTH; j++) {
      char t = screen[(2 * i + 0) * WIDTH + j];
      char b = screen[(2 * i + 1) * WIDTH + j];
      char c = ' ';

      if (t == ' ' && b == ' ')
        c = ' ';
      else if (t == '#' && b == ' ')
        c = '^';
      else if (t == ' ' && b == '#')
        c = '_';
      else if (t == '#' && b == '#')
        c = '#';

      row[j] = c;
    }

    fwrite(row, WIDTH, 1, stdout);
    putchar('\n');
  }
}


int main(void) {
  reset();

  while (true) {
    c.y += (vel.y * DT);
    c.x += (vel.x * DT);

    vel.y += DT * GRAVITY;

    if (c.y > HEIGHT - RADIUS) {
      c.y = HEIGHT - RADIUS;
      vel.y *= -DAMPER;
    }

    if (c.x > WIDTH + RADIUS + RADIUS) {
      // break;
      reset();
    }

    fill();
    circle(c);
    show();
    back();

    usleep(1000 * 1000 / FPS);
  }

  return 0;
}
