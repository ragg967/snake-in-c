#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"

#define TILE_SCALE 2
#define TILE_SIZE (16 * TILE_SCALE)
#define WIDTH (TILE_SIZE * 20)
#define HEIGHT (TILE_SIZE * 20)

struct Window {
    int width, height;
    char *name;
    bool gameRunning;
};

struct Window window = {
    .width = WIDTH,
    .height = HEIGHT,
    .name = "Snake Game",
    .gameRunning = true,
};

struct SnakeSegment {
    Vector2 position;
    const char *texturePath;
    Texture texture;
};

struct Snake {
    unsigned int movementDirection: 2;
    unsigned int segmentCount;
    struct SnakeSegment segments[];
};

struct Snake *InitSnake(const char *texturePath, Texture texture);

void drawSnake(const struct Snake *snake);

void moveSnake(struct Snake *snake);

void checkWallCollision(const struct Snake *snake);

int main(void) {
    InitWindow(window.width, window.height, window.name);
    SetTargetFPS(5);

    const char *texturePath = "../assets/snake-body.png";
    const Texture texture = LoadTexture(texturePath);

    if (!IsTextureValid(texture)) {
        printf("\033[31mERROR: Texture could not be loaded\033[0m\n");
        return 1;
    }

    struct Snake *snake = InitSnake(texturePath, texture);
    if (snake == NULL) {
        printf("\033[31mERROR: Snake could not be loaded\033[0m\n");
        return 1;
    }

    while (!WindowShouldClose() && window.gameRunning) {
        moveSnake(snake);
        checkWallCollision(snake);

        BeginDrawing();

        ClearBackground(BLACK);
        drawSnake(snake);
        DrawFPS(0, 0);

        EndDrawing();
    }

    free(snake);
    CloseWindow();
}

struct Snake *InitSnake(const char *texturePath, const Texture texture) {
    struct Snake *snake = malloc(sizeof(struct Snake) + (sizeof(struct SnakeSegment) * (WIDTH * HEIGHT)));
    for (int i = 0; i < (WIDTH * HEIGHT); i++) {
        const struct SnakeSegment newSnakeSegment = {
            .position = (Vector2){((float)i * TILE_SIZE) + (TILE_SIZE * 7), (TILE_SIZE * 7)},
            .texturePath = texturePath,
            .texture = texture,
        };
        snake->segments[i] = newSnakeSegment;
    }
    snake->segmentCount = 4;
    return snake;
}

void drawSnake(const struct Snake *snake) {
    if (snake == NULL) {
        return;
    }
    for (int i = 1; i < snake->segmentCount; i++) {
        DrawTextureRec(snake->segments[i].texture, (Rectangle){snake->segments[i].position.x, snake->segments[i].position.y, TILE_SIZE, TILE_SIZE}, snake->segments[i].position, DARKGREEN);
    }
    DrawTextureRec(snake->segments[0].texture, (Rectangle){snake->segments[0].position.x, snake->segments[0].position.y, TILE_SIZE, TILE_SIZE}, snake->segments[0].position, WHITE);
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
            break;
    }
}

void checkWallCollision(const struct Snake *snake) {
    if (snake == NULL) {
        return;
    }
    if (snake->segments[0].position.x == 0) {
        window.gameRunning = false;
    }
    if (snake->segments[0].position.x + TILE_SIZE == WIDTH) {
        window.gameRunning = false;
    }
    if (snake->segments[0].position.y == 0) {
        window.gameRunning = false;
    }
    if (snake->segments[0].position.y + TILE_SIZE == HEIGHT) {
        window.gameRunning = false;
    }
}
