#define BOOST_TEST_MODULE TestPopenWithZsh
#include <boost/asio.hpp>
#include <boost/process.hpp>
#include <boost/test/included/unit_test.hpp>
#include <string>

namespace bp = boost::process;
namespace asio = boost::asio;

BOOST_AUTO_TEST_CASE(TestPopenWithZsh) {
	asio::io_context io_context;
	bp::ipstream pipe_stream;

	bp::child spawn(bp::search_path("zsh"), "-c", "echo Hello, World!", bp::std_out > pipe_stream, io_context);

	auto line = std::string{};
	auto result = std::string{};
	while (std::getline(pipe_stream, line)) {
		result += line + "\n";
	}

	spawn.wait();

	BOOST_CHECK_EQUAL(spawn.exit_code(), 0);

	BOOST_CHECK_EQUAL(result, "Hello, World!\n");
}
