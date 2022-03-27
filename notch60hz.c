/******************************* SOURCE LICENSE *********************************
Copyright (c) 2021 MicroModeler.

A non-exclusive, nontransferable, perpetual, royalty-free license is granted to the Licensee to 
use the following Information for academic, non-profit, or government-sponsored research purposes.
Use of the following Information under this License is restricted to NON-COMMERCIAL PURPOSES ONLY.
Commercial use of the following Information requires a separately executed written license agreement.

This Information is distributed WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

******************************* END OF LICENSE *********************************/

// A commercial license for MicroModeler DSP can be obtained at https://www.micromodeler.com/launch.jsp

#include "notch60hz.h"

#include <stdlib.h> // For malloc/free
#include <string.h> // For memset

float notch60hz_coefficients[20] = 
{
// Scaled for floating point

    2.7635129804998297, 1.7087881340923488, 2.7635129804998297, -0.5785065425330213, -0.9433637270970286,// b0, b1, b2, a1, a2
    2, 1.2366782035402522, 2, -0.623114796591652, -0.9437926334350678,// b0, b1, b2, a1, a2
    1, 0.6183391017701261, 1, -0.5558689349960031, -0.9760316137731011,// b0, b1, b2, a1, a2
    1, 0.6183391017701261, 1, -0.6651083890949688, -0.9764744111551448// b0, b1, b2, a1, a2

};


notch60hzType *notch60hz_create( void )
{
    notch60hzType *result = (notch60hzType *)malloc( sizeof( notch60hzType ) ); // Allocate memory for the object
    notch60hz_init( result );                                               // Initialize it
    return result;                                                        // Return the result
}

void notch60hz_destroy( notch60hzType *pObject )
{
    free( pObject );
}

void notch60hz_init( notch60hzType * pThis )
{
    notch60hz_reset( pThis );
}

void notch60hz_reset( notch60hzType * pThis )
{
    memset( &pThis->state, 0, sizeof( pThis->state ) ); // Reset state to 0
    pThis->output = 0;                                    // Reset output
}

int notch60hz_filterBlock( notch60hzType * pThis, float * pInput, float * pOutput, unsigned int count )
{
    notch60hz_executionState executionState;          // The executionState structure holds call data, minimizing stack reads and writes 
    if( ! count ) return 0;                         // If there are no input samples, return immediately
    executionState.pInput = pInput;                 // Pointers to the input and output buffers that each call to filterBiquad() will use
    executionState.pOutput = pOutput;               // - pInput and pOutput can be equal, allowing reuse of the same memory.
    executionState.count = count;                   // The number of samples to be processed
    executionState.pState = pThis->state;                   // Pointer to the biquad's internal state and coefficients. 
    executionState.pCoefficients = notch60hz_coefficients;    // Each call to filterBiquad() will advance pState and pCoefficients to the next biquad

    // The 1st call to filter1_filterBiquad() reads from the caller supplied input buffer and writes to the output buffer.
    // The remaining calls to filterBiquad() recycle the same output buffer, so that multiple intermediate buffers are not required.

    notch60hz_filterBiquad( &executionState );		// Run biquad #0
    executionState.pInput = executionState.pOutput;         // The remaining biquads will now re-use the same output buffer.

    notch60hz_filterBiquad( &executionState );		// Run biquad #1

    notch60hz_filterBiquad( &executionState );		// Run biquad #2

    notch60hz_filterBiquad( &executionState );		// Run biquad #3

    // At this point, the caller-supplied output buffer will contain the filtered samples and the input buffer will contain the unmodified input samples.  
    return count;		// Return the number of samples processed, the same as the number of input samples
}

void notch60hz_filterBiquad( notch60hz_executionState * pExecState )
{

    // Read state variables
    float x0;
    float x1 = pExecState->pState[0];
    float x2 = pExecState->pState[1];
    float y1 = pExecState->pState[2];
    float y2 = pExecState->pState[3];

    // Read coefficients into work registers
    float b0 = *(pExecState->pCoefficients++);
    float b1 = *(pExecState->pCoefficients++);
    float b2 = *(pExecState->pCoefficients++);
    float a1 = *(pExecState->pCoefficients++);
    float a2 = *(pExecState->pCoefficients++);

    // Read source and target pointers
    float *pInput  = pExecState->pInput;
    float *pOutput = pExecState->pOutput;
    short count = pExecState->count;
    float accumulator;

    while( count-- )
    {
        x0 = *(pInput++);

        accumulator  = x2 * b2;
        accumulator += x1 * b1;
        accumulator += x0 * b0;

        x2 = x1;        // Shuffle left history buffer
        x1 = x0;

        accumulator += y2 * a2;
        accumulator += y1 * a1;

        y2 = y1;        // Shuffle right history buffer
        y1 = accumulator ;

        *(pOutput++) = accumulator ;
    }

    *(pExecState->pState++) = x1;
    *(pExecState->pState++) = x2;
    *(pExecState->pState++) = y1;
    *(pExecState->pState++) = y2;
   
}



