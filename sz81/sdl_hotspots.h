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
#define MAX_HOTSPOTS 206

/* Hotspot group IDs */
#define HS_GRP_ROOT 1
#define HS_GRP_LOAD 2
#define HS_GRP_VKEYB 4
#define HS_GRP_CTB 8
#define HS_GRP_RUNOPTS0 16
#define HS_GRP_RUNOPTS1 32
#define HS_GRP_RUNOPTS2 64
#define HS_GRP_RUNOPTS3 128
#define HS_GRP_RUNOPTS_ALL (HS_GRP_RUNOPTS0 | HS_GRP_RUNOPTS1 | HS_GRP_RUNOPTS2 | HS_GRP_RUNOPTS3)
#define HS_GRP_ALL ((HS_GRP_RUNOPTS3 - 1) | HS_GRP_RUNOPTS3)

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
#define HS_RUNOPTS0_RUNOPTS0 96
#define HS_RUNOPTS0_ZX80 97
#define HS_RUNOPTS0_ZX81 98
#define HS_RUNOPTS0_RAM_DN 99
#define HS_RUNOPTS0_RAM_UP 100
#define HS_RUNOPTS0_FRAMESKIP_DN 101
#define HS_RUNOPTS0_FRAMESKIP_UP 102
#define HS_RUNOPTS0_SPEED_DN 103
#define HS_RUNOPTS0_SPEED_UP 104
#define HS_RUNOPTS0_SAVE 105
#define HS_RUNOPTS0_EXIT 106
#define HS_RUNOPTS0_NEXT 107

#define HS_RUNOPTS2_RUNOPTS2 162
#define HS_RUNOPTS2_VOLUME_DN 163
#define HS_RUNOPTS2_VOLUME_UP 164
#define HS_RUNOPTS2_KRDELAY_DN 165
#define HS_RUNOPTS2_KRDELAY_UP 166
#define HS_RUNOPTS2_KRINTERVAL_DN 167
#define HS_RUNOPTS2_KRINTERVAL_UP 168
#define HS_RUNOPTS2_FGC_R_DN 169
#define HS_RUNOPTS2_FGC_R_UP 170
#define HS_RUNOPTS2_BGC_R_DN 171
#define HS_RUNOPTS2_BGC_R_UP 172
#define HS_RUNOPTS2_FGC_G_DN 173
#define HS_RUNOPTS2_FGC_G_UP 174
#define HS_RUNOPTS2_BGC_G_DN 175
#define HS_RUNOPTS2_BGC_G_UP 176
#define HS_RUNOPTS2_FGC_B_DN 177
#define HS_RUNOPTS2_FGC_B_UP 178
#define HS_RUNOPTS2_BGC_B_DN 179
#define HS_RUNOPTS2_BGC_B_UP 180
#define HS_RUNOPTS2_SAVE 181
#define HS_RUNOPTS2_EXIT 182
#define HS_RUNOPTS2_NEXT 183
#define HS_RUNOPTS2_SPARE1 184
#define HS_RUNOPTS2_SPARE2 185
#define HS_RUNOPTS2_SPARE3 186
#define HS_RUNOPTS2_SPARE4 187
#define HS_RUNOPTS3_RUNOPTS3 188
#define HS_RUNOPTS3_JDEADZ_DN 189
#define HS_RUNOPTS3_JDEADZ_UP 190
#define HS_RUNOPTS3_JOY_CFG_LTRIG 191
#define HS_RUNOPTS3_JOY_CFG_RTRIG 192
#define HS_RUNOPTS3_JOY_CFG_LEFT 193
#define HS_RUNOPTS3_JOY_CFG_RIGHT 194
#define HS_RUNOPTS3_JOY_CFG_UP 195
#define HS_RUNOPTS3_JOY_CFG_DOWN 196
#define HS_RUNOPTS3_JOY_CFG_SELECT 197
#define HS_RUNOPTS3_JOY_CFG_START 198
#define HS_RUNOPTS3_JOY_CFG_A 199
#define HS_RUNOPTS3_JOY_CFG_B 200
#define HS_RUNOPTS3_JOY_CFG_Y 201
#define HS_RUNOPTS3_JOY_CFG_X 202
#define HS_RUNOPTS3_BACK 203
#define HS_RUNOPTS3_SAVE 204
#define HS_RUNOPTS3_EXIT 205

/* Hotspot properties */
#define HS_PROP_ONOFF 1
#define HS_PROP_STICKY 2
#define HS_PROP_TOGGLE 4
#define HS_PROP_STUCK 8
#define HS_PROP_VISIBLE 16
#define HS_PROP_SELECTED 32
#define HS_PROP_NAVIGABLE 64

/* Variables */
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
void hotspots_vkeyb_shift_init(void);
void hotspots_resize(int gid);
void hotspots_update(void);
void hotspots_render(void);
int get_selected_hotspot(int gid);


