#include "LIB/neslib.h"
#include "LIB/nesdoug.h" 
#include "level1.h"
#include "level2.h"

#define BLACK 0x0f
#define DK_GY 0x00
#define LT_GY 0x10
#define WHITE 0x30
#define LEFT_BOUNDARY 0
#define RIGHT_BOUNDARY 240

#pragma bss-name(push, "ZEROPAGE")

const unsigned char palette[]=
{
BLACK, DK_GY, LT_GY, WHITE,
BLACK,0x18,0x28,0x38,
BLACK,0x05,0x16,0x27,
BLACK,0x0B,0x1A,0x29
}; 

const unsigned char player_sprite_idle[] = 
{
	0, 0, 0x04, 1,
	0, 8, 0x14, 1,
	8, 0, 0x05, 1,
	8, 8, 0x15, 1,
	128
};

const unsigned char player_sprite_walk[] = 
{
	0, 0, 0x02, 1,
	0, 8, 0x12, 1,
	8, 0, 0x03, 1,
	8, 8, 0x13, 1,
	128
};

struct sprite_data
{
	unsigned char x;
	unsigned char y;
	unsigned char height;
	unsigned char width;
};

struct sprite_data player_data = {16, 191, 16, 16};

unsigned char pad1;
unsigned char moving;
unsigned char frame_counter = 0;
unsigned char anim_state = 0;
unsigned char current_level = 1;

void load_level(unsigned char level)
{
    ppu_off();
    if (level == 1) {
        vram_adr(NAMETABLE_A);
        vram_write(level1, 1024);
        player_data.x = RIGHT_BOUNDARY - player_data.width; // start near right edge
    } else if (level == 2) {
        vram_adr(NAMETABLE_A);
        vram_write(level2, 1024);
        player_data.x = LEFT_BOUNDARY; // start near left edge
    }
    ppu_on_all();
}

void draw(unsigned char isWalkFrame)
{
	oam_clear();
	if (isWalkFrame)
	{
		oam_meta_spr(player_data.x, player_data.y, player_sprite_walk);
	}
	else 
	{
		oam_meta_spr(player_data.x, player_data.y, player_sprite_idle);
	}
}

unsigned char move()
{
	if (pad1 & PAD_LEFT)
	{
		player_data.x--;
		return 1;
	}
	else if (pad1 & PAD_RIGHT)
	{
		player_data.x++;
		return 1;
	}
	else 
	{
		return 0;
	}

}

void main(void)
{
    ppu_off();

    pal_bg(palette);
    pal_spr(palette);

    bank_spr(1);

    // Initially load level 1
    load_level(1);
    current_level = 1;

    ppu_on_all();

    while(1)
    {
        pad1 = pad_poll(0);
        ppu_wait_nmi();

        moving = move();

        if (current_level == 1 && player_data.x > RIGHT_BOUNDARY)
		{
   			 current_level = 2;
  			  load_level(2);
		}
		else if (current_level == 2 && player_data.x <= LEFT_BOUNDARY)
		{
    		current_level = 1;
    		load_level(1);
	}

        if (moving)
        {
            frame_counter++;
            if (frame_counter >= 5)
            {
                frame_counter = 0;
                anim_state ^= 1;
            }
        }
        else
        {
            frame_counter = 0;
            anim_state = 0;
        }

        draw(anim_state);
    }
}