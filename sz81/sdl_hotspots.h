/* sz81 Copyright (C) 2007-2010 Thunor <thunorsif@hotmail.com>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/* Defines */
#define MAX_HOTSPOTS 128

/* Hotspot group IDs */
#define HS_GRP_ROOT 0
#define HS_GRP_LOAD 1
#define HS_GRP_VKEYB 2
#define HS_GRP_CTB 3
#define HS_GRP_RUNOPT0 4
#define HS_GRP_RUNOPT1 5

/* Hotspot IDs (hotspots[] indexes which are also zorders) */
#define HS_ROOT_CANVAS 0
#define HS_LOAD_Q 1
#define HS_LOAD_A 2
#define HS_LOAD_ENTER 3
#define HS_LOAD_SPACE 4
#define HS_VKEYB_VKEYB 5
#define HS_VKEYB_1 6
#define HS_VKEYB_Q 16
#define HS_VKEYB_A 26
#define HS_VKEYB_NEWLINE 35
#define HS_VKEYB_SHIFT 36
#define HS_CTB_CTB 46
#define HS_CTB_EXIT 47
#define HS_CTB_RESET 48
#define HS_CTB_AUTOHIDE 49
#define HS_CTB_SHIFT_TYPE 50
#define HS_CTB_INVERSE 51
#define HS_CTB_ALPHA_DN 52
#define HS_CTB_ALPHA_UP 53
#define HS_CTB_RUNOPTS 54
#define HS_CTB_SPEED_DN 55
#define HS_CTB_SPEED_UP 56
#define HS_CTB_SAVESTATE 57
#define HS_CTB_LOADSTATE 58
#define HS_CTB_SPARE1 59
#define HS_CTB_SPARE2 60
#define HS_CTB_SPARE3 61
#define HS_RUNOPTS0_RUNOPTS0 62
#define HS_RUNOPTS0_VOLUME_DN 63
#define HS_RUNOPTS0_VOLUME_UP 64
#define HS_RUNOPTS0_KRDELAY_DN 65
#define HS_RUNOPTS0_KRDELAY_UP 66
#define HS_RUNOPTS0_KRINTERVAL_DN 67
#define HS_RUNOPTS0_KRINTERVAL_UP 68
#define HS_RUNOPTS0_FGC_R_DN 69
#define HS_RUNOPTS0_FGC_R_UP 70
#define HS_RUNOPTS0_FGC_G_DN 71
#define HS_RUNOPTS0_FGC_G_UP 72
#define HS_RUNOPTS0_FGC_B_DN 73
#define HS_RUNOPTS0_FGC_B_UP 74
#define HS_RUNOPTS0_BGC_R_DN 75
#define HS_RUNOPTS0_BGC_R_UP 76
#define HS_RUNOPTS0_BGC_G_DN 77
#define HS_RUNOPTS0_BGC_G_UP 78
#define HS_RUNOPTS0_BGC_B_DN 79
#define HS_RUNOPTS0_BGC_B_UP 80
#define HS_RUNOPTS0_SAVE 81
#define HS_RUNOPTS0_EXIT 82
#define HS_RUNOPTS0_NEXT 83
#define HS_RUNOPTS0_SPARE1 84
#define HS_RUNOPTS0_SPARE2 85
#define HS_RUNOPTS0_SPARE3 86
#define HS_RUNOPTS0_SPARE4 87
#define HS_RUNOPTS1_RUNOPTS1 88
#define HS_RUNOPTS1_JDEADZ_DN 89
#define HS_RUNOPTS1_JDEADZ_UP 90
#define HS_RUNOPTS1_START_CFG 91
#define HS_RUNOPTS1_CFG_LTRIG 92
#define HS_RUNOPTS1_CFG_RTRIG 93
#define HS_RUNOPTS1_CFG_LEFT 94
#define HS_RUNOPTS1_CFG_RIGHT 95
#define HS_RUNOPTS1_CFG_UP 96
#define HS_RUNOPTS1_CFG_DOWN 97
#define HS_RUNOPTS1_CFG_SELECT 98
#define HS_RUNOPTS1_CFG_START 99
#define HS_RUNOPTS1_CFG_A 100
#define HS_RUNOPTS1_CFG_B 101
#define HS_RUNOPTS1_CFG_Y 102
#define HS_RUNOPTS1_CFG_X 103
#define HS_RUNOPTS1_BACK 104
#define HS_RUNOPTS1_SAVE 105
#define HS_RUNOPTS1_EXIT 106

/* Hotspot properties */
#define HS_PROP_ONOFF 1
#define HS_PROP_STICKY 2
#define HS_PROP_TOGGLE 4
#define HS_PROP_STUCK 8
#define HS_PROP_VISIBLE 16
#define HS_PROP_SELECTED 32
#define HS_PROP_NAVIGABLE 64

/* Variables */
SDL_Surface *highlight;

struct hotspot {
	int gid;			/* Group id for easy management */
	int flags;			/* An OR'd combination of HS_PROP_ properties */
	int hit_x, hit_y, hit_w, hit_h;	/* Hit box */
	int hl_x, hl_y, hl_w, hl_h;		/* Highlight box (if all UNDEFINED then use hitbox */
	int remap_id;		/* The main destination control id (could be UNDEFINED) */
};
struct hotspot hotspots[MAX_HOTSPOTS];

/* Function prototypes */
void hotspots_init(void);
void hotspots_resize(void);
void hotspots_update(void);
void hotspots_render(void);
int get_selected_hotspot(int gid);


