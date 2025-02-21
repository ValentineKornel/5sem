/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2024.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* Supplementary program for Chapter 41 */

/* mod1.c */

#include <stdio.h>
#include <unistd.h>
int test1[250000] = { 1, 2, 3 };

#ifndef VERSION
#define VERSION ""
#endif

void
x1(void) {
    printf("Called mod1-x1 " VERSION "\n");
}

void __attribute__ ((constructor)) some_name_load(void)
{
/* Код инициализации */
}

void __attribute__ ((destructor)) some_name_unload(void)
{
/* Код финализации */
}