#include "FIRFilter.h"
#include "math.h"
static float FIR_IMPULSE_RESPONSE[FIR_FILTER_LENGTH] = {0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f};


void FIRFilter_Init(FIRFilter *fir) {

	/* Clear filter buffer */
	for (uint16_t n = 0; n < FIR_FILTER_LENGTH; n++) {

		fir->buf[n] = 0.0f;

	}

	/* Reset buffer index */
	fir->bufIndex = 0;

	/* Clear filter output */
	fir->pressure = 0.0f;

}

float FIRFilter_Update(FIRFilter *fir, float inp) {

	/* Store latest sample in buffer */
	fir->buf[fir->bufIndex] = inp;

	/* Increment buffer index and wrap around if necessary */
	fir->bufIndex++;

	if (fir->bufIndex == FIR_FILTER_LENGTH) {

		fir->bufIndex = 0;

	}

	/* Compute new output sample (via convolution) */
	fir->pressure = 0.0f;
	fir->ground_pressure = fir->buf[0];

	uint16_t sumIndex = fir->bufIndex;

	for (uint16_t n = 0; n < FIR_FILTER_LENGTH; n++) {

		/* Decrement index and wrap if necessary */
		if (sumIndex > 0) {

			sumIndex--;

		} else {

			sumIndex = FIR_FILTER_LENGTH - 1;

		}

		/* Multiply impulse response with shifted input sample and add to output */
		fir->pressure += FIR_IMPULSE_RESPONSE[n] * fir->buf[sumIndex];
		fir->altitude=44330.0f*(1-powf(fir->pressure/fir->ground_pressure,1.0f/5.255f));

	}

	/* Return filtered output */
	return fir->pressure;

}
