#define FFT_N (1 << 9) //2048 // Must be a power of 2

#define SAMPLE_FREQHZ 200
#define SAMPLE_PERIOD_uS (1000000/SAMPLE_FREQHZ)

#define MS_PER_BIN ((float)FFT_N/(float)SAMPLE_FREQHZ)
#define FREQHZ_PER_BIN ((float)SAMPLE_FREQHZ/(float)FFT_N)

float fft_input[FFT_N];
float fft_output[FFT_N];

uint8_t recorderIndex = 0;
uint16_t recorder[FFT_N][8];

float max_magnitude = 0;
float fundamental_freq = 0;

