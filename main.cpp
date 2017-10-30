#include <iostream>
#include <vector>
#include "input.cpp"

using namespace std;

const int DIRECTION_LEFT = 1;
const int DIRECTION_RIGHT = 2;
const int DIRECTION_UP = 3;
const int DIRECTION_DOWN = 4;

int SCORE = 0;

// current direction of the snake
int DIRECTION = DIRECTION_DOWN;

// size of the map
const int MAP_WIDTH = 40;
const int MAP_HEIGHT = 15;

// character of the snake body
const char SNAKE_BODY = '*';

// array of the available characters for food
const char FOOD_SYMBOLS[] = {'%', '$', '&', '@', '+'};
// current coordinates of food
int FOOD_X = 0;
int FOOD_Y = 0;

// current food's index 
int FOOD_SYMBOL_NUM = 0;

// cell of the snake's body 
struct snake_body {
    int x;
    int y;
};

// array of cells of the snake's body
vector<snake_body> snake;

// creates the primary snake
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

// checks if the snake has eaten itself
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

// checks if the snake's body overlaps with coordinates
bool is_snake_body(int x, int y) {
    for (int k(0); k < snake.size(); k++) {
        if (snake[k].y == y && snake[k].x == x) {
            return true;
        }
    }

    return false;
}

// checks if the coordinates are border of the map
bool is_map_border(int x, int y) {
    return y == 0 || x == 0 || x == MAP_WIDTH || y == MAP_HEIGHT;
}

// checks if the coordinates match the coordinates of food
bool is_food(int x, int y) {
    return FOOD_X == x && FOOD_Y == y;
}

// checks if the snake's body overlaps with food
bool snake_ate_food() {
    for (int i(0); i < snake.size(); i++) {
        if (snake[i].x == FOOD_X && snake[i].y == FOOD_Y) {
            return true;
        }
    }

    return false;
}

// checks if the snake is out of map
bool is_out_of_border() {
    return snake[0].x == 0 || snake[0].y == 0 || snake[0].x == MAP_WIDTH || snake[0].y == MAP_HEIGHT;
}

// generates new coordinates for food
void generate_food_coord() {
    srand( time( 0 ) );

    FOOD_X = 3 + rand() % (MAP_WIDTH - 3);
    FOOD_Y = 3 + rand() % (MAP_HEIGHT - 3);
}

// generates a new food character
void generate_food_symbol() {
    FOOD_SYMBOL_NUM = rand() % sizeof(FOOD_SYMBOLS);
}

// prints score 
void print_score() {
    printf("\n###### SCORE: %d ######\n\n", SCORE);
}

// draws a map and snake
void draw() {
    system("clear");

    // draws the map
    for (int i(0); i <= MAP_HEIGHT; i++) {
        for (int j(0); j <= MAP_WIDTH; j++) {

            if (is_map_border(j, i)) {
                cout << '#' << flush;

            } else if (is_snake_body(j, i)) {
                cout << SNAKE_BODY << flush;

            } else if (is_food(j, i)) {
                cout << FOOD_SYMBOLS[FOOD_SYMBOL_NUM] << flush;

            } else {
                cout << ' ' << flush;
            }
        }

        cout << "\n";
    }

    print_score();
}

// moves the snake and its body
void move_snake() {
    int _x = snake[0].x;
    int _y = snake[0].y;
    int last_x, last_y;

    // changing the head coordinates
    switch (DIRECTION) {
        case DIRECTION_DOWN: snake[0].y++; break;
        case DIRECTION_LEFT: snake[0].x--; break;
        case DIRECTION_UP:   snake[0].y--; break;
        case DIRECTION_RIGHT:snake[0].x++; break;
        default:break;
    }

    // each next cell gets coordinates of the previous cell
    for (int i(1); i < snake.size(); i++) {
        last_x = snake[i].x;
        last_y = snake[i].y;

        snake[i].x = _x;
        snake[i].y = _y;

        _x = last_x;
        _y = last_y;
    }
}

// will be called on 'Esc'
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

        // there is some data in the thread
        if (select(1, &rfds, NULL, NULL, &timeout) > 0 && FD_ISSET(STDIN_FILENO, &rfds)) {
            switch (getchar()) {

                case 100:
                case 68: if (DIRECTION != DIRECTION_LEFT)  DIRECTION = DIRECTION_RIGHT; break;

                case 83:
                case 115: if (DIRECTION != DIRECTION_UP)    DIRECTION = DIRECTION_DOWN; break;

                case 65:
                case 97: if (DIRECTION != DIRECTION_RIGHT) DIRECTION = DIRECTION_LEFT; break;

                case 119:
                case 87: if (DIRECTION != DIRECTION_DOWN)  DIRECTION = DIRECTION_UP; break;

                case 27: exit(); return 0;
                default:break;
            }
        }

        move_snake();

        // if the snake's head is overlapping with food's coordinates then add
        // a cell to the end of the snake and generate food
        if (snake_ate_food()) {
            generate_food_coord();
            generate_food_symbol();

            snake_body snake_peace;
            snake_peace.y = snake[snake.size()-1].y;
            snake_peace.x = snake[snake.size()-1].x;

            snake.push_back(snake_peace);

            SCORE += 7;
        }

        // the snake has eaten itself or is out of map
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
