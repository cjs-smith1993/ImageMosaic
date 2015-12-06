#ifndef __TIME_H
#define __TIME_H

#include <sys/time.h>

double getCurrentTime() {
	struct timeval tp;
	gettimeofday(&tp, NULL);
	return ((double) tp.tv_sec + (double) tp.tv_usec * 1e-6);
}

#endif