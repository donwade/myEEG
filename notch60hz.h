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

// Begin header file, notch60hz.h

#ifndef NOTCH60HZ_H_ // Include guards
#define NOTCH60HZ_H_

/*
Generated code is based on the following filter design:
<micro.DSP.FilterDocument sampleFrequency="#200" arithmetic="float" biquads="Direct1" classname="notch60hz" inputMax="#1" inputShift="#15" >
  <micro.DSP.IirButterworthFilter N="#4" bandType="s" w1="#0.295" w2="#0.305" stopbandRipple="#undefined" passbandRipple="#undefined" transitionRatio="#undefined" >
    <micro.DSP.FilterStructure coefficientBits="#0" variableBits="#0" accumulatorBits="#0" biquads="Direct1" >
      <micro.DSP.FilterSection form="Direct1" historyType="Double" accumulatorBits="#0" variableBits="#0" coefficientBits="#0" />
      <micro.DSP.FilterSection form="Direct1" historyType="Double" accumulatorBits="#0" variableBits="#0" coefficientBits="#0" />
      <micro.DSP.FilterSection form="Direct1" historyType="Double" accumulatorBits="#0" variableBits="#0" coefficientBits="#0" />
      <micro.DSP.FilterSection form="Direct1" historyType="Double" accumulatorBits="#0" variableBits="#0" coefficientBits="#0" />
    </micro.DSP.FilterStructure>
    <micro.DSP.PoleOrZeroContainer >
      <micro.DSP.PoleOrZero i="#0.9480422171769058" r="#-0.27793446749800155" isPoint="#true" isPole="#true" isZero="#false" symmetry="c" N="#1" cascade="#2" />
      <micro.DSP.PoleOrZero i="#0.9305278710731982" r="#-0.3325541945474844" isPoint="#true" isPole="#true" isZero="#false" symmetry="c" N="#1" cascade="#3" />
      <micro.DSP.PoleOrZero i="#0.9271980760110814" r="#-0.2892532712665106" isPoint="#true" isPole="#true" isZero="#false" symmetry="c" N="#1" cascade="#0" />
      <micro.DSP.PoleOrZero i="#0.9201764075448815" r="#-0.311557398295826" isPoint="#true" isPole="#true" isZero="#false" symmetry="c" N="#1" cascade="#1" />
      <micro.DSP.PoleOrZero i="#0.9510069341521797" r="#-0.30916955088506304" isPoint="#true" isPole="#false" isZero="#true" symmetry="c" N="#1" cascade="#1" />
      <micro.DSP.PoleOrZero i="#0.9510069341521797" r="#-0.30916955088506304" isPoint="#true" isPole="#false" isZero="#true" symmetry="c" N="#1" cascade="#0" />
      <micro.DSP.PoleOrZero i="#0.9510069341521797" r="#-0.30916955088506304" isPoint="#true" isPole="#false" isZero="#true" symmetry="c" N="#1" cascade="#3" />
      <micro.DSP.PoleOrZero i="#0.9510069341521797" r="#-0.30916955088506304" isPoint="#true" isPole="#false" isZero="#true" symmetry="c" N="#1" cascade="#2" />
    </micro.DSP.PoleOrZeroContainer>
    <micro.DSP.GenericC.CodeGenerator generateTestCases="#false" />
    <micro.DSP.GainControl magnitude="#6" frequency="#0.0087890625" peak="#true" />
  </micro.DSP.IirButterworthFilter>
</micro.DSP.FilterDocument>

*/

static const int notch60hz_numStages = 4;
static const int notch60hz_coefficientLength = 20;
extern float notch60hz_coefficients[20];

typedef struct
{
	float state[16];
	float output;
} notch60hzType;

typedef struct
{
    float *pInput;
    float *pOutput;
    float *pState;
    float *pCoefficients;
    short count;
} notch60hz_executionState;


notch60hzType *notch60hz_create( void );
void notch60hz_destroy( notch60hzType *pObject );
void notch60hz_init( notch60hzType * pThis );
void notch60hz_reset( notch60hzType * pThis );
#define notch60hz_writeInput( pThis, input )  \
    notch60hz_filterBlock( pThis, &(input), &(pThis)->output, 1 );

#define notch60hz_readOutput( pThis )  \
    (pThis)->output

int notch60hz_filterBlock( notch60hzType * pThis, float * pInput, float * pOutput, unsigned int count );
#define notch60hz_outputToFloat( output )  \
    (output)

#define notch60hz_inputFromFloat( input )  \
    (input)

void notch60hz_filterBiquad( notch60hz_executionState * pExecState );
#endif // NOTCH60HZ_H_
	

