#include "cmds.h"
#include "app_cfg.h"
#include "utils_wrapper.h"
#include "json_wrapper.h"
#include "wifi_wrapper.h"

#include "esp_log.h"
#include <cstring>

namespace cmds {

constexpr static char TAG[] = "cmds";

#define CMD_ASSERT(condition) do { if (!(condition)) { return Wrapper::Utils::snprint("assert failed, condition '" #condition "'"); } } while (0)

#define CMD_SWITCH(...) \
CMD_ASSERT(argc >= 1); \
switch (Wrapper::Utils::BKDR_hash(argv[0])) { \
	__VA_ARGS__ \
	default: return Wrapper::Utils::snprint("unknown option '%s'", argv[0]); \
}

#define CMD_CASE(name, ...) \
case #name##_hash: { \
	CMD_ASSERT(argc >= 1); \
	return ([](int argc, char* argv[]) { __VA_ARGS__ })(argc - 1, argv + 1); \
} break;

#define CMD_CASE_REUSE(name1, name2) \
case #name1##_hash: { \
	CMD_ASSERT(argc >= 1); \
	return cmd_##name2(argc - 1, argv + 1); \
}

#define CMD_CASE_ROOT(name) CMD_CASE_REUSE(name, name)


static OBuf cmd_wifi(int argc, char* argv[]) {
	CMD_SWITCH(
		CMD_CASE(state,
			Wrapper::JsonObject json;
			json["status"] = Wrapper::WiFi::stateString(Wrapper::WiFi::state());
			return Wrapper::Utils::snprint("%s", json.serialize().data());
		);
		CMD_CASE(connect,
			CMD_ASSERT(argc == 2);
			Wrapper::JsonObject json;
			Wrapper::WiFi::State state = Wrapper::WiFi::Station::provision(argv[0], argv[1]);
			if (state == Wrapper::WiFi::State::CONNECTED) {
				json["status"] = "succeed";
			} else {
				json["status"] = "failed";
			}
			return Wrapper::Utils::snprint("%s", json.serialize().data());
		);
	);
}


OBuf default_cmd_bundle(int argc, char* argv[]) {
	CMD_SWITCH(
		CMD_CASE_ROOT(wifi);
	);
}

OBuf call( int argc, char* argv[]) {
	return default_cmd_bundle(argc, argv) + OBuf(1, '\n');
}

}
