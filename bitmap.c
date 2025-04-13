#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define WIDTH 128
#define HEIGHT 64
#define ROWS (HEIGHT / 8)
#define BITMAP_SIZE (WIDTH * ROWS)


void embed_bitmap(const unsigned char *src, int src_width, int src_height,
                  int dst_x, int dst_y);


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

unsigned int grid_coords[] = {
    4, 18, 32, 46
};



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

int main() {
    int i;
    static int pos = 0;

    int x=0,y=0;
	int keyin=0;
	int movX=3, dirX=1;
	int movY=3, dirY=1;

    x=64;
    y=32;


    while (1) {
    // grid
    embed_bitmap(grid_bmp, 57, 57, 3, 3);

    // rock
    embed_bitmap(rock_bmp, 13, 13, 4, 4);


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
    
        if (x<0)          dirX=1;  //x=0;         // check X boundary Min;
		if (x+5>WIDTH)    dirX=-1; //x=WIDTH-5-1;     // check X boundary Max
		if (y<0)          dirY=1;  //y=0;         // check Y boundary Min
		if (y+7>HEIGHT)   dirY=-1; //y=HEIGHT-7-1;	// check Y boundary Max;

        //clear_bitmap();
    }

    return 0;
}


void embed_bitmap(const unsigned char *src, int src_width, int src_height,
                  int dst_x, int dst_y) {
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

