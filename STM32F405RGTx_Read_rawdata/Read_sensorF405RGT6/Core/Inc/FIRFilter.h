#ifndef FIR_FILTER_H
#define FIR_FILTER_H

#include <stdint.h>

#define FIR_FILTER_LENGTH 15000

typedef struct {
	float 	buf[FIR_FILTER_LENGTH];
	uint16_t bufIndex;
	float   altitude;
    float  pressure;
    float  ground_pressure;
} FIRFilter;

void FIRFilter_Init(FIRFilter *fir);
float FIRFilter_Update(FIRFilter *fir, float inp);

#endif
