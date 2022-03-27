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

// Begin header file, deltaV2.h

#ifndef DELTAV2_H_ // Include guards
#define DELTAV2_H_

/*
Generated code is based on the following filter design:
<micro.DSP.FilterDocument sampleFrequency="#200" arithmetic="float" biquads="Direct1" classname="deltaV2" inputMax="#1" inputShift="#-1" >
  <micro.DSP.IirButterworthFilter N="#4" bandType="b" w1="#0.005" w2="#0.015" stopbandRipple="#undefined" passbandRipple="#undefined" transitionRatio="#undefined" >
    <micro.DSP.FilterStructure coefficientBits="#0" variableBits="#0" accumulatorBits="#0" biquads="Direct1" >
      <micro.DSP.FilterSection form="Direct1" historyType="Double" accumulatorBits="#0" variableBits="#0" coefficientBits="#0" />
      <micro.DSP.FilterSection form="Direct1" historyType="Double" accumulatorBits="#0" variableBits="#0" coefficientBits="#0" />
      <micro.DSP.FilterSection form="Direct1" historyType="Double" accumulatorBits="#0" variableBits="#0" coefficientBits="#0" />
      <micro.DSP.FilterSection form="Direct1" historyType="Double" accumulatorBits="#0" variableBits="#0" coefficientBits="#0" />
    </micro.DSP.FilterStructure>
    <micro.DSP.PoleOrZeroContainer >
      <micro.DSP.PoleOrZero i="#0.03153102798634682" r="#0.9932378184751205" isPoint="#true" isPole="#true" isZero="#false" symmetry="c" N="#1" cascade="#3" />
      <micro.DSP.PoleOrZero i="#0.08809680223366054" r="#0.9784466300006545" isPoint="#true" isPole="#true" isZero="#false" symmetry="c" N="#1" cascade="#2" />
      <micro.DSP.PoleOrZero i="#0.03531194813830079" r="#0.9778139881126181" isPoint="#true" isPole="#true" isZero="#false" symmetry="c" N="#1" cascade="#1" />
      <micro.DSP.PoleOrZero i="#0.057989706756623224" r="#0.9626136189870095" isPoint="#true" isPole="#true" isZero="#false" symmetry="c" N="#1" cascade="#0" />
      <micro.DSP.PoleOrZero i="#0" r="#1" isPoint="#true" isPole="#false" isZero="#true" symmetry="r" N="#1" cascade="#3" />
      <micro.DSP.PoleOrZero i="#0" r="#-1" isPoint="#true" isPole="#false" isZero="#true" symmetry="r" N="#1" cascade="#0" />
      <micro.DSP.PoleOrZero i="#0" r="#1" isPoint="#true" isPole="#false" isZero="#true" symmetry="r" N="#1" cascade="#3" />
      <micro.DSP.PoleOrZero i="#0" r="#-1" isPoint="#true" isPole="#false" isZero="#true" symmetry="r" N="#1" cascade="#0" />
      <micro.DSP.PoleOrZero i="#0" r="#1" isPoint="#true" isPole="#false" isZero="#true" symmetry="r" N="#1" cascade="#1" />
      <micro.DSP.PoleOrZero i="#0" r="#-1" isPoint="#true" isPole="#false" isZero="#true" symmetry="r" N="#1" cascade="#2" />
      <micro.DSP.PoleOrZero i="#0" r="#1" isPoint="#true" isPole="#false" isZero="#true" symmetry="r" N="#1" cascade="#1" />
      <micro.DSP.PoleOrZero i="#0" r="#-1" isPoint="#true" isPole="#false" isZero="#true" symmetry="r" N="#1" cascade="#2" />
    </micro.DSP.PoleOrZeroContainer>
    <micro.DSP.GenericC.CodeGenerator generateTestCases="#false" />
    <micro.DSP.GainControl magnitude="#12" frequency="#0.0087890625" peak="#true" />
  </micro.DSP.IirButterworthFilter>
</micro.DSP.FilterDocument>

*/

static const int deltaV2_numStages = 4;
static const int deltaV2_coefficientLength = 20;
extern float deltaV2_coefficients[20];

typedef struct
{
	float state[16];
	float output;
} deltaV2Type;

typedef struct
{
    float *pInput;
    float *pOutput;
    float *pState;
    float *pCoefficients;
    short count;
} deltaV2_executionState;


deltaV2Type *deltaV2_create( void );
void deltaV2_destroy( deltaV2Type *pObject );
void deltaV2_init( deltaV2Type * pThis );
void deltaV2_reset( deltaV2Type * pThis );
#define deltaV2_writeInput( pThis, input )  \
    deltaV2_filterBlock( pThis, &(input), &(pThis)->output, 1 );

#define deltaV2_readOutput( pThis )  \
    (pThis)->output

int deltaV2_filterBlock( deltaV2Type * pThis, float * pInput, float * pOutput, unsigned int count );
#define deltaV2_outputToFloat( output )  \
    (output)

#define deltaV2_inputFromFloat( input )  \
    (input)

void deltaV2_filterBiquad( deltaV2_executionState * pExecState );
#endif // DELTAV2_H_
	

