#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"

#define TILE_SCALE 2.0F
#define TILE_SIZE (16.0F * TILE_SCALE)
#define WIDTH (TILE_SIZE * 20.0F)
#define HEIGHT (TILE_SIZE * 20.0F)
#define VECTOR_ADD(a,b) ((a).x + (b).x, (a).y + (b).y)

struct Window {
    int width, height;
    char *name;
};

struct Window window = {
    .width = WIDTH,
    .height = HEIGHT,
    .name = "Snake Game",
};

struct SnakeSegment {
    Vector2 position;
    char *texturePath;
    Texture texture;
};

struct Snake {
    unsigned int movementDirection: 2;
    struct SnakeSegment segments[];
};

struct Snake *InitSnake(char *texturePath, Texture texture);

void drawSnake(const struct Snake *snake);

void moveSnake(struct Snake *snake);

int main(void) {
    InitWindow(window.width, window.height, window.name);
    SetTargetFPS(5);

    char *texturePath = "assets/snake-body.png";
    const Texture texture = LoadTexture(texturePath);

    struct Snake *snake = InitSnake(texturePath, texture);

    while (!WindowShouldClose()) {
        moveSnake(snake);
        BeginDrawing();

        ClearBackground(BLACK);
        DrawRectangle(window.width / 2 - 25, window.height / 2 - 25, 50, 50, DARKGREEN);
        drawSnake(snake);
        DrawFPS(0, 0);

        EndDrawing();
    }

    free(snake);
    CloseWindow();
}

struct Snake *InitSnake(char *texturePath, const Texture texture) {
    struct Snake *snake = malloc(sizeof(struct Snake) + (sizeof(struct SnakeSegment) * 10));
    for (int i = 0; i < 10; i++) {
        struct SnakeSegment newSnakeSegment = {
            .position = (Vector2){i * TILE_SIZE, 0},
            .texturePath = texturePath,
            .texture = texture,
        };
        snake->segments[i] = newSnakeSegment;
    }
    return snake;
}

void drawSnake(const struct Snake *snake) {
    if (snake == NULL) {
        return;
    }
    for (int i = 0; i < 10; i++) {
        DrawRectangle(snake->segments[i].position.x, snake->segments[i].position.y, TILE_SIZE, TILE_SIZE, DARKGREEN);
    }
}

void moveSnake(struct Snake *snake) {
    if (snake == NULL) {
        return;
    }
    for (int i = 9; i > 0; i--) {
        snake->segments[i].position = snake->segments[i - 1].position;
    }
    if (IsKeyDown(KEY_W)) {
        snake->movementDirection = 0;
    } else if (IsKeyDown(KEY_S)) {
        snake->movementDirection = 2;
    } else if (IsKeyDown(KEY_A)) {
        snake->movementDirection = 1;
    } else if (IsKeyDown(KEY_D)) {
        snake->movementDirection = 3;
    }
    switch (snake->movementDirection) {
        case 0:
            snake->segments[0].position.y -= TILE_SIZE;
            break;
        case 1:
            snake->segments[0].position.x -= TILE_SIZE;
            break;
        case 2:
            snake->segments[0].position.y += TILE_SIZE;
            break;
        case 3:
            snake->segments[0].position.x += TILE_SIZE;
            break;
        default:
            return;
    }
}
