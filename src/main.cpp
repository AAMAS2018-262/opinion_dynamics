#include "defs.h"
#include "experiments.h"

#include <string>

void printUsage();

int main(int argc, char* argv[])
{
	if (argc != 3) {
		Error("Wrong number of arguments");
		printUsage();
		return EXIT_FAILURE;
	}
	std::string experiments_file(argv[1]);
	std::string result_files_prefix(argv[2]);

	Experiments experiments(experiments_file, result_files_prefix);
	experiments.run();

	return EXIT_SUCCESS;
}

void printUsage()
{
	std::cout << "Usage: ./main <experiments_file> <result_files_prefix>" << std::endl;
}
