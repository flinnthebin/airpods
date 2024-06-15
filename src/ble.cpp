#include "ble.hpp"
#include <iostream>
#include <thread>

constexpr auto airpodsMAC = "B0:3F:64:21:7E:D7";
auto airpods = false;

namespace ble {

	auto prompt(int pipe_fd) -> void {
		boost::iostreams::file_descriptor_source fd_source(pipe_fd, boost::iostreams::never_close_handle);
		boost::iostreams::stream<boost::iostreams::file_descriptor_source> pipe_stream(fd_source);

		auto line = std::string{};
		while (std::getline(pipe_stream, line)) {
			std::cout << line << std::endl;
			if (line.find("#") != std::string::npos) {
				break;
			}
		}
	}

	void connectBLE(const std::string& deviceMAC) {
		FILE* pipe = popen("zsh -c 'bluetoothctl'", "r+");
		if (!pipe) {
			std::cerr << "Error starting bluetoothctl: " << strerror(errno) << std::endl;
			return;
		}

		auto pipe_fd = fileno(pipe);
		if (pipe_fd == -1) {
			std::cerr << "Error getting file descriptor: " << strerror(errno) << std::endl;
			pclose(pipe);
			return;
		}

		if (fcntl(pipe_fd, F_SETFL, O_NONBLOCK) == -1) {
			std::cerr << "Error setting non-blocking mode: " << strerror(errno) << std::endl;
			pclose(pipe);
			return;
		}

		std::thread outputThread(prompt, pipe_fd);

		if (fprintf(pipe, "scan on\n") < 0 || fflush(pipe) == EOF) {
			std::cerr << "Error sending scan on command: " << strerror(errno) << std::endl;
			pclose(pipe);
			return;
		}

		if (fprintf(pipe, "connect %s\n", deviceMAC.c_str()) < 0 || fflush(pipe) == EOF) {
			std::cerr << "Error sending connect command: " << strerror(errno) << std::endl;
			pclose(pipe);
			return;
		}

		if (fprintf(pipe, "scan off\n") < 0 || fflush(pipe) == EOF) {
			std::cerr << "Error sending scan off command: " << strerror(errno) << std::endl;
			pclose(pipe);
			return;
		}

		if (fprintf(pipe, "quit\n") < 0 || fflush(pipe) == EOF) {
			std::cerr << "Error sending quit command: " << strerror(errno) << std::endl;
			pclose(pipe);
			return;
		}

		outputThread.join();
		pclose(pipe);
	}

	auto main(int argc, char* argv[]) -> int {
		for (int i = 1; i < argc; ++i) {
			if (std::string(argv[i]) == "-airpods") {
				airpods = true;
			}
		}

		if (airpods) {
			connectBLE(airpodsMAC);
		}
		else {
			std::cout << "No device specified. Use -airpods to connect to AirPods." << std::endl;
		}
		return 0;
	}
} // namespace ble
	
	auto main(int argc, char* argv[]) -> int {
		for (int i = 1; i < argc; ++i) {
			if (std::string(argv[i]) == "-airpods") {
				airpods = true;
			}
		}

		if (airpods) {
			ble::connectBLE(airpodsMAC);
		}
		else {
			std::cout << "No device specified. Use -airpods to connect to AirPods." << std::endl;
		}
		return 0;
	}

