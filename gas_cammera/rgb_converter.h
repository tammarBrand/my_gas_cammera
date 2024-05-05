#pragma once
#ifndef RGB_CONVERTER_H
#define RGB_CONVERTER_H
#include <stdio.h>
#include <stdlib.h>
#define SNAPSHOT_HEIGHT 240
#define SNAPSHOT_WIDTH 360
#define MAX_TEMP 30
#include "functions_tools.h"
#include "save_snapshot.h"


void* rgb_converter(void* data, void* params);


#endif // RGB_CONVERTER_H
