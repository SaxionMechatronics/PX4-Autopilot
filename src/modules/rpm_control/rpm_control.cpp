#include <px4_platform_common/log.h>

extern "C" __EXPORT int rpm_control_main(int argc, char *argv[]);

int rpm_control_main(int argc, char *argv[])
{
    PX4_INFO("rpm_control module (empty) loaded");
    return 0;
}
