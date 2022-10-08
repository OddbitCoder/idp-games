/*
 * done.h
 *
 * Miha Grčar 2022
 *
 * Save and restore game
 */

#ifndef __SAVE_H
#define __SAVE_H

int save(char *outfile);
int restore(char *infile);

#endif