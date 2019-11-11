/*
 * Copyright (c) 2019 Andrew Steel <copyright@andrewsteel.net>
 *
 * SPDX-License-Identifier: MIT
 *
 * The MIT License:
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/*
 * dropbox.c:
 * Displays the current Dropbox status as an icon, with detailed information
 * displayed in a dropdown menu.
 */

#define _POSIX_C_SOURCE 2 /* Enable POSIX functionality */

/* #include <sysexits.h> */
#define EX_OSERR 71 /* system error (e.g., can't fork) */

#include <stdio.h> /* fgetc,fprintf,NULL,pclose,popen,printf,stderr */
#include <stdlib.h> /* free,malloc,NULL */
#include <string.h> /* NULL,strncmp */

#define BUTTON_DROPDOWN "" /* Generated during compilation */

/*
 * List of recognized states the dropbox client can be in:
 */
enum state {
	APPLYING,  /* Dropbox is applying a setting */
	BLOCKED,   /* Dropbox is blocked from operating */
	DONE,      /* Dropbox has finished synchronizing */
	MISSING,   /* `dropbox` not in PATH, or unrecognized output */
	OFFLINE,   /* No network connection */
	STARTING,  /* Dropbox is initializing */
	SYNCING,   /* Dropbox is synchronizing */
	UPGRADING, /* Dropbox is upgrading */
};

/* Prototypes */
char *get_dropbox_status (char *buffer, size_t buffer_size);
const char *get_icon_name(enum state state);
enum state get_state (char *dropbox_status);
void print_button (const char *, char *);
void print_menu_item (char *);

int main (void)
{
	/*
	 * Create a character buffer and store the dropbox status in it.
	 */
	const short int buffer_size = 1024;
	char *dropbox_status = malloc(sizeof(char) * buffer_size);
	if (! dropbox_status) {
		goto malloc_failed;
	}
	dropbox_status = get_dropbox_status(dropbox_status, buffer_size);

	/*
	 * Generate and print the button using the dropbox status.
	 */
	print_button(get_icon_name(get_state(dropbox_status)), BUTTON_DROPDOWN);
	print_menu_item(dropbox_status);

	cleanup:
		free(dropbox_status);
		return 0;

	malloc_failed:
		fprintf(stderr, "malloc() failed\n");
		return EX_OSERR;
}

/*
 * Stores dropbox status output as a string in the passed character buffer.
 */
char *get_dropbox_status (char *buffer, size_t buffer_size)
{
	/*
	 * Attempt pipe stream initiation from the "dropbox status" command...
	 */
	FILE *stream = popen("dropbox status", "r");

	/*
	 * If successful, copy the stream into the buffer.
	 */
	if (stream != NULL) {
		int c;
		int i = 0;
		while ((c = fgetc(stream)) != EOF && i < buffer_size - 1) {
			buffer[i] = c;
			i++;
		}
		buffer[i] = '\0';
		pclose(stream);
	}

	/*
	 * If unsuccessful, make the buffer an empty string.
	 */
	else {
		buffer[0] = '\0';
	}

	return buffer;
}

/*
 * Returns an icon name corresponding to the passed state.
 */
const char *get_icon_name(enum state state)
{
	switch (state) {
		case APPLYING:
			return "dialog-information-symbolic";
		case BLOCKED:
			return "dialog-warning-symbolic";
		case DONE:
			return "emblem-ok-symbolic";
		default:
		case MISSING:
			return "goa-panel-symbolic";
		case OFFLINE:
			return "network-offline-symbolic";
		case STARTING:
			return "content-loading-symbolic";
		case SYNCING:
			return "emblem-synchronizing-symbolic";
		case UPGRADING:
			return "software-update-available-symbolic";
	}
}

/*
 * Returns a state, determined from the passed dropbox status output.
 */
enum state get_state (char *dropbox_status)
{
	/*
	 * Compare the first word of the dropbox status output against known
	 * states, ordered to minimize comparisons during a session...
	 */
	if (strncmp(dropbox_status, "Up", 2) == 0) {
		return DONE;
	}
	else if (
		strncmp(dropbox_status, "Syncing", 7) == 0
		/* || strncmp(dropbox_status, "Syncing...", 10) == 0 */
	) {
		return SYNCING;
	}
	else if (strncmp(dropbox_status, "Connecting...", 13) == 0) {
		return OFFLINE;
	}
	else if (strncmp(dropbox_status, "Can't", 5) == 0) {
		return BLOCKED;
	}
	else if (strncmp(dropbox_status, "Applying", 8) == 0) {
		return APPLYING;
	}
	else if (
		strncmp(dropbox_status, "Checking", 8) == 0
		|| strncmp(dropbox_status, "Starting...", 11) == 0
	) {
		return STARTING;
	}
	else if (strncmp(dropbox_status, "Upgrading", 9) == 0) {
		return UPGRADING;
	}
	else {
		return MISSING;
	}
}

/*
 * Print a button with an icon and dropdown image.
 */
void print_button (const char *icon_name, char *image)
{
	printf("| iconName=\"%s\" image=\"%s\"\n---\n", icon_name, image);
}

/*
 * Print a menu item.
 */
void print_menu_item (char *item)
{
	printf("%s\n", item);
}
