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

/* Includes */
#include "sdl_engine.h"

/* Defines */

/* Variables */

/* Function prototypes */
void clean_up_before_exit(void);


/***************************************************************************
 * SDL Initialise                                                          *
 ***************************************************************************/
/* Since this is the first function the emulator calls prior to
 * sdl_com_line_process (which could exit) many things are done here.
 * Firstly some variables are initialised with defaults, SDL is initialised
 * and then a window manager icon is loaded and a title set if the platform
 * requires it.
 * 
 * On exit: returns TRUE on error
 *          else FALSE */

int sdl_init(void) {
	int count;
	#if defined (PLATFORM_GP2X)
	#elif defined (PLATFORM_ZAURUS)
	#else
		char filename[256];
	#endif
	
	/* Set-up a default video resolution depending on platform */
	#if defined(PLATFORM_GP2X)
		video.xres = 320; video.yres = 240; video.scale = 1;
		video.fullscreen = SDL_FULLSCREEN;
	#elif defined(PLATFORM_ZAURUS)
		video.xres = 640; video.yres = 480; video.scale = 2;
		video.fullscreen = SDL_FULLSCREEN;
	#else
		video.xres = 640; video.yres = 480; video.scale = 2;
		video.fullscreen = FALSE;
	#endif

	/* All SDL pointers are initialised to NULL here since we could
	 * exit prematurely and we don't want to cause seg faults by
	 * freeing nothing. As long as they are NULL everything is fine */
	sdl_emulator.timer_id = NULL;
	control_bar.scaled = NULL;
	vkeyb.zx80original = vkeyb.zx81original = vkeyb.scaled = NULL;
	sz81icons.original = sz81icons.scaled = NULL;
	zx80font.original = zx81font.original = zx82font.original = NULL;
	for (count = 0; count < MAX_FONTS; count++)
		zx80font.scaled[count] = zx81font.scaled[count] = 
			zx82font.scaled[count] = NULL;
	joystick = NULL;
	wm_icon = NULL;

	/* Initialise everything to a default here that could possibly be
	 * overridden by a command line option or from an rcfile */
	joystick_dead_zone = JOYSTICK_DEAD_ZONE;
	sdl_key_repeat.delay = KEY_REPEAT_DELAY;
	sdl_key_repeat.interval = KEY_REPEAT_INTERVAL;
	#if defined(PLATFORM_GP2X)
		sdl_sound.volume = 30;
	#else
		sdl_sound.volume = 128;
	#endif
	colours.colour_key = 0xff0080;
	colours.bmf_fg_default = 0xffffff;
	colours.emu_fg = 0x202018;
	colours.emu_bg = 0xf0f0e8;
	colours.hs_load_selected = 0x00ff00;
	colours.hs_load_pressed = 0xffc000;
	colours.hs_vkeyb_zx80_selected = 0x00ff00;
	colours.hs_vkeyb_zx80_pressed = 0xffff00;
	colours.hs_vkeyb_zx80_toggle_pressed = 0xff4000;
	colours.hs_vkeyb_zx81_selected = 0x00ff00;
	colours.hs_vkeyb_zx81_pressed = 0xffc000;
	colours.hs_vkeyb_zx81_toggle_pressed = 0xff4000;
	colours.hs_ctb_selected = 0x00ff00;
	colours.hs_ctb_pressed = 0xffc000;
	colours.hs_options_selected = 0x00ff00;
	colours.hs_options_pressed = 0xffc000;
	current_input_id = UNDEFINED;
	sdl_com_line.fullscreen = UNDEFINED;
	sdl_com_line.scale = UNDEFINED;
	sdl_com_line.xres = UNDEFINED;
	sdl_com_line.yres = UNDEFINED;
	sdl_com_line.filename[0] = 0;

	/* Initialise other things that need to be done before sdl_video_setmode */
	sdl_emulator.state = TRUE;
	sdl_emulator.speed = 50;	/* 50Hz is the default */
	sdl_sound.state = FALSE;
	sdl_zx80rom.state = FALSE;
	sdl_zx81rom.state = FALSE;
	vkeyb.state = vkeyb.autohide = vkeyb.toggle_shift = FALSE;
	vkeyb.alpha = SDL_ALPHA_OPAQUE;
	runtime_options[0].state = FALSE;
	runtime_options[0].text = runtime_options_text0;
	runtime_options[1].state = FALSE;
	runtime_options[1].text = runtime_options_text1;
	runtime_options[2].state = FALSE;
	runtime_options[2].text = runtime_options_text2;
	runtime_options[3].state = FALSE;
	runtime_options[3].text = runtime_options_text3;
	ctrl_remapper.state = FALSE;
	joy_cfg.state = FALSE;
	rcfile.rewrite = FALSE;
	show_input_id = FALSE;

	/* Initialise SDL */
	if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_JOYSTICK)) {
		fprintf(stderr, "%s: Cannot initialise SDL: %s", __func__,
			SDL_GetError());
		return TRUE;
	}

	atexit(clean_up_before_exit);

	/* Set WM icon and title if required for platform before
	 * setting a video mode as per SDL docs instructions */
	#if defined (PLATFORM_GP2X)
	#elif defined (PLATFORM_ZAURUS)
	#else
		strcpy(filename, PACKAGE_DATA_DIR "/");
		strcat(filename, IMG_WM_ICON);

		/* Load the bitmap */
		if ((wm_icon = SDL_LoadBMP(filename)) == NULL) {
			fprintf(stderr, "%s: Cannot load window manager icon %s: %s\n",
				__func__, filename, SDL_GetError ());
		} else {
			SDL_WM_SetIcon (wm_icon, NULL);
		}

		/* Set display window title */
		SDL_WM_SetCaption("sz81", "sz81");
	#endif

	/* Record the current working directory.
	 * Once the load selector has been replaced this code
	 * should be reviewed. I'll mark it temp temp */
	strcpy(workdir, "");
	getcwd(workdir, 256);

	/* Set-up the local data directory */
	local_data_dir_init();
				
	return FALSE;
}

/***************************************************************************
 * Process the Command Line Options                                        *
 ***************************************************************************/
/* On exit: returns TRUE on error
 *          else FALSE */

int sdl_com_line_process(int argc, char *argv[]) {
	int count;

#ifdef __amigaos4__
	if(argc == 0) amiga_read_tooltypes((struct WBStartup *)argv);
#endif

	if (argc > 1) {
		for (count = 1; count < argc; count++) {
			if (!strcmp (argv[count], "-f")) {
				sdl_com_line.fullscreen = TRUE;
			} else if (!strcmp (argv[count], "-w")) {
				sdl_com_line.fullscreen = FALSE;
			} else if (sscanf (argv[count], "-%ix%i", 
				&sdl_com_line.xres, &sdl_com_line.yres) == 2) {
				if (sdl_com_line.xres < 240 || sdl_com_line.yres < 240) {
					fprintf (stdout, "Invalid resolution: a minimum of 240x240 is required.\n");
					return TRUE;
				}
			} else if (strstr(argv[count], ".o") != NULL ||
				strstr(argv[count], ".p") != NULL ||
				strstr(argv[count], ".80") != NULL ||
				strstr(argv[count], ".81") != NULL) {
				strcpy(sdl_com_line.filename, argv[count]);
			} else {
				/*   1234567890123456789012345678901234567890 <- Formatting for small terminal. */
				fprintf (stdout,
					"z81 2.1 - copyright (C) 1994-2004 Ian Collier and Russell Marks.\n"
					"sz81 " VERSION " - copyright (C) 2007-2010 Thunor and Chris Young.\n\n"
					"usage: sz81 [-fhw] [-XRESxYRES] [filename.{o|p|80|81}]\n\n"
					"  -f  run the program fullscreen\n"
					"  -h  this usage help\n"
					"  -w  run the program in a window\n"
					"  -XRESxYRES e.g. -800x480\n\n");
				return TRUE;
			}
		}
	}

	/* Process sz81's command line options */
	if (sdl_com_line.fullscreen != UNDEFINED && sdl_com_line.fullscreen)
		video.fullscreen = SDL_FULLSCREEN;
	if (sdl_com_line.xres != UNDEFINED) {
		/* Calculate the scale for the requested resolution */
		if (sdl_com_line.xres / 240 > sdl_com_line.yres / 240) {
			sdl_com_line.scale = sdl_com_line.yres / 240;
		} else {
			sdl_com_line.scale = sdl_com_line.xres / 240;
		}
		video.scale = sdl_com_line.scale;
		video.xres = sdl_com_line.xres;
		video.yres = sdl_com_line.yres;
	}

	#ifdef SDL_DEBUG_COM_LINE
		printf("%s:\n", __func__);
		printf("  argc=%i\n", argc);
		for (count = 0; count < argc; count++) {
			printf("  %i: %s\n", count, argv[count]);
		}
		printf("  sdl_com_line.fullscreen=%i\n", sdl_com_line.fullscreen);
		printf("  sdl_com_line.scale=%i\n", sdl_com_line.scale);
		printf("  sdl_com_line.xres=%i\n", sdl_com_line.xres);
		printf("  sdl_com_line.yres=%i\n", sdl_com_line.yres);
		printf("  sdl_com_line.filename=%s\n", sdl_com_line.filename);
	#endif

	return FALSE;
}

/***************************************************************************
 * Component Executive                                                     *
 ***************************************************************************/
/* This function monitors program component state changes and if something
 * needs to be done as a result of a state change then it is managed from
 * here to keep things organised and to cut down on code duplication */

void component_executive(void) {
	static int active_components = COMP_EMU;
	static int ctrl_remapper_state = FALSE;
	int found = FALSE;
	int count;
	
	/* Monitor emulator's state */
	if ((active_components & COMP_EMU) != (sdl_emulator.state * COMP_EMU)) {
		if (!sdl_emulator.state && load_selector_state) load_selector_state = FALSE;
		found = TRUE;
	}

	/* Monitor load selector's state */
	if ((active_components & COMP_LOAD) != (load_selector_state * COMP_LOAD)) {
		if (load_selector_state && vkeyb.state) vkeyb.state = FALSE;
		found = TRUE;
	}

	/* Monitor runtime options' state */
	for (count = 0; count < MAX_RUNTIME_OPTIONS; count++) {
		if ((active_components & (COMP_RUNOPTS0 << count)) !=
			(runtime_options[count].state * (COMP_RUNOPTS0 << count))) {
			if ((runtime_options_which() < MAX_RUNTIME_OPTIONS) && vkeyb.state)
				vkeyb.state = FALSE;
			/* Update the joycfg text */
			set_joy_cfg_text(0);
			video.redraw = TRUE;
			found = TRUE;
			break;
		}
	}

	/* Monitor virtual keyboard's state */
	if ((active_components & COMP_VKEYB) != (vkeyb.state * COMP_VKEYB)) {
		if (!vkeyb.state) video.redraw = TRUE;
		found = TRUE;
	}

	if (found) {
		keyboard_buffer_reset(FALSE);
		key_repeat_manager(KRM_FUNC_RELEASE, NULL, 0);
		/* Update hotspot states */
		hotspots_update();
	}

	/* Monitor control remapper's state */ 
	if (ctrl_remapper_state != ctrl_remapper.state) {
		if (ctrl_remapper.state) ctrl_remapper.interval = 0;
	}
	/* Manage control remapper's interval decrementing and resetting from here */
	if (ctrl_remapper.state && --ctrl_remapper.interval <= 0)
		ctrl_remapper.interval = ctrl_remapper.master_interval;

	/* Maintain a copy of current program component states  */
	ctrl_remapper_state = ctrl_remapper.state;
	active_components = 0;
	if (sdl_emulator.state) {
		active_components |= COMP_EMU;
	} else {
		active_components &= ~COMP_EMU;
	}
	if (load_selector_state) {
		active_components |= COMP_LOAD;
	} else {
		active_components &= ~COMP_LOAD;
	}
	if (vkeyb.state) {
		active_components |= COMP_VKEYB;
	} else {
		active_components &= ~COMP_VKEYB;
	}
	for (count = 0; count < MAX_RUNTIME_OPTIONS; count++) {
		if (runtime_options[count].state) {
			active_components |= (COMP_RUNOPTS0 << count);
		} else {
			active_components &= ~(COMP_RUNOPTS0 << count);
		}
	}
}

/***************************************************************************
 * Get Active Component                                                    *
 ***************************************************************************/
/* This returns the main currently active component */

int get_active_component(void) {
	int retval;
	
	if (runtime_options_which() < MAX_RUNTIME_OPTIONS) {
		retval = COMP_RUNOPTS0 << runtime_options_which();
	} else if (vkeyb.state) {
		retval = COMP_VKEYB;
	} else if (load_selector_state) {
		retval = COMP_LOAD;
	} else {
		retval = COMP_EMU;
	}
	
	return retval;
}

/***************************************************************************
 * Runtime Options Which                                                   *
 ***************************************************************************/
/*  On exit: returns 0 to MAX_RUNTIME_OPTIONS-1 if one of the
 *               COMP_RUNOPTSx is active
 *           returns MAX_RUNTIME_OPTIONS if none of them are active */

int runtime_options_which(void) {
	int count;
	
	for (count = 0; count < MAX_RUNTIME_OPTIONS; count++)
		if (runtime_options[count].state) break;
	
	return count;
}

/***************************************************************************
 * Timer Initialise                                                        *
 ***************************************************************************/

void sdl_timer_init(void) {
	/* Create a 10ms timer */
	sdl_emulator.timer_id = SDL_AddTimer (10, emulator_timer, NULL);
}

/***************************************************************************
 * Emulator Timer                                                          *
 ***************************************************************************/

Uint32 emulator_timer (Uint32 interval, void *param) {
	static int intervals = 0;

	intervals++;
	if (intervals >= 100 / sdl_emulator.speed) {
		signal_int_flag = TRUE;
		intervals = 0;
	}
	
	return interval;
}

/***************************************************************************
 * Clean Up Before Exit                                                    *
 ***************************************************************************/

void clean_up_before_exit(void) {
	int count;

	sdl_sound_end();

	if (sdl_emulator.timer_id) SDL_RemoveTimer (sdl_emulator.timer_id);

	if (rcfile.rewrite) rcfile_write();

	if (control_bar.scaled) SDL_FreeSurface(control_bar.scaled);
	if (vkeyb.zx80original) SDL_FreeSurface(vkeyb.zx80original);
	if (vkeyb.zx81original) SDL_FreeSurface(vkeyb.zx81original);
	if (vkeyb.scaled) SDL_FreeSurface(vkeyb.scaled);
	if (sz81icons.original) SDL_FreeSurface(sz81icons.original);
	if (sz81icons.scaled) SDL_FreeSurface(sz81icons.scaled);
	if (zx80font.original) SDL_FreeSurface(zx80font.original);
	if (zx81font.original) SDL_FreeSurface(zx81font.original);
	if (zx82font.original) SDL_FreeSurface(zx82font.original);
	for (count = 0; count < MAX_FONTS; count++) {
		if (zx80font.scaled[count]) SDL_FreeSurface(zx80font.scaled[count]);
		if (zx81font.scaled[count]) SDL_FreeSurface(zx81font.scaled[count]);
		if (zx82font.scaled[count]) SDL_FreeSurface(zx82font.scaled[count]);
	}

	#ifdef PLATFORM_GP2X
		#if defined(TOOLCHAIN_OPEN2X) || defined(TOOLCHAIN_OPENWIZ)
			if (joystick) SDL_JoystickClose(joystick);
		#endif
	#else
		if (joystick) SDL_JoystickClose(joystick);
	#endif

	if (wm_icon) SDL_FreeSurface(wm_icon);

	SDL_Quit();
}

/***************************************************************************
 * Reset                                                                   *
 ***************************************************************************/
/* This resets and reinitialises the few parts of sz81 that require this
 * to support peripheral and machine type changes */

void sdl_reset(void) {

	//zx80 ^= 1;	// temp temp

	/*switch (memory_size) {	 temp temp /
		case 1:
		case 2:
		case 4:
		case 8:
		case 16:
			memory_size *= 2;
			break;
		case 32:
			memory_size = 48;
			break;
		case 48:
			memory_size = 1;
			break;
	}
	printf("%s: memory_size=%i\n", __func__, memory_size);	/ temp temp */

	keyboard_buffer_reset(TRUE);
	key_repeat_manager(KRM_FUNC_RELEASE, NULL, 0);
	vkeyb_init();
	hotspots_init();
}


