#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define WIDTH 128
#define HEIGHT 64
#define ROWS (HEIGHT / 8)
#define BITMAP_SIZE (WIDTH * ROWS)

#define SQUARE 13
#define TOPLEFT 3
#define BOTRIGHT (TOPLEFT + (4*SQUARE) + 4)

#define GRID_WIDTH 4
#define GRID_HEIGHT 4

//FUNCTION DECLARATIONS
void clear_screen();
void clear_bitmap();
void draw_bitmap();
void set_pixel(int x, int y, int on);
void embed_bitmap(const unsigned char *src, int src_width, int src_height, int dst_x, int dst_y);

// Game functions
void init_grid();
int is_passable(int row, int col);
int is_deadly(int row, int col);
void embed_grid_elements();
int screen_to_grid_index(int coord);
int check_rock_collision(int x, int y);
void move_player();
void sweep_void(int dir);
void reset_sweep();


int player_x = 10;
int player_y = 10;
int player_width = 5;
int player_height = 7;

int movX=2, dirX=1;
int movY=2, dirY=1;

// GAME ELEMENTS
typedef enum {
    EMPTY,   // Player can walk on it
    ROCK,    // Impassable
    VOID     // Kills player
} Cell;


Cell grid[GRID_HEIGHT][GRID_WIDTH];

// BITMAPS
unsigned char bitmap[BITMAP_SIZE];

unsigned char amogus_bmp[5] = {
	0x7D,0x1D,0x1F,0x7F,0x0E
};

unsigned char grid_bmp[57*8] = {
    0xFF,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0xFF,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0xFF,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0xFF,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0xFF,
	0xFF,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0xFF,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0xFF,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0xFF,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0xFF,
	0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,
	0xFF,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0xFF,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0xFF,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0xFF,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0xFF,
	0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,
	0xFF,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0xFF,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0xFF,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0xFF,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0xFF,
	0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,
	0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01
};

unsigned char rock_bmp[26] = {
    0x00,0xE0,0x78,0xFE,0xCE,0x9A,0x92,0x92,0xD6,0x7C,0xE0,0x80,0x00,
	0x00,0x01,0x03,0x0F,0x0F,0x0D,0x0C,0x0D,0x0D,0x0F,0x06,0x03,0x00
};

unsigned char void_bmp[26] = {
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
};

unsigned int grid_coords[] = {
    4, 18, 32, 46               // represents the coords to be used when drawing rocks in a square
};

int main() {
    int i;
    static int pos = 0;

	int keyin=0;
    int countdown=40;

    // initialize the grid, rocks, and player position
    init_grid();

    while (1) {
        // grid and rocks
        embed_grid_elements();

        // embed the character
        embed_bitmap(amogus_bmp, 5, 7, player_x, player_y);

        draw_bitmap();
        usleep(200000);  // Sleep 200ms

        move_player();

        countdown--;
        if (countdown == 0) {
            sweep_void(rand() % 4);
        } else if (countdown == -10) {
            reset_sweep();
            countdown = 40;
        }

    
    }

    return 0;
}




void clear_screen() {
    printf("\x1b[2J\x1b[H");  // Clear screen & move cursor to top-left
}

void clear_bitmap() {
    int i, j;
    for (i=0; i<WIDTH; i++) 
        for (j=0; j<ROWS; j++)
            bitmap[(j*WIDTH)+i]=0;
}

void draw_bitmap() {
    int row;
    int col;
    int y_top;
    int y_bot;
    int byte_index_top;
    int byte_index_bot;
    int bit_top;
    int bit_bot;
    int top_set;
    int bot_set;
    //unsigned char pixel_char;
    const char *pixel_char;

    clear_screen();

    for (row = 0; row < HEIGHT; row += 2) {
        for (col = 0; col < WIDTH; col++) {
            y_top = row;
            y_bot = row + 1;

            byte_index_top = col + (y_top / 8) * WIDTH;
            byte_index_bot = col + (y_bot / 8) * WIDTH;

            bit_top = y_top % 8;
            bit_bot = y_bot % 8;

            top_set = (bitmap[byte_index_top] >> bit_top) & 1;
            bot_set = (bitmap[byte_index_bot] >> bit_bot) & 1;

            if (top_set && bot_set) {
                pixel_char = "█";
            } else if (top_set) {
                pixel_char = "▀";
            } else if (bot_set) {
                pixel_char = "▄";
            } else {
                pixel_char = " ";
            }

            //printf("%c", pixel_char);
            printf("%s", pixel_char);
        }
        printf("\n");
    }

    fflush(stdout);
    clear_bitmap();
}

/*
void draw_bitmap() {
    int y;
    int x;
    int byte_index;
    int bit_index;
    int pixel_set;

    clear_screen();

    for (y = 0; y < HEIGHT; y++) {
        for (x = 0; x < WIDTH; x++) {
            byte_index = x + (y / 8) * WIDTH;
            bit_index = y % 8;
            pixel_set = (bitmap[byte_index] >> bit_index) & 1;

            if (pixel_set) {
                printf("█");  // Filled pixel
            } else {
                printf(" ");  // Empty pixel
            }
        }
        printf("\n");
    }

    fflush(stdout);
}
*/

void set_pixel(int x, int y, int on) {
    int byte_index;
    int bit_index;

    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
        return;

    byte_index = x + (y / 8) * WIDTH;
    bit_index = y % 8;

    if (on)
        bitmap[byte_index] |= (1 << bit_index);
    else
        bitmap[byte_index] &= ~(1 << bit_index);
}

void embed_bitmap(const unsigned char *src, int src_width, int src_height, int dst_x, int dst_y) {
    int src_col;
    int src_byte_row;
    int dst_col;
    int dst_y_pos;
    int src_byte_index;
    int src_bit_index;
    int src_bit_value;

    for (src_col = 0; src_col < src_width; src_col++) {
        for (src_byte_row = 0; src_byte_row < (src_height + 7) / 8; src_byte_row++) {
            src_byte_index = src_col + src_byte_row * src_width;
            
            //                                                           * 8
            for (src_bit_index = 0; src_bit_index < 8 &&  (src_byte_row << 3) + src_bit_index < src_height; src_bit_index++) {
                dst_col = dst_x + src_col;
                dst_y_pos = dst_y + src_byte_row * 8 + src_bit_index;

                // Check bounds
                if (dst_col < 0 || dst_col >= WIDTH || dst_y_pos < 0 || dst_y_pos >= HEIGHT)
                    continue;

                // Read the bit from the source bitmap
                src_bit_value = (src[src_byte_index] >> src_bit_index) & 1;

                // Set the bit in the large bitmap
                set_pixel(dst_col, dst_y_pos, src_bit_value);
            }
        }
    }
}


// Game functions
void init_grid() {
    int row;
    int col;
    int i, index;
    int used_rows[4] = {0, 0, 0, 0};
    int used_cols[4] = {0, 0, 0, 0};
    int chosen_rows[3];
    int chosen_cols[3];
    int type_choice;

    // Seed RNG
    srand(1028);

    // Pick 3 distinct rows and 3 distinct columns
    i = 0, index = rand() % 13;
    while (i < 3) {
        row = rand() % GRID_HEIGHT;
        col = rand() % GRID_WIDTH;

        if (!used_rows[row] && !used_cols[col]) {
            used_rows[row] = 1;
            used_cols[col] = 1;
            chosen_rows[i] = row;
            chosen_cols[i] = col;
            i++;
        }
    }

    // Assign random ROCK to the chosen positions
    for (i = 0; i < 3; i++) {
        grid[chosen_rows[i]][chosen_cols[i]] = ROCK;
    }

    // Set all other cells to EMPTY
    for (row = 0; row < GRID_HEIGHT; row++) {
        for (col = 0; col < GRID_WIDTH; col++) {
            if (grid[row][col] == ROCK) continue;
            grid[row][col] = EMPTY;

            if (index == 0) {
                player_x = grid_coords[col];
                player_y = grid_coords[row];
            }
            index--;
        }
    }
}

int is_passable(int row, int col) {
    if (row < 0 || row >= GRID_HEIGHT || col < 0 || col >= GRID_WIDTH)
        return 0;

    return grid[row][col] == EMPTY;
}

int is_deadly(int row, int col) {
    if (row < 0 || row >= GRID_HEIGHT || col < 0 || col >= GRID_WIDTH)
        return 0;

    return grid[row][col] == VOID;
}

void embed_grid_elements() {
    int row;
    int col;
    int screen_x;
    int screen_y;

    embed_bitmap(grid_bmp, 57, 57, TOPLEFT, TOPLEFT);

    for (row = 0; row < GRID_HEIGHT; row++) {
        for (col = 0; col < GRID_WIDTH; col++) {
            screen_x = grid_coords[col];
            screen_y = grid_coords[row];

            switch (grid[row][col]) {
                case ROCK:
                    embed_bitmap(rock_bmp, 13, 13, screen_x, screen_y);
                    break;

                case VOID:
                    embed_bitmap(void_bmp, 13, 13, screen_x, screen_y);
                    break;

                case EMPTY:
                default:
                    // No drawing for empty cells
                    break;
            }
        }
    }
}

int screen_to_grid_index(int coord) {
    int i;
    for (i = 0; i < 4; i++) {
        if (coord >= grid_coords[i] && coord < grid_coords[i] + 13) {
            return i;
        }
    }

    // special case for grid lines
    for (i = 1; i < 4; i++) {
        if (coord == grid_coords[i] - 1)
            return -2;
    }

    return -1;  // Outside grid
}

int check_rock_collision(int x, int y) {
    int corners_x[4];
    int corners_y[4];
    int i;
    int grid_col;
    int grid_row;

    // check if the player would collide with a rock at x,y
    // Define corner positions
    corners_x[0] = x;                        corners_y[0] = y;                         // Top-left
    corners_x[1] = x + player_width - 1;     corners_y[1] = y;                         // Top-right
    corners_x[2] = x;                        corners_y[2] = y + player_height - 1;     // Bottom-left
    corners_x[3] = x + player_width - 1;     corners_y[3] = y + player_height - 1;     // Bottom-right

    for (i = 0; i < 4; i++) {   // coordinate selection
        grid_col = screen_to_grid_index(corners_x[i]);
        grid_row = screen_to_grid_index(corners_y[i]);

        if (grid_col < 0 || grid_row < 0) continue;

        if (grid[grid_row][grid_col] == ROCK) {
            return 1;
        }
    }
    return 0;   
}

void move_player() {
    int new_x, new_y;

    int i, j;
    int grid_col;
    int grid_row;
    int flag = 0;

    new_x=player_x + (dirX * movX); // increment/decrement X
    new_y=player_y + (dirY * movY); // increment/decrement Y

    // printf("new_x: %d, new_y: %d\nplayer_x: %d, player_y: %d\n", new_x, new_y, player_x, player_y);

    // we check for the boundaries of the grid, not the screen
    // horizontal check
    if (new_x<=TOPLEFT+1) {
        dirX=1;                    // check X boundary Min;
        new_x = TOPLEFT + 1;
    }
    else if (new_x+5>=BOTRIGHT){
        dirX=-1;                   // check X boundary Max
        new_x = BOTRIGHT - 5;
    }
    
    // vertical check
    if (new_y<=TOPLEFT+1) {
        dirY=1;                    // check Y boundary Min
        new_y = TOPLEFT + 1;
    }
    else if (new_y+7>=BOTRIGHT) {
        dirY=-1;	               // check Y boundary Max;
        new_y=BOTRIGHT - 7;
    }


    if (check_rock_collision(new_x, new_y)) {
        // if rock collision
        if (check_rock_collision(new_x, player_y)) {
            new_x = player_x;
        } else if (check_rock_collision(player_x, new_y)) {
            new_y = player_y;
        } else {
            new_x = player_x;
            new_y = player_y;
        }
    }

    player_x = new_x;
    player_y = new_y;
}

void sweep_void(int dir) {
    int row, col;
    int r, c;

    // dir
    // 0 left to right
    // 1 right to left
    // 2 top to bottom
    // 3 bottom to top

    switch (dir) {
        case 0:
            for (row = 0; row < GRID_HEIGHT; row++) {
                for (col = 0; col < GRID_WIDTH; col++) {
                    if (grid[row][col] == ROCK)
                        break;
                    if (grid[row][col] == EMPTY)
                        grid[row][col] = VOID;
                }
            }
            break;

        case 1:
            for (row = 0; row < GRID_HEIGHT; row++) {
                for (col = GRID_WIDTH - 1; col >= 0; col--) {
                    if (grid[row][col] == ROCK)
                        break;
                    if (grid[row][col] == EMPTY)
                        grid[row][col] = VOID;
                }
            }
            break;

        case 2:
            for (col = 0; col < GRID_WIDTH; col++) {
                for (row = 0; row < GRID_HEIGHT; row++) {
                    if (grid[row][col] == ROCK)
                        break;
                    if (grid[row][col] == EMPTY)
                        grid[row][col] = VOID;
                }
            }
            break;

        case 3:
            for (col = 0; col < GRID_WIDTH; col++) {
                for (row = GRID_HEIGHT - 1; row >= 0; row--) {
                    if (grid[row][col] == ROCK)
                        break;
                    if (grid[row][col] == EMPTY)
                        grid[row][col] = VOID;
                }
            }
            break;
    }
}

void reset_sweep() {
    int row, col;

    // Set all cells to EMPTY
    for (row = 0; row < GRID_HEIGHT; row++) {
        for (col = 0; col < GRID_WIDTH; col++) {
            if (grid[row][col] == ROCK) continue;
            grid[row][col] = EMPTY;
        }
    }
}