#ifndef SAVE_SNAPSHOT_H
#define SAVE_SNAPSHOT_H
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <jpeglib.h>//added
#include <setjmp.h> //added
#include "functions_tools.h"

size_t bmp_save(snapshot_t* snap);
void jpeg_save(snapshot_t *snap);
void save_snapshot(snapshot_t* snap,char* rgb_matrix);

#endif // SAVE_SNAPSHOT_H
