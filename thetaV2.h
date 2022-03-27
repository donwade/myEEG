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

// Begin header file, thetaV2.h

#ifndef THETAV2_H_ // Include guards
#define THETAV2_H_

/*
Generated code is based on the following filter design:
<micro.DSP.FilterDocument sampleFrequency="#200" arithmetic="float" biquads="Direct1" classname="thetaV2" inputMax="#1" inputShift="#-1" >
  <micro.DSP.IirButterworthFilter N="#4" bandType="b" w1="#0.02" w2="#0.035" stopbandRipple="#undefined" passbandRipple="#undefined" transitionRatio="#undefined" >
    <micro.DSP.FilterStructure coefficientBits="#0" variableBits="#0" accumulatorBits="#0" biquads="Direct1" >
      <micro.DSP.FilterSection form="Direct1" historyType="Double" accumulatorBits="#0" variableBits="#0" coefficientBits="#0" />
      <micro.DSP.FilterSection form="Direct1" historyType="Double" accumulatorBits="#0" variableBits="#0" coefficientBits="#0" />
      <micro.DSP.FilterSection form="Direct1" historyType="Double" accumulatorBits="#0" variableBits="#0" coefficientBits="#0" />
      <micro.DSP.FilterSection form="Direct1" historyType="Double" accumulatorBits="#0" variableBits="#0" coefficientBits="#0" />
    </micro.DSP.FilterStructure>
    <micro.DSP.PoleOrZeroContainer >
      <micro.DSP.PoleOrZero i="#0.12537964921324746" r="#0.9786233039439437" isPoint="#true" isPole="#true" isZero="#false" symmetry="c" N="#1" cascade="#3" />
      <micro.DSP.PoleOrZero i="#0.20814425674803932" r="#0.9552780821027121" isPoint="#true" isPole="#true" isZero="#false" symmetry="c" N="#1" cascade="#2" />
      <micro.DSP.PoleOrZero i="#0.1376099774158781" r="#0.9520958605739699" isPoint="#true" isPole="#true" isZero="#false" symmetry="c" N="#1" cascade="#1" />
      <micro.DSP.PoleOrZero i="#0.17028583562700356" r="#0.9373992037033613" isPoint="#true" isPole="#true" isZero="#false" symmetry="c" N="#1" cascade="#0" />
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
    <micro.DSP.GainControl magnitude="#12" frequency="#0.0263671875" peak="#true" />
  </micro.DSP.IirButterworthFilter>
</micro.DSP.FilterDocument>

*/

static const int thetaV2_numStages = 4;
static const int thetaV2_coefficientLength = 20;
extern float thetaV2_coefficients[20];

typedef struct
{
	float state[16];
	float output;
} thetaV2Type;

typedef struct
{
    float *pInput;
    float *pOutput;
    float *pState;
    float *pCoefficients;
    short count;
} thetaV2_executionState;


thetaV2Type *thetaV2_create( void );
void thetaV2_destroy( thetaV2Type *pObject );
void thetaV2_init( thetaV2Type * pThis );
void thetaV2_reset( thetaV2Type * pThis );
#define thetaV2_writeInput( pThis, input )  \
    thetaV2_filterBlock( pThis, &(input), &(pThis)->output, 1 );

#define thetaV2_readOutput( pThis )  \
    (pThis)->output

int thetaV2_filterBlock( thetaV2Type * pThis, float * pInput, float * pOutput, unsigned int count );
#define thetaV2_outputToFloat( output )  \
    (output)

#define thetaV2_inputFromFloat( input )  \
    (input)

void thetaV2_filterBiquad( thetaV2_executionState * pExecState );
#endif // THETAV2_H_
	

