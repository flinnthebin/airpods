#include <boost/iostreams/device/file_descriptor.hpp>
#include <boost/iostreams/stream.hpp>
#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <string>
#include <unistd.h>

namespace ble {

	// utilising boost::iostreams to synchronise commands to system prompt returns
	auto prompt(int pipe_fd) -> void;

	// Opens a pipe to bluetoothctl to automate the device connection process
	auto connect_BLE(const std::string& dev_MAC);

} // namespace ble
