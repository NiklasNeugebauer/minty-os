#ifndef MINTY_OS_CONF
#define MINTY_OS_CONF

#define MINTY_OS_VERSION_NUM "0.8"

#define ARDUINO_WATCHY_V10
#define NTP_SERVER "pool.ntp.org"

#define TICK_LENGTH_MS 50
#define HOLD_TICKS 10
#define SCREEN_ACTIVE_TICKS 15

struct configuration {
    // location
    short gmt_offset_sec = 3600 * 2;
    // night time mode
    bool nighttime = false;
    char night_begin_hour = 23;
    char night_end_hour = 7;
};

class Config {
public:
    static configuration getGlobalConfig();
};

#endif