#include "rpm_control.hpp"

#include <px4_platform_common/log.h>

#include <px4_platform_common/getopt.h>
#include <px4_platform_common/log.h>
#include <px4_platform_common/posix.h>

#include <uORB/topics/parameter_update.h>
#include <uORB/topics/sensor_combined.h>


int RPMControl::print_status()
{
	PX4_INFO("Running");
	//TODO: Add more status information here as needed
	return 0;
}

int RPMControl::custom_command(int argc, char *argv[])
{
	/*
	if (!is_running()) {
		print_usage("not running");
		return 1;
	}

	// additional custom commands can be handled like this:
	if (!strcmp(argv[0], "do-something")) {
		get_instance()->do_something();
		return 0;
	}
	 */

	return print_usage("unknown command");
}

int RPMControl::task_spawn(int argc, char *argv[])
{
	_task_id = px4_task_spawn_cmd("module",
				      SCHED_DEFAULT,
				      SCHED_PRIORITY_DEFAULT,
				      1024,
				      (px4_main_t)&run_trampoline,
				      (char *const *)argv);

	if (_task_id < 0) {
		_task_id = -1;
		return -errno;
	}

	return 0;
}

RPMControl *RPMControl::instantiate(int argc, char *argv[])
{
	int example_param = 0;
	bool example_flag = false;
	bool error_flag = false;

	int myoptind = 1;
	int ch;
	const char *myoptarg = nullptr;

	// parse CLI arguments
	while ((ch = px4_getopt(argc, argv, "p:f", &myoptind, &myoptarg)) != EOF) {
		switch (ch) {
		case 'p':
			example_param = (int)strtol(myoptarg, nullptr, 10);
			break;

		case 'f':
			example_flag = true;
			break;

		case '?':
			error_flag = true;
			break;

		default:
			PX4_WARN("unrecognized flag");
			error_flag = true;
			break;
		}
	}

	if (error_flag) {
		return nullptr;
	}

	RPMControl *instance = new RPMControl(example_param, example_flag);

	if (instance == nullptr) {
		PX4_ERR("alloc failed");
	}

	return instance;
}

RPMControl::RPMControl(int example_param, bool example_flag)
	: ModuleParams(nullptr)
{
}

void RPMControl::run()
{
    	PX4_INFO("Started");

	while (!should_exit()) {

		// Read thrust setpoint
		actuator_motors_s motors{};
		if (_sub_actuator_motors.update(&motors)) {
		}

		// Read ESC rpm feedback
		esc_status_s esc{};
		if (_sub_esc_status.update(&esc)) {
		}

		actuator_outputs_s actuator_outputs{};

		actuator_outputs.timestamp = hrt_absolute_time();
		actuator_outputs.noutputs = actuator_outputs_s::NUM_ACTUATOR_OUTPUTS;

		for (int i = 0; i < (int)actuator_outputs.noutputs; i++) {
		actuator_outputs.output[i] = 0.f;
		}

		// Publish outputs
		_pub_actuator_outputs.publish(actuator_outputs);

		px4_usleep(10000); // 100 Hz loop
	}
	PX4_INFO("Exiting");
}


void RPMControl::parameters_update(bool force)
{
	// check for parameter updates
	if (_parameter_update_sub.updated() || force) {
		// clear update
		parameter_update_s update;
		_parameter_update_sub.copy(&update);

		// update parameters from storage
		updateParams();
	}
}

int RPMControl::print_usage(const char *reason)
{
	if (reason) {
		PX4_WARN("%s\n", reason);
	}

	PRINT_MODULE_DESCRIPTION(
		R"DESCR_STR(
### Description
Section that describes the provided module functionality.

This is a template for a module running as a task in the background with start/stop/status functionality.

### Implementation
Section describing the high-level implementation of this module.

### Examples
CLI usage example:
$ module start -f -p 42

)DESCR_STR");

	PRINT_MODULE_USAGE_NAME("module", "template");
	PRINT_MODULE_USAGE_COMMAND("start");
	PRINT_MODULE_USAGE_PARAM_FLAG('f', "Optional example flag", true);
	PRINT_MODULE_USAGE_PARAM_INT('p', 0, 0, 1000, "Optional example parameter", true);
	PRINT_MODULE_USAGE_DEFAULT_COMMANDS();

	return 0;
}

int rpm_control_main(int argc, char *argv[])
{
	return RPMControl::main(argc, argv);
}
