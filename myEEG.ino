
#include "FFT.h" // include the library
#include "FFT_signal.h"


/*
https://deepbluembedded.com/esp32-adc-tutorial-read-analog-voltage-arduino/

The ESP32 has 2 x 12-Bit SAR (Successive Approximation Register) ADC hardware peripherals that can read 18 different analog input channels.

The ADC firmware driver API supports ADC1 (8 channels, attached to GPIOs 32 – 39), and ADC2 (10 channels, attached to GPIOs 0, 2, 4, 12 – 15, and 25 – 27). However, the usage of ADC2 has some restrictions for the application:

    ADC2 is used by the Wi-Fi driver. Therefore the application can only use ADC2 when the Wi-Fi driver has not been started.
    Some of the ADC2 pins are used as strapping pins (GPIO 0, 2, 15) thus cannot be used freely.

*/

#define GREEN_LED       25 // TTGO GPIO25
#define ADC_PIN         34 // TTGO GPIO34 = adc1_ch6
#define CANTUSE_DAC1    26
#define DAC2_PIN        25

#define INTER_LINE_SPACING 1000

#define CONFIG_FOR_FILTERS 0 // 0=fft 1 = classic filter

typedef struct
{
    int16_t data;
    unsigned long timeUs;
}A2D_t;

static QueueHandle_t queue4A2D = NULL;  // rfm data pin

hw_timer_t * hSilenceTimer = NULL;
volatile SemaphoreHandle_t semBlockDataReady;

//-------------------------------------------------------------------------
#define SINE_TABLE_ENTRIES 1024
#define SINE_AMPLITUDE  1000
int16_t sinTable[SINE_TABLE_ENTRIES];

// a sine wave has no frequency, only resolution
void genSineWave() {

    srand((unsigned)time(NULL));

    int i;

    for(i = 0; i < SINE_TABLE_ENTRIES; i++)
    {
        double temp = sin((2.0 * M_PI) * (float)i / (float)SINE_TABLE_ENTRIES);
        sinTable[i] = (double)SINE_AMPLITUDE * temp;// + sqrt(0.01) * rand();
    }

/*
    for(i = 0; i < SINE_TABLE_ENTRIES; i++)
    {
        Serial.printf("%d  %d\n", i, sinTable[i]);
    }
*/

}


int16_t getSineWave(uint64_t timeUs, double frequency)
{
    double uSPerCycle = 1000000.0 / frequency;
    double whereInCycle = timeUs / uSPerCycle;
    whereInCycle = whereInCycle - (double)((uint64_t) whereInCycle); // get fractional part
    uint32_t whereInTable = whereInCycle * SINE_TABLE_ENTRIES;
    int16_t  value = sinTable[whereInTable];

    // no print when called inside isr :P
    //Serial.printf("%s : timeUs=%llu uSPerCycle=%f percent=%f whereInTable=%4d value=%d\n",
    //    __FUNCTION__, timeUs, uSPerCycle, whereInCycle, whereInTable, value);
    return value;
}
//-------------------------------------------------------------------------

void signalRunFFT()
{
    static BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xSemaphoreGiveFromISR(semBlockDataReady, &xHigherPriorityTaskWoken);
    if( xHigherPriorityTaskWoken == pdTRUE)
    {
        // this wakes up waiting immediately instead of on next FreeRTOS tick
        portYIELD_FROM_ISR();
    }
}


int16_t isrA2Dbuffer[FFT_N];
int16_t taskA2Dbuffer[FFT_N];

static volatile bool bIsrUsingLeftBuffer = 0;

//-------------------------------------------------------------------------
// note: if data is shared b/n isr and task, this is how you do it
//portMUX_TYPE criticalSection = portMUX_INITIALIZER_UNLOCKED;
//portENTER_CRITICAL_ISR(&criticalSection);
//portEXIT_CRITICAL_ISR(&criticalSection);

A2D_t a2dValue;

void IRAM_ATTR IRQTimerFn()
{
    // auto reload is set. No need to reset cnt h/w register.

    volatile uint16_t timerCntISR;
    uint16_t a2d;
    static uint64_t timePassed;

#ifndef CONFIG_FOR_FILTERS
    #error "Need to define CONFIG_FOR_FILTERS"
#endif


#if 0
    int16_t fake;
    timePassed += SAMPLE_PERIOD_uS;
    //fake = getSineWave(timePassed,2.344); //N hz delta,
    //fake = getSineWave(timePassed,4.2);   //N hz theta,
    //fake = getSineWave(timePassed,9.1);   //N hz alpha,
    //fake = getSineWave(timePassed,40.1);   //N hz gamma,
    fake = getSineWave(timePassed,5.1);    //N hz theta,
    a2d =fake;
#else
    a2d = analogRead(ADC_PIN);
#endif

#if CONFIG_FOR_FILTERS
    a2dValue.timeUs = micros();
    a2dValue.data = a2d;

    // FILTER mode
    if (queue4A2D) xQueueSendToBackFromISR(queue4A2D, &a2dValue, NULL);

#else
    timerCntISR++;
    if (timerCntISR == FFT_N)
    {
        timerCntISR = 0;

        memcpy(taskA2Dbuffer, isrA2Dbuffer, sizeof(taskA2Dbuffer));

        signalRunFFT();
    }
    isrA2Dbuffer[timerCntISR] = a2d;

#endif
}
//-------------------------------------------------------------------------
void setupIRQTimer()
{

    hSilenceTimer = timerBegin(0, 80, true);  // 80Mhz/80 = 1Mhz tick rate
    timerAttachInterrupt(hSilenceTimer, &IRQTimerFn, true);

    // Sets an alarm to sound every X mS
    //timerAlarmWrite(hSilenceTimer, A2D_SamplePeriodUs, true);// auto reload
    timerAlarmWrite(hSilenceTimer, SAMPLE_PERIOD_uS, true);// auto reload
    //Serial.printf("Sample period in uS = %d\n", SAMPLE_PERIOD_uS);
    // wait til ready.
    timerAlarmEnable(hSilenceTimer);

}
//-------------------------------------------------------------------------

extern "C" {
#include "alphaV2.h"
#include "betaV2.h"
#include "deltaV2.h"
#include "gammaV2.h"
#include "thetaV2.h"
#include "notch60hz.h"
 }

alphaV2Type *afilterV2;
betaV2Type  *bfilterV2;
deltaV2Type *dfilterV2;
gammaV2Type *gfilterV2;
thetaV2Type *tfilterV2;
notch60hzType *nfilterV2;

void setup()
{
  Serial.begin(115200); // use the serial port

  pinMode(GREEN_LED,OUTPUT);

  //analogSetPinAttenuation(ADC_PIN, ADC_11db); // (default anyhow) max input range 0->2.5V
  //analogSetWidth(11);  // (default anyhow) 11bit = 4096

  semBlockDataReady = xSemaphoreCreateBinary();

  //pinMode(CANTUSE_DAC1, OUTPUT);
  pinMode(DAC2_PIN, OUTPUT);


  genSineWave();

/*
  for (uint64_t i = 0; i < 2000000; i+= SAMPLE_PERIOD_uS) // for 2M us, step 1mS
  {
      int16_t wave = getSineWave(i,3); // frequency is second parameter
  }
  while(1) delay(1000); //////////sssssssssssssssssssssssss///////////////////
*/

  int samplesProcessed;

  afilterV2 = alphaV2_create(); // Create an instance of the filter
  bfilterV2 = betaV2_create();  // Create an instance of the filter
  dfilterV2 = deltaV2_create(); // Create an instance of the filter
  gfilterV2 = gammaV2_create(); // Create an instance of the filter
  tfilterV2 = thetaV2_create(); // Create an instance of the filter
  nfilterV2 = notch60hz_create();

  alphaV2_reset(afilterV2);
  betaV2_reset (bfilterV2);
  deltaV2_reset(dfilterV2);
  gammaV2_reset(gfilterV2);
  thetaV2_reset(tfilterV2);
  notch60hz_reset(nfilterV2);


/*
  printf( "\n\nRunning tests for: delta\n" );

  printf( "\nimpulse test\n" );
  deltaV2_reset( filter );
  samplesProcessed = deltaV2_filterInChunks( filter, delta_impulseInput, delta_testOutput, 63 );        // Filter the input test signal
*/


  setupIRQTimer();  // interrupts now running!!!!


#if CONFIG_FOR_FILTERS

  queue4A2D = xQueueCreate(200, sizeof(A2D_t));
  xTaskCreatePinnedToCore(
                  task_FILTERS,       /* Function to implement the task */
                  "task_FILTERS",     /* Name of the task */
                  50000,          /* Stack size in words */
                  (void *)99,     /* Task input parameter */
                  0,              /* Priority of the task */
                  NULL,           /* Task handle. */
                  0);             /* Core where the task should run */

#else
#if 0
    xTaskCreatePinnedToCore(
                    task_FFT,       /* Function to implement the task */
                    "task_FFT",     /* Name of the task */
                    10000,          /* Stack size in words */
                    (void *)99,     /* Task input parameter */
                    0,              /* Priority of the task */
                    NULL,           /* Task handle. */
                    1);             /* Core where the task should run */
#else
xTaskCreatePinnedToCore(
                task_FILTERS,       /* Function to implement the task */
                "task_FILTERS",     /* Name of the task */
                50000,          /* Stack size in words */
                (void *)99,     /* Task input parameter */
                0,              /* Priority of the task */
                NULL,           /* Task handle. */
                0);             /* Core where the task should run */
#endif

#endif

}
//------------------------------------------------------------------------------------------
#define VERT_SCALE   100
#define SPACING (VERT_SCALE*2)
#define A2D_HISTORY (1<< 5)

void task_FILTERS( void *parameter)
{
    char print_buf[300];
    unsigned long oldArrival = 0;

    float dataIn[FFT_N];

    float alphaOut[FFT_N];
    float betaOut[FFT_N];
    float deltaOut[FFT_N];
    float gammaOut[FFT_N];
    float thetaOut[FFT_N];
    float n60hzOut[FFT_N];

    Serial.printf("%s running\n", __FUNCTION__);
    while (true)
    {
        unsigned long slackTime = micros();
        while (xSemaphoreTake( semBlockDataReady, portMAX_DELAY ) != pdTRUE );
        {
            unsigned long start = micros();
            uint32_t avg = 0;
            int32_t  mean = 0;
            static bool bFlip;

            digitalWrite(GREEN_LED, bFlip);
            bFlip = !bFlip;

            slackTime = start - slackTime;

            // convert int stream to floats
            for (int k = 0 ; k < FFT_N ; k++)
            {
                //if (!(tab++ % 8)) Serial.println();
                dataIn[k] = (float) taskA2Dbuffer[k];
                avg += taskA2Dbuffer[k];
                //Serial.printf("0x%04X ", taskA2DBuffer[k]);
            }
            //Serial.println();
            avg /= FFT_N;

            for (int k = 0 ; k < FFT_N ; k++)
            {
                mean += (avg - taskA2Dbuffer[k]) * (avg - taskA2Dbuffer[k]);
            }

            mean = sqrt((double) mean / (double)FFT_N);

            unsigned long end = micros();

            int notchRet= notch60hz_filterBlock(nfilterV2, dataIn,n60hzOut,FFT_N);
 //                                                          input     output
            int alphaRet= alphaV2_filterBlock( afilterV2, n60hzOut, alphaOut, FFT_N );
            int betaRet=  betaV2_filterBlock ( bfilterV2, n60hzOut, betaOut, FFT_N );
            int deltaRet= deltaV2_filterBlock( dfilterV2, n60hzOut, deltaOut, FFT_N );
            int gammaRet= gammaV2_filterBlock( gfilterV2, n60hzOut, gammaOut, FFT_N );
            int thetaRet= thetaV2_filterBlock( tfilterV2, n60hzOut, thetaOut, FFT_N );

#if 0
            for (int k = 1 ; k < FFT_N ; k++)
            {
              Serial.printf("%6d %6d %6d %6d \n"
                    ,taskA2Dbuffer[k]
                    ,  avg
                    ,  avg+mean
                    ,  avg-mean
              );
            }

#else
            #define VSCALE 10
            if (mean < 250)  // if intermittent connection the mean is large.
            {
                for (int k = 0 ; k < FFT_N ; k++)
                {
                  Serial.printf("%6d %6d %6d %6d %6d %6d %6d %6d %d\n" ,
                        (int32_t) alphaOut[k] / VSCALE      + INTER_LINE_SPACING * 1
                        ,(int32_t)  betaOut[k]/ VSCALE     + INTER_LINE_SPACING * 2
                        ,(int32_t) deltaOut[k]/ VSCALE     + INTER_LINE_SPACING * 3
                        ,(int32_t) gammaOut[k]*3/8 / VSCALE + INTER_LINE_SPACING * 4

                        ,(int32_t) thetaOut[k]/ VSCALE     + INTER_LINE_SPACING * 5
                        ,taskA2Dbuffer[k]     //    + INTER_LINE_SPACING * 6
                        ,  avg                  //   + INTER_LINE_SPACING * 6
                        ,  avg+mean             //   + INTER_LINE_SPACING * 6
                        ,  avg-mean             //   + INTER_LINE_SPACING * 6
                        );
                }
            }
#endif

#if 0
            Serial.print("Compute Time: ");Serial.print((end-start)*1.0/1000);Serial.println(" milliseconds!");
            Serial.print("Slack   Time: ");Serial.print((slackTime)*1.0/1000);Serial.println(" milliseconds!");

            Serial.printf("SAMPLE RATE = %d hz %f mS\n", SAMPLE_FREQHZ, 1000./SAMPLE_FREQHZ);
            Serial.printf("hz/bin = %f\n", FREQHZ_PER_BIN);
            Serial.printf("buffer = %p\n\-------------------------\n", taskA2Dbuffer);
#endif

        }
    }
    delay(3000);
}

//------------------------------------------------------------------------------------------
void task_FFT( void *parameter)
{
    char print_buf[300];
    unsigned long oldArrival = 0;

    Serial.printf("%s running\n", __FUNCTION__);
    while (true)
    {
        unsigned long slackTime = micros();
        while (xSemaphoreTake( semBlockDataReady, portMAX_DELAY ) != pdTRUE );
        {
          unsigned long t1 = micros();

          static bool bFlip;
          digitalWrite(GREEN_LED, bFlip);
          bFlip = !bFlip;

          slackTime = t1 - slackTime;

          fft_config_t *real_fft_plan = fft_init(FFT_N, FFT_REAL, FFT_FORWARD, fft_input, fft_output);

          // copy uint ram stream into float fft input buffer.
          uint16_t ramp=0;
          uint8_t tab = 0;
          for (int k = 0 ; k < FFT_N ; k++)
          {
              //if (!(tab++ % 8)) Serial.println();
              real_fft_plan->input[k] = (float)taskA2Dbuffer[k];
              //Serial.printf("0x%04X ", taskA2DBuffer[k]);
          }

          //Serial.println();

          // Execute transformation
          fft_execute(real_fft_plan);
          unsigned long t2 = micros();

          max_magnitude = 0.0;

          // Print the output
          char *type;
          type = "?????";

          for (int k = 1 ; k < real_fft_plan->size / 2 ; k++)
          {
            /*The real part of a magnitude at a frequency is followed by the corresponding imaginary part in the output*/
            float mag = sqrt(pow(real_fft_plan->output[2*k],2) + pow(real_fft_plan->output[2*k+1],2))/1;
            float freq = k * FREQHZ_PER_BIN;



            recorder[k][recorderIndex] = mag;

            if (freq >= 0. && freq <= 4.)
                type = "DELTA";
            else if (freq >= 4. && freq <= 7.)
                type = "THETA";
            else if (freq >= 8 && freq <= 12.)
                type = "ALPHA";
            else if (freq >= 12 && freq <= 30.)
                type = "BETA ";
            else if (freq >= 30. && freq <= 100.)
                type = "GAMMA";

            //sprintf(print_buf,"%s %4d %7.3f Hz : %8d", type , k, freq, (uint32_t) mag);
            sprintf(print_buf,"%s %4d %7.3f Hz : %6d %6d %6d %6d %6d %6d %6d %6d",
                type , k, freq,
                recorder[k][0],recorder[k][1],recorder[k][2],recorder[k][3],
                recorder[k][4],recorder[k][5],recorder[k][6],recorder[k][7]
                );
            Serial.println(print_buf);

            // find peak bin
            if(mag > max_magnitude)
            {
                max_magnitude = mag;
                fundamental_freq = freq;
            }
          }

          recorderIndex++; recorderIndex &= 0x7;

          Serial.println();
          /*Multiply the magnitude of the DC component with (1/FFT_N) to obtain the DC component*/
          //sprintf(print_buf,"DC component : %f g\n", (real_fft_plan->output[0])/10000/FFT_N);  // DC is at [0]
          sprintf(print_buf,"DC component : %f g\n", (real_fft_plan->output[0])/FFT_N);  // DC is at [0]
          Serial.println(print_buf);

          /*Multiply the magnitude at all other frequencies with (2/FFT_N) to obtain the amplitude at that frequency*/
          sprintf(print_buf,"Fundamental Freq : %f Hz\t Mag: %f g\n", fundamental_freq, max_magnitude); //(max_magnitude*2/FFT_N));
          Serial.println(print_buf);

          Serial.print("Compute Time: ");Serial.print((t2-t1)*1.0/1000);Serial.println(" milliseconds!");
          Serial.print("Slack   Time: ");Serial.print((slackTime)*1.0/1000);Serial.println(" milliseconds!");

          // Clean up at the end to free the memory allocated
          fft_destroy(real_fft_plan);

          Serial.printf("SAMPLE RATE = %d hz %f mS\n", SAMPLE_FREQHZ, 1000./SAMPLE_FREQHZ);
          Serial.printf("hz/bin = %f\n", FREQHZ_PER_BIN);
          Serial.printf("buffer = %p\n\-------------------------\n", taskA2Dbuffer);
        }
    }
    delay(3000);
}
//---------------------------------------------------------------------------

void loop()
{
    // only needed for watchdog, allow all tasks to run.
    delay(100);// vTaskSuspend(NULL);
}
