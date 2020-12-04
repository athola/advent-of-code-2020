/* report_repair.cpp 
 * Day 1 of advent of code 2020 
 * Currently being built using g++ 
 * Linux Build: `g++ report_repair.cpp -o report_repair.o`
 * Copyright: Alex Thola 12/02/2020
 */

#include <getopt.h>
#include <sys/stat.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#define no_argument 0
#define required_argument 1
#define optional_argument 2

static const char *optstring = "f:s:c:h";

static struct option longopts[] = {
    { "filename", required_argument, NULL, 'f' },
    { "sum", required_argument, NULL, 's' },
    { "count", required_argument, NULL, 'c' },
    { "help", no_argument, NULL, 'h' },
    { NULL, 0, NULL, 0}
};

void calculate_sum(std::string filename, uint32_t sum, uint16_t count) {
    std::vector<int> read_values;
    std::fstream input_file;
    input_file.open(filename);
    if (input_file.is_open()) {
        std::string read_string;
        while (getline(input_file, read_string)) {
            int read_int = std::stoi(read_string);
            read_values.push_back(read_int);
        }
        input_file.close();
    }
    if (count == 2) {
        for (uint16_t i = 0; i < read_values.size(); i++) {
            for (uint16_t index = i + 1; index < read_values.size(); index++) {
                if ((read_values[i] + read_values[index]) == sum) {
                    std::cout << read_values[i] << " + "
                              << read_values[index]
                              << " equals " << sum << std::endl;
                    uint64_t product = read_values[i] * read_values[index];
                    std::cout << read_values[i] << " * "
                              << read_values[index]
                              << " equals " << product << std::endl;
                    return;
                }
            }
        }
    } else if (count == 3) {
        for (uint16_t i = 0; i < read_values.size(); i++) {
            for (uint16_t index = i + 1; index < read_values.size(); index++) {
                for (uint16_t index_2 = index + 1; index_2 < read_values.size();
                        index_2++) {
                    if ((read_values[i] + read_values[index]
                            + read_values[index_2]) == sum) {
                        std::cout << read_values[i] << " + "
                                  << read_values[index] << " + "
                                  << read_values[index_2]
                                  << " equals " << sum << std::endl;
                        uint64_t product =
                                read_values[i] * read_values[index]
                                               * read_values[index_2];
                        std::cout << read_values[i] << " * "
                                  << read_values[index] << " * "
                                  << read_values[index_2]
                                  << " equals " << product << std::endl;
                        return;
                    }
                }
            }
        }
    }
    std::cout << "Could not find " << count << " numbers which add up to "
              << sum << std::endl;
}

int main(int argc, char* argv[]) {
    std::string filename = "";
    uint32_t sum = 0;
    uint16_t count = 0;
    std::string usage = "Help/Usage Example: ";
    std::string script_name = "day_1_report_repair.o ";
    std::string args = "-f <filename> -s <sum> -c <count> ";
    std::string bools = "-h";
    usage.append(filename);
    usage.append(args);
    usage.append(bools);

    int cmd;
    while ((cmd = getopt_long(argc, argv, optstring, longopts, NULL)) != -1) {
        switch (cmd) {
            case 'f':
                if (optarg) filename = optarg;
                break;
            case 's':
                if (optarg) sum = std::stoi(optarg);
                break;
            case 'c':
                if (optarg) count = std::stoi(optarg);
                break;
            case '?':
            case 'h':
            default:
                std::cout << usage << std::endl;
                return 1;
                break;
        }
    }

    if (sum == 0 || count == 0 || filename == "") {
        std::cerr << "Required arguments not entered - please enter filename, "
                  << "sum, and count." << std::endl;
        return -1;
    }

    if (count != 2 && count != 3) {
        std::cerr << "Provided count of numbers used for sum must be 2 or 3."
                  << std::endl;
        return -1;
    }

    struct stat buffer;
    if (stat(filename.c_str(), &buffer) != 0) {
        std::cerr << "Provided filename does not exist." << std::endl;
        return -1;
    }

    calculate_sum(filename, sum, count);

    return 0;
}
