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

// Begin header file, gammaV2.h

#ifndef GAMMAV2_H_ // Include guards
#define GAMMAV2_H_

/*
Generated code is based on the following filter design:
<micro.DSP.FilterDocument sampleFrequency="#200" arithmetic="float" biquads="Direct1" classname="gammaV2" inputMax="#1" inputShift="#-1" >
  <micro.DSP.IirButterworthFilter N="#4" bandType="b" w1="#0.15" w2="#0.495" stopbandRipple="#undefined" passbandRipple="#undefined" transitionRatio="#undefined" >
    <micro.DSP.FilterStructure coefficientBits="#0" variableBits="#0" accumulatorBits="#0" biquads="Direct1" >
      <micro.DSP.FilterSection form="Direct1" historyType="Double" accumulatorBits="#0" variableBits="#0" coefficientBits="#0" />
      <micro.DSP.FilterSection form="Direct1" historyType="Double" accumulatorBits="#0" variableBits="#0" coefficientBits="#0" />
      <micro.DSP.FilterSection form="Direct1" historyType="Double" accumulatorBits="#0" variableBits="#0" coefficientBits="#0" />
      <micro.DSP.FilterSection form="Direct1" historyType="Double" accumulatorBits="#0" variableBits="#0" coefficientBits="#0" />
    </micro.DSP.FilterStructure>
    <micro.DSP.PoleOrZeroContainer >
      <micro.DSP.PoleOrZero i="#0.5745075906688707" r="#0.44899474537255685" isPoint="#true" isPole="#true" isZero="#false" symmetry="c" N="#1" cascade="#1" />
      <micro.DSP.PoleOrZero i="#0.02881141887169337" r="#-0.9877664322131494" isPoint="#true" isPole="#true" isZero="#false" symmetry="c" N="#1" cascade="#3" />
      <micro.DSP.PoleOrZero i="#0.180689444132579" r="#0.330371588569871" isPoint="#true" isPole="#true" isZero="#false" symmetry="c" N="#1" cascade="#0" />
      <micro.DSP.PoleOrZero i="#0.012006730696734235" r="#-0.9709935362312694" isPoint="#true" isPole="#true" isZero="#false" symmetry="c" N="#1" cascade="#2" />
      <micro.DSP.PoleOrZero i="#0" r="#1" isPoint="#true" isPole="#false" isZero="#true" symmetry="r" N="#1" cascade="#0" />
      <micro.DSP.PoleOrZero i="#0" r="#-1" isPoint="#true" isPole="#false" isZero="#true" symmetry="r" N="#1" cascade="#3" />
      <micro.DSP.PoleOrZero i="#0" r="#1" isPoint="#true" isPole="#false" isZero="#true" symmetry="r" N="#1" cascade="#0" />
      <micro.DSP.PoleOrZero i="#0" r="#-1" isPoint="#true" isPole="#false" isZero="#true" symmetry="r" N="#1" cascade="#3" />
      <micro.DSP.PoleOrZero i="#0" r="#1" isPoint="#true" isPole="#false" isZero="#true" symmetry="r" N="#1" cascade="#1" />
      <micro.DSP.PoleOrZero i="#0" r="#-1" isPoint="#true" isPole="#false" isZero="#true" symmetry="r" N="#1" cascade="#2" />
      <micro.DSP.PoleOrZero i="#0" r="#1" isPoint="#true" isPole="#false" isZero="#true" symmetry="r" N="#1" cascade="#1" />
      <micro.DSP.PoleOrZero i="#0" r="#-1" isPoint="#true" isPole="#false" isZero="#true" symmetry="r" N="#1" cascade="#2" />
    </micro.DSP.PoleOrZeroContainer>
    <micro.DSP.GenericC.CodeGenerator generateTestCases="#false" />
    <micro.DSP.GainControl magnitude="#12" frequency="#0.447265625" peak="#true" />
  </micro.DSP.IirButterworthFilter>
</micro.DSP.FilterDocument>

*/

static const int gammaV2_numStages = 4;
static const int gammaV2_coefficientLength = 20;
extern float gammaV2_coefficients[20];

typedef struct
{
	float state[16];
	float output;
} gammaV2Type;

typedef struct
{
    float *pInput;
    float *pOutput;
    float *pState;
    float *pCoefficients;
    short count;
} gammaV2_executionState;


gammaV2Type *gammaV2_create( void );
void gammaV2_destroy( gammaV2Type *pObject );
void gammaV2_init( gammaV2Type * pThis );
void gammaV2_reset( gammaV2Type * pThis );
#define gammaV2_writeInput( pThis, input )  \
    gammaV2_filterBlock( pThis, &(input), &(pThis)->output, 1 );

#define gammaV2_readOutput( pThis )  \
    (pThis)->output

int gammaV2_filterBlock( gammaV2Type * pThis, float * pInput, float * pOutput, unsigned int count );
#define gammaV2_outputToFloat( output )  \
    (output)

#define gammaV2_inputFromFloat( input )  \
    (input)

void gammaV2_filterBiquad( gammaV2_executionState * pExecState );
#endif // GAMMAV2_H_
	

