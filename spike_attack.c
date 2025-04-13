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

    int x=0,y=0;
	int keyin=0;
	int movX=2, dirX=1;
	int movY=2, dirY=1;

    x=32;
    y=32;

    init_grid();

    while (1) {
        // grid
        embed_grid_elements();

        // embed the character
        embed_bitmap(amogus_bmp, 5, 7, x, y);


        // Example: draw a diagonal line
        /*for (i = 0; i < WIDTH && i < HEIGHT; i++) {
            set_pixel(i, i, 1);
            set_pixel(i+WIDTH/2, i, 1);
        }*/

        
        draw_bitmap();
        usleep(200000);  // Sleep 200ms

        // Animate a pixel moving across the screen
/*        for (y = 0; y < HEIGHT; y++)
            set_pixel((pos - 1 + WIDTH) % WIDTH, y, 0);  // Clear previous
        for (y = 0; y < HEIGHT; y++)
            set_pixel(pos, y, 1);  // Set new column

        pos = (pos + 1) % WIDTH;
*/
        x=x + (dirX * movX); // increment/decrement X
		y=y + (dirY * movY); // increment/decrement Y
    

        // we check for the boundaries of the grid, not the screen
        // horizontal check
        if (x<=TOPLEFT) {
            dirX=1;                    // check X boundary Min;
            x = TOPLEFT + 1;
        }
        else if (x+5>BOTRIGHT){
            dirX=-1;                   // check X boundary Max
            x = BOTRIGHT - 5;
        }
		
        // vertical check
        if (y<=TOPLEFT) {
            dirY=1;                    // check Y boundary Min
            y = TOPLEFT + 1;
        }
		else if (y+7>BOTRIGHT) {
            dirY=-1;	               // check Y boundary Max;
            y=BOTRIGHT - 7;
        }

        //clear_bitmap();
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

    for (row = 0; row < GRID_HEIGHT; row++) {
        for (col = 0; col < GRID_WIDTH; col++) {
            grid[row][col] = EMPTY;
        }
    }

    grid[1][2] = ROCK;
    grid[2][3] = VOID;
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
