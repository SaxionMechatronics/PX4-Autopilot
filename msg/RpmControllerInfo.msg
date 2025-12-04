# TOPICS rpm_controller_info

uint64 timestamp             # [us] time since system boot

uint8 num_motors             # number of valid entries in the arrays

# Per-motor RPM control info (index 0..num_motors-1)
float32[16]  rpm_setpoint    # [rpm] desired RPM
float32[16] rpm_measured     # [rpm] measured RPM (filtered)
float32[16]  rpm_error       # [rpm] setpoint - measured

# Control command
float32[16] cmd_norm         # [-] normalized command (0..1)
uint16[16] dshot_command     # [DShot units] raw DShot throttle command
