
/*
A huge library of NES sounds converted to Famitone.
*/

#include "neslib.h"
#include "vrambuf.h"
//#link "vrambuf.c"

// link the pattern table into CHR ROM
//#link "chr_generic.s"

// setup Famitone library

//#link "famitone2.s"
void __fastcall__ famitone_update(void);
//#link "music_player_title_screen.s"
extern char player_music_data[];
//#link "demosounds.s"
extern char demo_sounds[];

int i;       //for loops
char oam_id; //for sprites
bool start_pressed = false; // Only allows one input from Start Button at a time.
bool select_pressed = false; // Only allows one input from Select Button at a time.
bool a_pressed = false; // Only allows one input from A Button at a time.
bool b_pressed = false; // Only allows one input from B Button at a time.
bool left_pressed = false; // Only allows one input from Select Button at a time.
bool right_pressed = false; // Only allows one input from Select Button at a time.
int sfx_timer; //pauses music to play sound
char title_select = 0;
char selected_sfx = 0;


const char ATTRIBUTE_TABLE_1[0x40] = {
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // rows 0-3
  0xFF, 0xFF, 0xFF, 0xFF, 0x55, 0x55, 0xFF, 0xFF, // rows 4-7
  0xFF, 0xFF, 0x0F, 0x47, 0x5F, 0x5F, 0xFF, 0xFF, // rows 8-11
  0xFF, 0xFF, 0xF0, 0x74, 0x55, 0x55, 0xFF, 0xFF, // rows 12-15
  0xFF, 0xFF, 0xFF, 0xA7, 0xA5, 0xA5, 0xFF, 0xFF, // rows 16-19
  0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, // rows 20-23
  0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, // rows 24-27
  0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA  // rows 28-29
                                     };


const char TITLE_POINTER[][1] = 
    {
    {0x00}, {0x28}
    }; // Number String For Inventory


const char PALETTE[32] = {
    0x0F, // screen color
          0x21, 0x00, 0x30, // background palette 0
    0x0F, 0x0C, 0x00, 0x30, // background palette 1
    0x0F, 0x05, 0x23, 0x30, // background palette 2
    0x0F, 0x16, 0x00, 0x30, // background palette 3

    0x0F, 0x00, 0x00, 0x00, // sprite palette 0
    0x0F, 0x00, 0x00, 0x00, // sprite palette 1
    0x0F, 0x00, 0x00, 0x00, // sprite palette 2
    0x0F, 0x00, 0x00, 0x00  // sprite palette 3
                         }; //Screen Pallette

const char TITLE_SCREEN[960] = 
    {
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x40, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 
0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 
0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 
0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 
0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 
0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 
0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 
0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 
0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 
0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 
0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 
0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0x5F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0x4E, 0x45, 0x53, 0xFF, 0x53, 0x4F, 0x55, 0x4E, 0x44, 0xFF, 0xFF, 0x45, 
0x46, 0x46, 0x45, 0x43, 0x54, 0xFF, 0x50, 0x4C, 0x41, 0x59, 0x45, 0x52, 0xFF, 0xFF, 0xFF, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3B, 0xFF, 0x4E, 0x4F, 0x57, 0xFF, 0x50, 0x4C, 
0x41, 0x59, 0x49, 0x4E, 0x47, 0x3A, 0xFF, 0x3C, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x32, 0x30, 0x32, 0x33, 0xFF, 0xFF, 0x52, 0x41, 0x4E, 
0x49, 0xFF, 0x54, 0x49, 0x4D, 0x45, 0x4B, 0x45, 0x59, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
    }; //Title Screen Tiles


void draw_background()
{
  ppu_off();
  pal_all(PALETTE);
  vram_adr(0x23c0);
  vram_write(ATTRIBUTE_TABLE_1, sizeof(ATTRIBUTE_TABLE_1));
  for (i = 0; i < sizeof(TITLE_SCREEN); i++)
    { //Draw_Title Screen
    vram_put(TITLE_SCREEN[i]);
    }
  ppu_on_bg();
}

void main(void)
{
  draw_background();
  vram_adr(NTADR_A(2,2));
    // initialize music system
  //famitone_init(gabalin_music_data);
  vrambuf_clear();
  set_vram_update(updbuf);
  famitone_init(player_music_data);
  sfx_init(demo_sounds);
  // set music callback function for NMI
  nmi_set_callback(famitone_update);
  // play music
  sfx_play(selected_sfx, 0);
  //enable rendering
  ppu_on_all();
  // repeat forever
  while(1) {
    // poll controller 0
    char pad = pad_poll(0);
    ppu_wait_frame();
    vrambuf_clear();
    set_vram_update(updbuf);
    // play sounds when buttons pushed
    if (pad & PAD_A) 
       {
       if (!a_pressed)
          {
          sfx_play(selected_sfx, 0);
          }    	
       else
          {
          a_pressed=false;
          }
    	}
    if (pad & PAD_B) 
    	{
      	// sfx_play(1,1); // commenting out other sfx for now
    	}
    if (pad & PAD_DOWN) 
    	{
      	//sfx_play(2,2); // commenting out other sfx for now
    	}
    if (pad & PAD_LEFT) 
    	{
      	if (!left_pressed)
            {
            left_pressed=true;
            if (selected_sfx > 0)
               {
               selected_sfx--;
               sfx_play(selected_sfx, 0);
               }   
            else
               {
               selected_sfx = 54;
               sfx_play(selected_sfx, 0);
               }
            }
    	}
    else
        {
        left_pressed=false;
        }
    if (pad & PAD_RIGHT) 
    	{
      	if (!right_pressed)
            {
            right_pressed=true;
            if (selected_sfx < 54)
               {
               selected_sfx++;
               sfx_play(selected_sfx , 0);
               } 
            else
               {
               selected_sfx = 0;
               sfx_play(3,0);
               sfx_play(selected_sfx, 0);
               }
            }
    	}
    else
        {
        right_pressed=false;
        }
    if (pad & PAD_SELECT) 
    	{
      	if (!select_pressed)
            {
            select_pressed=true;
            //if (!title_select)
               {
               //sfx_play(3,0);
               //title_select = 1;
               }
            //else
               {
               //sfx_play(3,0);
               //title_select = 0;
               }             
            }
        }
    else
        {
        select_pressed=false;
        }
    if (pad & PAD_START) 
    	{
        if (!start_pressed)
    	    {
      	    sfx_play(selected_sfx, 0);
            }
    	}
    else
  	{
    	start_pressed = false;
  	}
   if (sfx_timer)
        {
        // music_pause(1); //Pauses Music For Gabalin Start Noise
        sfx_timer--;
        }
   else
        {
        // music_pause(0); //Unpauses Music When Timer Runs Out
        }  
   if (!title_select)
        {
        //vrambuf_put(NTADR_A(11, 19), TITLE_POINTER[1], 1);
        //vrambuf_put(NTADR_A(11, 21), TITLE_POINTER[0], 1);
        }
   else
        {
        //vrambuf_put(NTADR_A(11, 19), TITLE_POINTER[0], 1);
        //vrambuf_put(NTADR_A(11, 21), TITLE_POINTER[1], 1);
        }             
   if (selected_sfx == 0)
        {
        vrambuf_put(NTADR_A(12, 24), "START  1", 8);
        }
   if (selected_sfx == 1)
        {
        vrambuf_put(NTADR_A(12, 24), "START  2", 8);
        }
   if (selected_sfx == 2)
        {
        vrambuf_put(NTADR_A(12, 24), "START  3", 8);
        }  
   if (selected_sfx == 3)
        {
        vrambuf_put(NTADR_A(12, 24), "START  4", 8);
        }  
   if (selected_sfx == 4)
        {
        vrambuf_put(NTADR_A(12, 24), " COIN 1 ", 8);
        } 
   if (selected_sfx == 5)
        {
        vrambuf_put(NTADR_A(12, 24), " COIN 2 ", 8);
        } 
   if (selected_sfx == 6)
        {
        vrambuf_put(NTADR_A(12, 24), " COIN 3 ", 8);
        }   
   if (selected_sfx == 7)
        {
        vrambuf_put(NTADR_A(12, 24), " ITEM 1 ", 8);
        }   
   if (selected_sfx == 8)
        {
        vrambuf_put(NTADR_A(12, 24), " ITEM 2 ", 8);
        } 
   if (selected_sfx == 9)
        {
        vrambuf_put(NTADR_A(12, 24), " ITEM 3 ", 8);
        } 
   if (selected_sfx == 10)
        {
        vrambuf_put(NTADR_A(12, 24), " ITEM 4 ", 8);
        } 
   if (selected_sfx == 11)
        {
        vrambuf_put(NTADR_A(12, 24), " ITEM 5 ", 8);
        }     
   if (selected_sfx == 12)
        {
        vrambuf_put(NTADR_A(12, 24), " ITEM 6 ", 8);
        } 
   if (selected_sfx == 13)
        {
        vrambuf_put(NTADR_A(12, 24), " ITEM 7 ", 8);
        }     
   if (selected_sfx == 14)
        {
        vrambuf_put(NTADR_A(12, 24), " ITEM 8 ", 8);
        } 
   if (selected_sfx == 15)
        {
        vrambuf_put(NTADR_A(12, 24), " ITEM 9 ", 8);
        } 
   if (selected_sfx == 16)
        {
        vrambuf_put(NTADR_A(12, 24), " MENU 1 ", 8);
        }     
   if (selected_sfx == 17)
        {
        vrambuf_put(NTADR_A(12, 24), " MENU 2 ", 8);
        }     
   if (selected_sfx == 18)
        {
        vrambuf_put(NTADR_A(12, 24), " MENU 3 ", 8);
        }         
   if (selected_sfx == 19)
        {
        vrambuf_put(NTADR_A(12, 24), " MENU 4 ", 8);
        } 
   if (selected_sfx == 20)
        {
        vrambuf_put(NTADR_A(12, 24), " MENU 5 ", 8);
        } 
   if (selected_sfx == 21)
        {
        vrambuf_put(NTADR_A(12, 24), " MENU 6 ", 8);
        } 
   if (selected_sfx == 22)
        {
        vrambuf_put(NTADR_A(12, 24), " MENU 7 ", 8);
        } 
   if (selected_sfx == 23)
        {
        vrambuf_put(NTADR_A(12, 24), " MENU 8 ", 8);
        }    
   if (selected_sfx == 24)
        {
        vrambuf_put(NTADR_A(12, 24), "RESPAWN1", 8);
        }    
   if (selected_sfx == 25)
        {
        vrambuf_put(NTADR_A(12, 24), "RESPAWN2", 8);
        }    
   if (selected_sfx == 26)
        {
        vrambuf_put(NTADR_A(12, 24), "RESPAWN3", 8);
        }        
   if (selected_sfx == 27)
        {
        vrambuf_put(NTADR_A(12, 24), "RESPAWN4", 8);
        }  
   if (selected_sfx == 28)
        {
        vrambuf_put(NTADR_A(12, 24), " JUMP 1 ", 8);
        } 
   if (selected_sfx == 29)
        {
        vrambuf_put(NTADR_A(12, 24), " JUMP 2 ", 8);
        }     
   if (selected_sfx == 30)
        {
        vrambuf_put(NTADR_A(12, 24), " JUMP 3 ", 8);
        } 
   if (selected_sfx == 31)
        {
        vrambuf_put(NTADR_A(12, 24), " FALL 1 ", 8);
        } 
   if (selected_sfx == 32)
        {
        vrambuf_put(NTADR_A(12, 24), " FALL 2 ", 8);
        } 
   if (selected_sfx == 33)
        {
        vrambuf_put(NTADR_A(12, 24), "BRAKES 1", 8);
        }     
   if (selected_sfx == 34)
        {
        vrambuf_put(NTADR_A(12, 24), "BRAKES 2", 8);
        } 
   if (selected_sfx == 35)
        {
        vrambuf_put(NTADR_A(12, 24), " STAIRS ", 8);
        } 
   if (selected_sfx == 36)
        {
        vrambuf_put(NTADR_A(12, 24), " HIT  1 ", 8);
        } 
   if (selected_sfx == 37)
        {
        vrambuf_put(NTADR_A(12, 24), " HIT  2 ", 8);
        } 
   if (selected_sfx == 38)
        {
        vrambuf_put(NTADR_A(12, 24), " HIT  3 ", 8);
        } 
   if (selected_sfx == 39)
        {
        vrambuf_put(NTADR_A(12, 24), " HIT  4 ", 8);
        } 
   if (selected_sfx == 40)
        {
        vrambuf_put(NTADR_A(12, 24), " HIT  5 ", 8);
        } 
   if (selected_sfx == 41)
        {
        vrambuf_put(NTADR_A(12, 24), " HIT  6 ", 8);
        } 
   if (selected_sfx == 42)
        {
        vrambuf_put(NTADR_A(12, 24), " HIT  7 ", 8);
        }     
   if (selected_sfx == 43)
        {
        vrambuf_put(NTADR_A(12, 24), " HIT  8 ", 8);
        } 
   if (selected_sfx == 44)
        {
        vrambuf_put(NTADR_A(12, 24), " HIT  9 ", 8);
        }    
   if (selected_sfx == 45)
        {
        vrambuf_put(NTADR_A(12, 24), "MAGIC 1 ", 8);
        }  
   if (selected_sfx == 46)
        {
        vrambuf_put(NTADR_A(12, 24), "MAGIC 2 ", 8);
        }  
   if (selected_sfx == 47)
        {
        vrambuf_put(NTADR_A(12, 24), "MAGIC 3 ", 8);
        }  
   if (selected_sfx == 48)
        {
        vrambuf_put(NTADR_A(12, 24), " BOMB 1 ", 8);
        } 
   if (selected_sfx == 49)
        {
        vrambuf_put(NTADR_A(12, 24), " BOMB 2 ", 8);
        } 
   if (selected_sfx == 50)
        {
        vrambuf_put(NTADR_A(12, 24), " BOMB 3 ", 8);
        }  
   if (selected_sfx == 51)
        {
        vrambuf_put(NTADR_A(12, 24), "DEATH  1", 8);
        } 
   if (selected_sfx == 52)
        {
        vrambuf_put(NTADR_A(12, 24), "DEATH  2", 8);
        }     
   if (selected_sfx == 53)
        {
        vrambuf_put(NTADR_A(12, 24), " WATER  ", 8);
        } 
   if (selected_sfx == 54)
        {
        vrambuf_put(NTADR_A(12, 24), "  FIRE  ", 8);
        }     
   }
}
