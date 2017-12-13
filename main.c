// Author: Cyle Riggs (@beardedfoo)
// This program for the Sega Genesis/Megadrive is designed for the
// creation and editing of tile bitmaps on the Sega Megadrive. One
// notable feature of this application is that it runs directly on
// a Sega Megadrive.
#include <genesis.h>

// Disable the use of DMA for tile loading
#define LOAD_TILE_WITH_DMA FALSE

// Load one tile at a time into the VDP
#define LOAD_TILE_COUNT 1

// Do not allow the cursor to extend beyond the visible plane
#define X_MIN 2
#define X_MAX 17
#define Y_MIN 2
#define Y_MAX 17
#define HEX_DUMP_X 2
#define HEX_DUMP_Y 20

// Ensure only valid colors are painted
#define MAX_COLOR 0xF
#define MIN_COLOR 0x0

// Define TILE_A as a solid block of color from palette entry 1
#define VRAM_POS_TILE_FILL_BASE 0
const u32 TILESET[16][8] = {
  {0x00000000, 0x00000000, 0x00000000, 0x00000000,
   0x00000000, 0x00000000, 0x00000000, 0x00000000},
  {0x11111111, 0x11111111, 0x11111111, 0x11111111,
   0x11111111, 0x11111111, 0x11111111, 0x11111111},
  {0x22222222, 0x22222222, 0x22222222, 0x22222222,
   0x22222222, 0x22222222, 0x22222222, 0x22222222},
  {0x33333333, 0x33333333, 0x33333333, 0x33333333,
   0x33333333, 0x33333333, 0x33333333, 0x33333333},
  {0x44444444, 0x44444444, 0x44444444, 0x44444444,
   0x44444444, 0x44444444, 0x44444444, 0x44444444},
  {0x55555555, 0x55555555, 0x55555555, 0x55555555,
   0x55555555, 0x55555555, 0x55555555, 0x55555555},
  {0x66666666, 0x66666666, 0x66666666, 0x66666666,
   0x66666666, 0x66666666, 0x66666666, 0x66666666},
  {0x77777777, 0x77777777, 0x77777777, 0x77777777,
   0x77777777, 0x77777777, 0x77777777, 0x77777777},
  {0x88888888, 0x88888888, 0x88888888, 0x88888888,
   0x88888888, 0x88888888, 0x88888888, 0x88888888},
  {0x99999999, 0x99999999, 0x99999999, 0x99999999,
   0x99999999, 0x99999999, 0x99999999, 0x99999999},
  {0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA,
   0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA},
  {0xBBBBBBBB, 0xBBBBBBBB, 0xBBBBBBBB, 0xBBBBBBBB,
   0xBBBBBBBB, 0xBBBBBBBB, 0xBBBBBBBB, 0xBBBBBBBB},
  {0xCCCCCCCC, 0xCCCCCCCC, 0xCCCCCCCC, 0xCCCCCCCC,
   0xCCCCCCCC, 0xCCCCCCCC, 0xCCCCCCCC, 0xCCCCCCCC},
  {0xDDDDDDDD, 0xDDDDDDDD, 0xDDDDDDDD, 0xDDDDDDDD,
   0xDDDDDDDD, 0xDDDDDDDD, 0xDDDDDDDD, 0xDDDDDDDD},
  {0xEEEEEEEE, 0xEEEEEEEE, 0xEEEEEEEE, 0xEEEEEEEE,
   0xEEEEEEEE, 0xEEEEEEEE, 0xEEEEEEEE, 0xEEEEEEEE},
  {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
   0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}
};

// Define TILE_CURSOR as an empty square; a tranparent inner square
// and an outer square of color 2 from the palette. Since this tile
// will be placed in front of TILE_FILL the pixels with color 0 will
// be transparent and color from TILE_FILL will be visible in these
// locations. Note how the shape of the drawn image is visible in
// the arrangement of 2s and 0s in the tile data, if stretched.
#define VRAM_POS_TILE_CURSOR_BASE 16
const u32 TILESET_CURSOR[16][8] = {
  {0x00000000, 0x00000000, 0x00000000, 0x00000000,
   0x00000000, 0x00000000, 0x00000000, 0x00000000},
  {0x11111111, 0x10000001, 0x10000001, 0x10000001,
   0x10000001, 0x10000001, 0x10000001, 0x11111111},
  {0x22222222, 0x20000002, 0x20000002, 0x20000002,
   0x20000002, 0x20000002, 0x20000002, 0x22222222},
  {0x33333333, 0x30000003, 0x30000003, 0x30000003,
   0x30000003, 0x30000003, 0x30000003, 0x33333333},
  {0x44444444, 0x40000004, 0x40000004, 0x40000004,
   0x40000004, 0x40000004, 0x40000004, 0x44444444},
  {0x55555555, 0x50000005, 0x50000005, 0x50000005,
   0x50000005, 0x50000005, 0x50000005, 0x55555555},
  {0x66666666, 0x60000006, 0x60000006, 0x60000006,
   0x60000006, 0x60000006, 0x60000006, 0x66666666},
  {0x77777777, 0x70000007, 0x70000007, 0x70000007,
   0x70000007, 0x70000007, 0x70000007, 0x77777777},
  {0x88888888, 0x80000008, 0x80000008, 0x80000008,
   0x80000008, 0x80000008, 0x80000008, 0x88888888},
  {0x99999999, 0x90000009, 0x90000009, 0x90000009,
   0x90000009, 0x90000009, 0x90000009, 0x99999999},
  {0xAAAAAAAA, 0xA000000A, 0xA000000A, 0xA000000A,
   0xA000000A, 0xA000000A, 0xA000000A, 0xAAAAAAAA},
  {0xBBBBBBBB, 0xB000000B, 0xB000000B, 0xB000000B,
   0xB000000B, 0xB000000B, 0xB000000B, 0xBBBBBBBB},
  {0xCCCCCCCC, 0xC000000C, 0xC000000C, 0xC000000C,
   0xC000000C, 0xC000000C, 0xC000000C, 0xCCCCCCCC},
  {0xDDDDDDDD, 0xD000000D, 0xD000000D, 0xD000000D,
   0xD000000D, 0xD000000D, 0xD000000D, 0xDDDDDDDD},
  {0xEEEEEEEE, 0xE000000E, 0xE000000E, 0xE000000E,
   0xE000000E, 0xE000000E, 0xE000000E, 0xEEEEEEEE},
  {0xFFFFFFFF, 0xF000000F, 0xF000000F, 0xF000000F,
   0xF000000F, 0xF000000F, 0xF000000F, 0xFFFFFFFF},
};


// Define a basic color palette
const u16 palette_basic[16] = {
  0x000,
  0xEEE,
  0xE00,
  0x0e0,
  0x00e,
  0xE0E,
  0xEE0,
  0x0EE,
  0x000,
  0x888,
  0x800,
  0x080,
  0x008,
  0x808,
  0x880,
  0x088,
};

// A map to convert a byte to a hex digit
const char itoa_map[16] = {
  '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C',
  'D', 'E', 'F',
};

// Use a variable to instruct the main loop how to move
#define DIR_RIGHT 1
#define DIR_LEFT 2
#define DIR_UP 3
#define DIR_DOWN 4
u16 move_dir = NULL;

u16 brush_color = 2;
u16 do_paint = FALSE;
u16 redraw = TRUE;

void joyHandler(u16 joy, u16 changed, u16 state) {
  // Ignore input from anything except player one
  if (joy != JOY_1) {
    return;
  }

  // Move the cursor with gamepad input
  if (state & BUTTON_C) {
    if (state & BUTTON_LEFT && brush_color > MIN_COLOR) {
      brush_color--;
    } else if (state & BUTTON_RIGHT && brush_color < MAX_COLOR) {
      brush_color++;
    }
  } else {
    if (state & BUTTON_LEFT) {
      move_dir = DIR_LEFT;
    } else if (state & BUTTON_RIGHT) {
      move_dir = DIR_RIGHT;
    } else if (state & BUTTON_UP) {
      move_dir = DIR_UP;
    } else if (state & BUTTON_DOWN) {
      move_dir = DIR_DOWN;
    }
  }

  // On A button apply current brush color to current pixel
  if (state & BUTTON_A) {
    do_paint = TRUE;
  }

  redraw = TRUE;
}

// Draw the background in PLAN_A and the cursor in PLAN_B, which
// has a higher drawing priority and allows for transparency
// effects on the tiles.
#define CURSOR_PLANE PLAN_A
#define BG_PLANE PLAN_B

void load_tile_buffer() {
  // Fill PLAN_B with TILE_FILL
  for (u16 fill_x = X_MIN; fill_x <= X_MAX; fill_x++) {
    for (u16 fill_y = Y_MIN; fill_y <= Y_MAX; fill_y++) {
      VDP_setTileMapXY(BG_PLANE, VRAM_POS_TILE_FILL_BASE+1, fill_x , fill_y);
    }
  }
}

void update_hex_dump(char buffer[8][9], u8 x, u8 y) {
  // Dump a hex string for each row in the bitmap
  for (int dump_x = 0; dump_x < 8; dump_x++) {
    // Prepare a hex u32 string output
    VDP_drawText(buffer[dump_x], x, y + dump_x);
  }
}

int main()
{
  // Place the cursor on a grid of 40x28 tiles
  u16 cursor_x = 8;
  u16 cursor_y = 8;

  // Setup the video chip
  VDP_init();

  // Setup gamepad input handling
  JOY_init();
  JOY_setEventHandler(&joyHandler);

  // Load the tile data into the video chip (one at a time)
  VDP_loadTileData((const u32 *)TILESET, VRAM_POS_TILE_FILL_BASE, 16, LOAD_TILE_WITH_DMA);
  VDP_loadTileData((const u32 *)TILESET_CURSOR, VRAM_POS_TILE_CURSOR_BASE, 16, LOAD_TILE_WITH_DMA);

  load_tile_buffer();

  // Load the color palette as palette 0
  VDP_setPalette(PAL0, palette_basic);

  // Allow the cursor to be placed over text
  VDP_setTextPlan(BG_PLANE);

  // The char buffer for the bitmap, used to make hex dump
  char buffer[4][8][9] = {
    {
      {'1', '1', '1', '1', '1', '1', '1', '1', NULL},
      {'1', '1', '1', '1', '1', '1', '1', '1', NULL},
      {'1', '1', '1', '1', '1', '1', '1', '1', NULL},
      {'1', '1', '1', '1', '1', '1', '1', '1', NULL},
      {'1', '1', '1', '1', '1', '1', '1', '1', NULL},
      {'1', '1', '1', '1', '1', '1', '1', '1', NULL},
      {'1', '1', '1', '1', '1', '1', '1', '1', NULL},
      {'1', '1', '1', '1', '1', '1', '1', '1', NULL},
    },
    {
      {'1', '1', '1', '1', '1', '1', '1', '1', NULL},
      {'1', '1', '1', '1', '1', '1', '1', '1', NULL},
      {'1', '1', '1', '1', '1', '1', '1', '1', NULL},
      {'1', '1', '1', '1', '1', '1', '1', '1', NULL},
      {'1', '1', '1', '1', '1', '1', '1', '1', NULL},
      {'1', '1', '1', '1', '1', '1', '1', '1', NULL},
      {'1', '1', '1', '1', '1', '1', '1', '1', NULL},
      {'1', '1', '1', '1', '1', '1', '1', '1', NULL},
    },
    {
      {'1', '1', '1', '1', '1', '1', '1', '1', NULL},
      {'1', '1', '1', '1', '1', '1', '1', '1', NULL},
      {'1', '1', '1', '1', '1', '1', '1', '1', NULL},
      {'1', '1', '1', '1', '1', '1', '1', '1', NULL},
      {'1', '1', '1', '1', '1', '1', '1', '1', NULL},
      {'1', '1', '1', '1', '1', '1', '1', '1', NULL},
      {'1', '1', '1', '1', '1', '1', '1', '1', NULL},
      {'1', '1', '1', '1', '1', '1', '1', '1', NULL},
    },
    {
      {'1', '1', '1', '1', '1', '1', '1', '1', NULL},
      {'1', '1', '1', '1', '1', '1', '1', '1', NULL},
      {'1', '1', '1', '1', '1', '1', '1', '1', NULL},
      {'1', '1', '1', '1', '1', '1', '1', '1', NULL},
      {'1', '1', '1', '1', '1', '1', '1', '1', NULL},
      {'1', '1', '1', '1', '1', '1', '1', '1', NULL},
      {'1', '1', '1', '1', '1', '1', '1', '1', NULL},
      {'1', '1', '1', '1', '1', '1', '1', '1', NULL},
    },
  };

  // Load different tiles on screen as buttons are pressed
  while(1)
  {
    // Wait for the VDP to finish drawing the frame so that screen
    // updates work as expected.
    VDP_waitVSync();

    // Handle cursor moves triggered by the gamepad input handler
    if (move_dir != NULL) {
      // Figure out where on the canvas the user moved
      u8 canvas_x = cursor_x - X_MIN;
      u8 canvas_y = cursor_y - Y_MIN;

      // Clear the cursor tiles current positions
      VDP_setTileMapXY(CURSOR_PLANE, NULL, cursor_x, cursor_y);
      VDP_setTileMapXY(CURSOR_PLANE, NULL, HEX_DUMP_X + (canvas_x % 8), HEX_DUMP_Y + (canvas_y % 8));

      // Move the cursor position, restricting to viewable areas
      if (move_dir == DIR_RIGHT && cursor_x < X_MAX) {
        cursor_x++;
      } else if (move_dir == DIR_LEFT && cursor_x > X_MIN) {
        cursor_x--;
      } else if (move_dir == DIR_UP && cursor_y > Y_MIN) {
        cursor_y--;
      } else if (move_dir == DIR_DOWN && cursor_y < Y_MAX) {
        cursor_y++;
      }

      // Update the canvas coordinates
      canvas_x = cursor_x - X_MIN;
      canvas_y = cursor_y - Y_MIN;

      // Figure out which tile (the canvas represents 4 tiles)
      // the user moved onto
      u8 tile_index = (canvas_x / 8) | ((canvas_y / 8) << 1);

      // Update the hex dump to show the currently selected tile
      update_hex_dump(buffer[tile_index], HEX_DUMP_X, HEX_DUMP_Y);

      // Update the screen
      redraw = TRUE;

      // Reset the move direction, allowing for more input
      move_dir = NULL;
    }


    // Paint the pixel per user input
    if (do_paint) {
      // Figure out where on the canvas the user painted
      u8 canvas_x = cursor_x - X_MIN;
      u8 canvas_y = cursor_y - Y_MIN;

      // Figure out which tile (the canvas represents 4 tiles)
      // the user painted on
      u8 tile_index = (canvas_x / 8) | ((canvas_y / 8) << 1);

      // Update the strings used for hex dumps
      buffer[tile_index][canvas_y % 8][canvas_x % 8] = itoa_map[brush_color];
      VDP_setTileMapXY(BG_PLANE, brush_color, cursor_x, cursor_y);

      do_paint = FALSE;
    }

    // When needed refresh the display
    if (redraw) {
      // Figure out current position on the canvas
      u8 canvas_x = cursor_x - X_MIN;
      u8 canvas_y = cursor_y - Y_MIN;
      u8 tile_index = (canvas_x / 8) | ((canvas_y / 8) << 1);

      // Draw the cursors
      VDP_setTileMapXY(
        CURSOR_PLANE, VRAM_POS_TILE_CURSOR_BASE+brush_color,
        cursor_x, cursor_y);
      VDP_setTileMapXY(
        CURSOR_PLANE, VRAM_POS_TILE_CURSOR_BASE + 1,
        HEX_DUMP_X + (canvas_x % 8), HEX_DUMP_Y + (canvas_y % 8));

      // Draw the current tile number
      char msg[7] = {"Tile _"};
      msg[5] = itoa_map[tile_index];
      VDP_drawText(msg, HEX_DUMP_X, HEX_DUMP_Y-1);

      // Draw the hex dump
      update_hex_dump(buffer[tile_index], HEX_DUMP_X, HEX_DUMP_Y);
      redraw = FALSE;
    }
  }
}
