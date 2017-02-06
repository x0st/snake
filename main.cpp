#include <iostream>
#include "input.h"
#include <vector>

using namespace std;

const int DIRECTION_LEFT = 1;
const int DIRECTION_RIGHT = 2;
const int DIRECTION_UP = 3;
const int DIRECTION_DOWN = 4;

int SCORE = 0;

// текущее направление змейки
int DIRECTION = DIRECTION_DOWN;

// размер карты
const int MAP_WIDTH = 40;
const int MAP_HEIGHT = 15;

// символ части тела змейки
const char SNAKE_BODY = '*';

// массив доступных символов для еды
const char FOOD_SYMBOLS[] = {'%', '$', '&', '@', '+'};
// текущие координаты еды
int FOOD_X = 0;
int FOOD_Y = 0;

// номер текущего символа еды
int FOOD_SYMBOL_NUM = 0;

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
        body.x = -10;
        body.y = -10;

        snake.push_back(body);
    }

    snake[0].x = 5;
    snake[0].y = 3;
}

/// Проверяет, съела ли змейка сама себя.
bool snake_eats_itself() {
    int head_x = snake[0].x;
    int head_y = snake[0].y;

    for (int i(1); i < snake.size(); i++) {
        if (snake[i].x == head_x && snake[i].y == head_y) {
            return true;
        }
    }

    return false;
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

/// Проверяет, являются ли указанные координаты стенками карты.
bool is_map_border(int x, int y) {
    return y == 0 || x == 0 || x == MAP_WIDTH || y == MAP_HEIGHT;
}

/// Проверяет, находится ли по указанным координатам еда.
bool is_food(int x, int y) {
    return FOOD_X == x && FOOD_Y == y;
}

/// Проверяет, находится ли часть тела змейки на еде.
bool snake_ate_food() {
    for (int i(0); i < snake.size(); i++) {
        if (snake[i].x == FOOD_X && snake[i].y == FOOD_Y) {
            return true;
        }
    }

    return false;
}

/// Проверяет, врезалась ли змейка в стену.
bool is_out_of_border() {
    return snake[0].x == 0 || snake[0].y == 0 || snake[0].x == MAP_WIDTH || snake[0].y == MAP_HEIGHT;
}

/// Генерирует новые координаты для еды.
void generate_food_coord() {
    srand( time( 0 ) );

    FOOD_X = 3 + rand() % (MAP_WIDTH - 3);
    FOOD_Y = 3 + rand() % (MAP_HEIGHT - 3);
}

/// Генерирует новый символ еды.
void generate_food_symbol() {
    FOOD_SYMBOL_NUM = rand() % sizeof(FOOD_SYMBOLS);
}

/// Печатает кол-во очков.
void print_score() {
    printf("\n###### SCORE: %d ######\n\n", SCORE);
}

/// Рисует карту со змейкой.
void draw() {
    system("clear");

    // рисуем карту с блэкджеком и шлюхами
    for (int i(0); i <= MAP_HEIGHT; i++) {
        for (int j(0); j <= MAP_WIDTH; j++) {

            if (is_map_border(j, i)) { // проверка на границы карты
                cout << '#' << flush;

            } else if (is_snake_body(j, i)) { // проверка на тело змейки
                cout << SNAKE_BODY << flush;

            } else if (is_food(j, i)) { // проверка на еду
                cout << FOOD_SYMBOLS[FOOD_SYMBOL_NUM] << flush;

            } else {
                cout << ' ' << flush;
            }
        }

        cout << "\n";
    }

    print_score();
}

/// Передвигает змейку и все части её тела.
void move_snake() {
    int _x = snake[0].x;
    int _y = snake[0].y;
    int last_x, last_y;

    // меняем координаты головы
    switch (DIRECTION) {
        case DIRECTION_DOWN: snake[0].y++; break;
        case DIRECTION_LEFT: snake[0].x--; break;
        case DIRECTION_UP:   snake[0].y--; break;
        case DIRECTION_RIGHT:snake[0].x++; break;
        default:break;
    }

    // каждому следующему элементу тела змейки
    // присваиваем координаты предыдущего элемента
    for (int i(1); i < snake.size(); i++) {
        last_x = snake[i].x;
        last_y = snake[i].y;

        snake[i].x = _x;
        snake[i].y = _y;

        _x = last_x;
        _y = last_y;
    }

    // если голова змейки находится на тех же координатах что и еда
    // тогда добавляем часть тела в конец змейки
    // и генерируем новую еду
    if (snake_ate_food()) {
        generate_food_coord();
        generate_food_symbol();

        snake_body snake_peace;
        snake_peace.y = snake[snake.size()-1].y;
        snake_peace.x = snake[snake.size()-1].x;

        snake.push_back(snake_peace);

        SCORE += 7;
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

    generate_food_coord();
    generate_food_symbol();
    init_snake();

    while (true) {

        FD_ZERO(&rfds);
        FD_SET(STDIN_FILENO, &rfds);

        // если в потоке появились данные
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

        if (is_out_of_border() || snake_eats_itself()) {
            exit();
            break;
        }

        draw();

        usleep(100000);
    }

    input_off();
    return 0;
}