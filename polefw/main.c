#include "ch.h"
#include "hal.h"

static PWMConfig pwmcfg = {
    10000,
    100,
    NULL,
    {
        {PWM_OUTPUT_ACTIVE_HIGH, NULL},
        {PWM_OUTPUT_ACTIVE_HIGH, NULL},
        {PWM_OUTPUT_ACTIVE_HIGH, NULL},
        {PWM_OUTPUT_DISABLED, NULL},
    },
    0,
    0
};

static THD_WORKING_AREA(waThreadLEDs, 512);
static THD_FUNCTION(threadLEDs, arg) {
    (void)arg;
    chRegSetThreadName("LEDs");
    while (true) {
        int ch;
        for(ch=0; ch<3; ch++) {
            int duty;
            for(duty=0; duty<100; duty++) {
                pwmEnableChannel(&PWMD1, ch, duty);
                chThdSleepMilliseconds(10);
            }
            for(duty=100; duty>0; duty--) {
                pwmEnableChannel(&PWMD1, ch, duty);
                chThdSleepMilliseconds(10);
            }
        }
    }
}

static volatile uint8_t rxbuf[32];
static volatile unsigned int rxbufptr = 0;

static void rxchar(UARTDriver *uartp, uint16_t c)
{
    (void)uartp;
    (void)c;
    rxbuf[rxbufptr++] = (uint8_t)c;
    if(rxbufptr >= sizeof(rxbuf)) {
        rxbufptr = 0;
    };
}

static UARTConfig uartcfg = {
    txend1_cb: NULL,
    txend2_cb: NULL,
    rxend_cb: NULL,
    rxchar_cb: rxchar,
    rxerr_cb: NULL,
    speed: 115200,
    cr1: 0,
    /*cr2: USART_CR2_RXINV,*/
    cr2: 0,
    cr3: 0
};

static THD_WORKING_AREA(waThreadUART, 512);
static THD_FUNCTION(threadUART, arg) {
    (void)arg;
    chRegSetThreadName("UART");
    while (true) {

    }
}

#define DAC_BUFFER_SIZE 360
static const dacsample_t dac_buffer[DAC_BUFFER_SIZE] = {
  2047, 2082, 2118, 2154, 2189, 2225, 2260, 2296, 2331, 2367, 2402, 2437,
  2472, 2507, 2542, 2576, 2611, 2645, 2679, 2713, 2747, 2780, 2813, 2846,
  2879, 2912, 2944, 2976, 3008, 3039, 3070, 3101, 3131, 3161, 3191, 3221,
  3250, 3278, 3307, 3335, 3362, 3389, 3416, 3443, 3468, 3494, 3519, 3544,
  3568, 3591, 3615, 3637, 3660, 3681, 3703, 3723, 3744, 3763, 3782, 3801,
  3819, 3837, 3854, 3870, 3886, 3902, 3917, 3931, 3944, 3958, 3970, 3982,
  3993, 4004, 4014, 4024, 4033, 4041, 4049, 4056, 4062, 4068, 4074, 4078,
  4082, 4086, 4089, 4091, 4092, 4093, 4094, 4093, 4092, 4091, 4089, 4086,
  4082, 4078, 4074, 4068, 4062, 4056, 4049, 4041, 4033, 4024, 4014, 4004,
  3993, 3982, 3970, 3958, 3944, 3931, 3917, 3902, 3886, 3870, 3854, 3837,
  3819, 3801, 3782, 3763, 3744, 3723, 3703, 3681, 3660, 3637, 3615, 3591,
  3568, 3544, 3519, 3494, 3468, 3443, 3416, 3389, 3362, 3335, 3307, 3278,
  3250, 3221, 3191, 3161, 3131, 3101, 3070, 3039, 3008, 2976, 2944, 2912,
  2879, 2846, 2813, 2780, 2747, 2713, 2679, 2645, 2611, 2576, 2542, 2507,
  2472, 2437, 2402, 2367, 2331, 2296, 2260, 2225, 2189, 2154, 2118, 2082,
  2047, 2012, 1976, 1940, 1905, 1869, 1834, 1798, 1763, 1727, 1692, 1657,
  1622, 1587, 1552, 1518, 1483, 1449, 1415, 1381, 1347, 1314, 1281, 1248,
  1215, 1182, 1150, 1118, 1086, 1055, 1024,  993,  963,  933,  903,  873,
   844,  816,  787,  759,  732,  705,  678,  651,  626,  600,  575,  550,
   526,  503,  479,  457,  434,  413,  391,  371,  350,  331,  312,  293,
   275,  257,  240,  224,  208,  192,  177,  163,  150,  136,  124,  112,
   101,   90,   80,   70,   61,   53,   45,   38,   32,   26,   20,   16,
    12,    8,    5,    3,    2,    1,    0,    1,    2,    3,    5,    8,
    12,   16,   20,   26,   32,   38,   45,   53,   61,   70,   80,   90,
   101,  112,  124,  136,  150,  163,  177,  192,  208,  224,  240,  257,
   275,  293,  312,  331,  350,  371,  391,  413,  434,  457,  479,  503,
   526,  550,  575,  600,  626,  651,  678,  705,  732,  759,  787,  816,
   844,  873,  903,  933,  963,  993, 1024, 1055, 1086, 1118, 1150, 1182,
  1215, 1248, 1281, 1314, 1347, 1381, 1415, 1449, 1483, 1518, 1552, 1587,
  1622, 1657, 1692, 1727, 1763, 1798, 1834, 1869, 1905, 1940, 1976, 2012
};

size_t nx = 0, ny = 0, nz = 0;
static void end_cb1(DACDriver *dacp, const dacsample_t *buffer, size_t n) {

  (void)dacp;

  nz++;
  if (dac_buffer == buffer) {
    nx += n;
  }
  else {
    ny += n;
  }
}

static void error_cb1(DACDriver *dacp, dacerror_t err) {

  (void)dacp;
  (void)err;

  chSysHalt("DAC failure");
}

static const DACConfig daccfg = {
  .init         = 2047U,
  .datamode     = DAC_DHRM_12BIT_RIGHT
};

static const DACConversionGroup dacgrpcfg1 = {
  .num_channels = 1U,
  .end_cb       = end_cb1,
  .error_cb     = error_cb1,
  .trigger      = DAC_TRG(0)
};

static const GPTConfig gpt6cfg = {
  .frequency    = 1500000U,
  .callback     = NULL,
  .cr2          = TIM_CR2_MMS_1,    /* MMS = 010 = TRGO on Update Event.    */
  .dier         = 0U
};

int main(void) {

    halInit();
    chSysInit();

    pwmStart(&PWMD1, &pwmcfg);
    uartStart(&UARTD2, &uartcfg);

    dacStart(&DACD1, &daccfg);
    gptStart(&GPTD6, &gpt6cfg);

    dacStartConversion(&DACD1, &dacgrpcfg1, dac_buffer, DAC_BUFFER_SIZE);
    gptStartContinuous(&GPTD6, 2U);

    chThdCreateStatic(waThreadLEDs, sizeof(waThreadLEDs),
                      NORMALPRIO, threadLEDs, NULL);

    chThdCreateStatic(waThreadUART, sizeof(waThreadUART),
                      NORMALPRIO, threadUART, NULL);

    while (true) {
    }
}