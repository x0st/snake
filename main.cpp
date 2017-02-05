#include <iostream>
#include "input.h"
#include <vector>

using namespace std;

const int DIRECTION_LEFT = 1;
const int DIRECTION_RIGHT = 2;
const int DIRECTION_UP = 3;
const int DIRECTION_DOWN = 4;

// текущее направление змейки
int DIRECTION = DIRECTION_DOWN;

// размер карты
const int MAP_WIDTH = 40;
const int MAP_HEIGHT = 15;

const char SNAKE_BODY = '*';

// одна клетка змейки
struct snake_body {
    int x;
    int y;
};

// массив частей змейки
vector<snake_body> snake;

/// Добавляет в массив частей несколько частей.
void init_snake() {
    for (int i(0); i < 5; i++) {
        snake_body body;
        body.x = 5;
        body.y = 3 + i;

        snake.push_back(body);
    }
}

/// Проверяет, есть ли по указанным координатам часть тела змейки.
bool is_snake_body(int x, int y) {
    for (int k(0); k < snake.size(); k++) {
        if (snake[k].y == y && snake[k].x == x) {
            return true;
        }
    }

    return false;
}

/// Рисует карту со змейкой.
void draw() {
    system("clear");

    for (int i(0); i <= MAP_HEIGHT; i++) {
        for (int j(0); j <= MAP_WIDTH; j++) {
            if (i == 0 || j == 0 || j == MAP_WIDTH || i == MAP_HEIGHT) {
                cout << '#' << flush;
            } else if (is_snake_body(j, i)) {
                cout << SNAKE_BODY << flush;
            } else {
                cout << ' ' << flush;
            }
        }

        cout << "\n";
    }
}

/// Передвигает змейку и все части её тела.
void move_snake() {
    int _x = snake[0].x;
    int _y = snake[0].y;
    int last_x, last_y;

    switch (DIRECTION) {
        case DIRECTION_DOWN: snake[0].y++; break;
        case DIRECTION_LEFT: snake[0].x--; break;
        case DIRECTION_UP:   snake[0].y--; break;
        case DIRECTION_RIGHT:snake[0].x++; break;
        default:break;
    }

    for (int i(1); i < snake.size(); i++) {
        last_x = snake[i].x;
        last_y = snake[i].y;

        snake[i].x = _x;
        snake[i].y = _y;

        _x = last_x;
        _y = last_y;
    }
}

/// Фк-ция выполнится после нажатия Esc.
void exit() {
    input_off();

    printf("\n\n###### THANK YOU FOR GAME ######\n\n");
}

int main () {
    fd_set rfds;
    timeval timeout;

    timeout.tv_sec = 0;
    timeout.tv_usec = 0;

    input_on();
    init_snake();

    while (true) {

        FD_ZERO(&rfds);
        FD_SET(STDIN_FILENO, &rfds);

        if (select(1, &rfds, NULL, NULL, &timeout) > 0 && FD_ISSET(STDIN_FILENO, &rfds)) {
            switch (getchar()) {

                    // задать направление вправо
                case 100:
                case 68: if (DIRECTION != DIRECTION_LEFT)  DIRECTION = DIRECTION_RIGHT; break;

                    // задать направление вниз
                case 83:
                case 115: if (DIRECTION != DIRECTION_UP)    DIRECTION = DIRECTION_DOWN; break;

                    // задать направление влево
                case 65:
                case 97: if (DIRECTION != DIRECTION_RIGHT) DIRECTION = DIRECTION_LEFT; break;

                    // задать направление вверх
                case 119:
                case 87: if (DIRECTION != DIRECTION_DOWN)  DIRECTION = DIRECTION_UP; break;

                    // выход из программы
                case 27: exit(); return 0;
                default:break;
            }
        }

        move_snake();
        draw();

        usleep(100000);
    }

    input_off();
    return 0;
}