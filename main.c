#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define WIDTH      800
#define HEIGHT     800
#define CELL_SIZE  20
#define TOTAL_COLS (WIDTH / CELL_SIZE)
#define TOTAL_ROWS (HEIGHT / CELL_SIZE)

typedef enum
{
    STATE_EMPTY = 0,
    STATE_SAND  = 1,
} State;

typedef struct
{
    int x;
    int y;
    State state;
} Grid;

void render_grid(
    SDL_Renderer *ren, State grid[WIDTH / CELL_SIZE][HEIGHT / CELL_SIZE]
)
{
    for (int col = 0; col < WIDTH / CELL_SIZE; col++)
    {
        for (int row = 0; row < HEIGHT / CELL_SIZE; row++)
        {
            if (grid[col][row] == STATE_SAND)
                SDL_SetRenderDrawColor(ren, 194, 178, 128, 255);
            else
                SDL_SetRenderDrawColor(ren, 50, 50, 50, 255);

            SDL_Rect rect = {
                .x = col * CELL_SIZE,
                .y = row * CELL_SIZE,
                .w = CELL_SIZE,
                .h = CELL_SIZE
            };
            SDL_RenderFillRect(ren, &rect);
        }
    }
}

void update_grid(State grid[WIDTH / CELL_SIZE][HEIGHT / CELL_SIZE])
{
    for (int col = 0; col < TOTAL_COLS; col++)
    {
        for (int row = 0; row < TOTAL_ROWS; row++)
        {
            State state = grid[col][row];
            if (state == STATE_SAND && row != TOTAL_ROWS - 1)
            {
                if (grid[col][row + 1] == STATE_SAND)
                {
                    if (grid[col + 1][row + 1] == STATE_EMPTY)
                    {
                        grid[col][row]         = STATE_EMPTY;
                        grid[col + 1][row + 1] = STATE_SAND;
                        row++;
                        col++;
                    }
                    else if (grid[col - 1][row + 1] == STATE_EMPTY)
                    {
                        grid[col][row]         = STATE_EMPTY;
                        grid[col - 1][row + 1] = STATE_SAND;
                        row++;
                    }
                }
                else
                {
                    grid[col][row]     = STATE_EMPTY;
                    grid[col][row + 1] = STATE_SAND;
                    row++;
                }
            }
        }
    }
}

int main()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(
            stderr, "ERROR: Failed to initialize SDL: %s\n", SDL_GetError()
        );
        exit(1);
    }

    SDL_Window *win =
        SDL_CreateWindow("Falling sand", 0, 0, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);

    if (win == NULL)
    {
        fprintf(stderr, "ERROR: Failed to create window: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    if (ren == NULL)
    {
        fprintf(
            stderr, "ERROR: Failed to create renderer: %s\n", SDL_GetError()
        );
        exit(1);
    }

    bool quit = false;
    SDL_Event event;

    State grid[WIDTH / CELL_SIZE][HEIGHT / CELL_SIZE] = {STATE_EMPTY};

    while (!quit)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_MOUSEMOTION:
                    if (event.button.button == SDL_BUTTON_LEFT)
                    {
                        int col        = event.button.x / CELL_SIZE;
                        int row        = event.button.y / CELL_SIZE;
                        grid[col][row] = STATE_SAND;
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT)
                    {
                        int col        = event.button.x / CELL_SIZE;
                        int row        = event.button.y / CELL_SIZE;
                        grid[col][row] = STATE_SAND;
                    }
                    break;
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == 'u')
                        update_grid(grid);
                    if (event.key.keysym.sym == 'c')
                        for (int col = 0; col < TOTAL_COLS; col++)
                            for (int row = 0; row < TOTAL_ROWS; row++)
                                grid[col][row] = STATE_EMPTY;

                    break;
            }
        }

        update_grid(grid);

        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        SDL_RenderClear(ren);

        render_grid(ren, grid);

        SDL_RenderPresent(ren);
        SDL_Delay(20);
    }

    SDL_DestroyWindow(win);
    SDL_DestroyRenderer(ren);
    SDL_Quit();
}
