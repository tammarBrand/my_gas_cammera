#pragma once
#ifndef ENCODE_H
#define ENCODE_H
#include <libavcodec/avcodec.h>
#include "functions_tools.h"
#include <sys/time.h>

void* encode(void* data, void* params);

#endif // ENCODE_H
