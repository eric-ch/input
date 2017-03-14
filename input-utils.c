#include "project.h"

static int __input_support_event(int fd, unsigned int ev)
{
    size_t ev_types_len = __bitset_len(EV_MAX, unsigned long);
    unsigned long ev_types[ev_types_len];
    int rc;

    memset(ev_types, 0, sizeof (ev_types));
    rc = ioctl(fd, EVIOCGBIT(0, sizeof (ev_types)), ev_types);
    if (rc < 0) {
        warning("ioctl() EVIOCGBIT failed (%s).", strerror(errno));
        return -1;
    }
    return __test_bit(ev, ev_types);
}

void print_input_events(int fd)
{
    unsigned int evs[] = {
        EV_SYN, EV_KEY, EV_REL, EV_ABS, EV_MSC, EV_SW, EV_LED, EV_SND, EV_REP,
        EV_FF, EV_PWR, EV_FF_STATUS,
    };
    size_t ev_types_len = __bitset_len(EV_MAX, unsigned long);
    unsigned long ev_types[ev_types_len];
    unsigned int i;
    int rc;

    memset(ev_types, 0, sizeof (ev_types));
    rc = ioctl(fd, EVIOCGBIT(0, sizeof (ev_types)), ev_types);
    if (rc < 0) {
        warning("ioctl() EVIOCGBIT failed (%s).", strerror(errno));
        return;
    }

    for (i = 0; i < ARRAY_LEN(evs); ++i)
        if (__test_bit(evs[i], ev_types))
            info("bit%zu:%s", evs[i], ev_type_str(evs[i]));
}

void print_input_axes(int fd)
{
    unsigned int abs[] = {
        ABS_X, ABS_Y, ABS_Z,
        ABS_RX, ABS_RY, ABS_RZ,
        ABS_PRESSURE, ABS_WHEEL,
        ABS_MT_SLOT, ABS_MT_TOOL_X, ABS_MT_TOOL_Y,
        ABS_MT_POSITION_X, ABS_MT_POSITION_Y,
        ABS_MT_PRESSURE, ABS_MT_DISTANCE,
    };
    size_t abs_types_len = __bitset_len(ABS_MAX, unsigned long);
    unsigned long abs_types[abs_types_len];
    unsigned int i;
    int rc;

    if (__input_support_event(fd, EV_ABS) <= 0) {
        info("EV_ABS not supported...");
        return;
    }

    memset(abs_types, 0, sizeof (abs_types));
    rc = ioctl(fd, EVIOCGBIT(EV_ABS, sizeof (abs_types)), abs_types);
    if (rc < 0) {
        warning("ioctl() EVIOCGBIT failed (%s).", strerror(errno));
        return;
    }

    for (i = 0; i < ARRAY_LEN(abs); ++i)
        if (__test_bit(abs[i], abs_types))
            info("bit%zu:%s", abs[i], abs_type_str(abs[i]));
}

void print_input_keys(int fd)
{
    unsigned int keys[] = {
        BTN_LEFT, BTN_RIGHT, BTN_MIDDLE, BTN_SIDE,
        BTN_EXTRA, BTN_FORWARD, BTN_BACK, BTN_TASK,
        BTN_TOOL_PEN, BTN_TOOL_RUBBER,
        BTN_TOOL_BRUSH, BTN_TOOL_PENCIL, BTN_TOOL_AIRBRUSH,
        BTN_TOOL_FINGER, BTN_TOOL_MOUSE, BTN_TOOL_LENS, BTN_TOOL_QUINTTAP,
        BTN_TOUCH, BTN_STYLUS, BTN_STYLUS2,
        BTN_GEAR_UP, BTN_GEAR_DOWN,
    };
    size_t key_types_len = __bitset_len(KEY_MAX, unsigned long);
    unsigned long key_types[key_types_len];
    unsigned int i;
    int rc;

    if (__input_support_event(fd, EV_KEY) <= 0) {
        info("EV_KEY not supported...");
        return;
    }

    memset(key_types, 0, sizeof (key_types));
    rc = ioctl(fd, EVIOCGBIT(EV_KEY, sizeof (key_types)), key_types);
    if (rc < 0) {
        warning("ioctl() EVIOCGBIT failed (%s).", strerror(errno));
        return;
    }

    for (i = 0; i < ARRAY_LEN(keys); ++i)
        if (__test_bit(keys[i], key_types))
            info("bit%zu:%s", keys[i], key_type_str(keys[i]));
}

void print_input_keys_interesting(int fd)
{
    size_t key_types_len = __bitset_len(KEY_MAX, unsigned long);
    unsigned long key_types[key_types_len];
    unsigned int i;
    int rc;

    if (__input_support_event(fd, EV_KEY) <= 0) {
        info("EV_KEY not supported...");
        return;
    }

    memset(key_types, 0, sizeof (key_types));
    rc = ioctl(fd, EVIOCGBIT(EV_KEY, sizeof (key_types)), key_types);
    if (rc < 0) {
        warning("ioctl() EVIOCGBIT failed (%s).", strerror(errno));
        return;
    }

    for (i = 0; i < KEY_MIN_INTERESTING; ++i)
        if (__test_bit(i, key_types))
            info("bit%zu:%s", i, key_type_str(i));
}

void print_bt(void)
{
    void *bta[256];
    char **fnames;
    int i, n;

    n = backtrace(bta, sizeof (bta) / sizeof (bta[0]));
    fnames = backtrace_symbols(bta, n);
    if (fnames == NULL) {
        error("backtrace_symbols:");
        exit(1);
    }

    for (i = 0; i < n; ++i) {
        info("%s", fnames[i]);
    }

    free(fnames);
}

