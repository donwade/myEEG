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

// Begin header file, betaV2.h

#ifndef BETAV2_H_ // Include guards
#define BETAV2_H_

/*
Generated code is based on the following filter design:
<micro.DSP.FilterDocument sampleFrequency="#200" arithmetic="float" biquads="Direct1" classname="betaV2" inputMax="#1" inputShift="#-1" >
  <micro.DSP.IirButterworthFilter N="#4" bandType="b" w1="#0.065" w2="#0.15" stopbandRipple="#undefined" passbandRipple="#undefined" transitionRatio="#undefined" >
    <micro.DSP.FilterStructure coefficientBits="#0" variableBits="#0" accumulatorBits="#0" biquads="Direct1" >
      <micro.DSP.FilterSection form="Direct1" historyType="Double" accumulatorBits="#0" variableBits="#0" coefficientBits="#0" />
      <micro.DSP.FilterSection form="Direct1" historyType="Double" accumulatorBits="#0" variableBits="#0" coefficientBits="#0" />
      <micro.DSP.FilterSection form="Direct1" historyType="Double" accumulatorBits="#0" variableBits="#0" coefficientBits="#0" />
      <micro.DSP.FilterSection form="Direct1" historyType="Double" accumulatorBits="#0" variableBits="#0" coefficientBits="#0" />
    </micro.DSP.FilterStructure>
    <micro.DSP.PoleOrZeroContainer >
      <micro.DSP.PoleOrZero i="#0.3765782653704944" r="#0.8564585610016813" isPoint="#true" isPole="#true" isZero="#false" symmetry="c" N="#1" cascade="#3" />
      <micro.DSP.PoleOrZero i="#0.6950557414750713" r="#0.5355342171614924" isPoint="#true" isPole="#true" isZero="#false" symmetry="c" N="#1" cascade="#2" />
      <micro.DSP.PoleOrZero i="#0.373578292633052" r="#0.7159881681451697" isPoint="#true" isPole="#true" isZero="#false" symmetry="c" N="#1" cascade="#1" />
      <micro.DSP.PoleOrZero i="#0.48077834607659053" r="#0.5667883827029213" isPoint="#true" isPole="#true" isZero="#false" symmetry="c" N="#1" cascade="#0" />
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
    <micro.DSP.GainControl magnitude="#12" frequency="#0.099609375" peak="#true" />
  </micro.DSP.IirButterworthFilter>
</micro.DSP.FilterDocument>

*/

static const int betaV2_numStages = 4;
static const int betaV2_coefficientLength = 20;
extern float betaV2_coefficients[20];

typedef struct
{
	float state[16];
	float output;
} betaV2Type;

typedef struct
{
    float *pInput;
    float *pOutput;
    float *pState;
    float *pCoefficients;
    short count;
} betaV2_executionState;


betaV2Type *betaV2_create( void );
void betaV2_destroy( betaV2Type *pObject );
void betaV2_init( betaV2Type * pThis );
void betaV2_reset( betaV2Type * pThis );
#define betaV2_writeInput( pThis, input )  \
    betaV2_filterBlock( pThis, &(input), &(pThis)->output, 1 );

#define betaV2_readOutput( pThis )  \
    (pThis)->output

int betaV2_filterBlock( betaV2Type * pThis, float * pInput, float * pOutput, unsigned int count );
#define betaV2_outputToFloat( output )  \
    (output)

#define betaV2_inputFromFloat( input )  \
    (input)

void betaV2_filterBiquad( betaV2_executionState * pExecState );
#endif // BETAV2_H_
	

