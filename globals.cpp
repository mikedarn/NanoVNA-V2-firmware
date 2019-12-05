#include "globals.hpp"
#include "ili9341.hpp"

int8_t sweep_enabled;
int16_t vbat;

uint8_t registers[32];

int16_t lastsaveid = 0;

config_t config = {
  .magic =             CONFIG_MAGIC,
  .dac_value =         1922,
  .grid_color =        0x1084,
  .menu_normal_color = 0xffff,
  .menu_active_color = 0x7777,
  .trace_color =       { RGB565(0,255,255), RGB565(255,0,40), RGB565(0,0,255), RGB565(50,255,0) },
  .touch_cal =         { 693, 605, 124, 171 },  //{ 620, 600, 160, 190 },
  .default_loadcal =   0,
  .harmonic_freq_threshold = 300000000,
  .checksum =          0
};

properties_t current_props = {
  /* magic */   CONFIG_MAGIC,
  /* frequency0 */ 100000000, // start = 100MHz
  /* frequency1 */ 900000000, // end = 900MHz
  /* sweep_points */     101,
  /* cal_status */         0,
  /* cal_data */          {},
  /* electrical_delay */   0,
  /* trace[4] */
  {/*enable, type, channel, polar, scale, refpos*/
    { 1, TRC_LOGMAG, 0, 0, 1.0, 7.0 },
    { 1, TRC_LOGMAG, 1, 0, 1.0, 7.0 },
    { 1, TRC_SMITH,  0, 1, 1.0, 0.0 },
    { 1, TRC_PHASE,  0, 0, 1.0, 4.0 }
  },
  /* markers[4] */ {
    { 1, 30, 0 }, { 0, 40, 0 }, { 0, 60, 0 }, { 0, 80, 0 }
  },
  /* active_marker */      0,
  /* domain_mode */        0,
  /* velocity_factor */   70,
  /* checksum */           0
};


properties_t* active_props = &current_props;


complexf measured[2][SWEEP_POINTS_MAX];
complexf measuredEcal[3][SWEEP_POINTS_MAX];
volatile bool ecalComplete = false;

uistat_t uistat = {
 digit: 6,
 digit_mode: 0,
 current_trace: 0,
 value: 0,
 previous_value: 0
};

