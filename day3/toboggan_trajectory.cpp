/* toboggan_trajectory.cpp
 * Day 3 of advent of code 2020
 * Currently being built using g++
 * Linux Build:
 * `g++ toboggan_trajectory.cpp -o toboggan_trajectory.o`
 * Copyright: Alex Thola 12/03/2020
 */

#include <getopt.h>
#include <sys/stat.h>
#include <iostream>
#include <fstream>
#include <string>

#define no_argument 0
#define required_argument 1
#define optional_argument 2

static const char *optstring = "f:s:d:m:h";

static struct option longopts[] = {
    { "filename", required_argument, NULL, 'f' },
    { "step", optional_argument, NULL, 's' },
    { "down", optional_argument, NULL, 'd' },
    { "multiple", optional_argument, NULL, 'm' },
    { "help", no_argument, NULL, 'h' },
    { NULL, 0, NULL, 0}
};

uint32_t count_trees(std::string filename, int step, int down) {
    std::fstream input_file;
    input_file.open(filename);
    uint16_t current_position = 0;
    uint32_t tree_count = 0;
    uint32_t line_number = 1;
    if (input_file.is_open()) {
        std::string read_string;
        while (getline(input_file, read_string)) {
            if ((line_number - 1) % down == 0) {
                if (read_string.at(current_position) == '#') {
                    tree_count++;
                }
                if (current_position + step >= read_string.length()) {
                    current_position =
                        current_position + step - read_string.length();
                } else {
                    current_position += step;
                }
            }
            line_number++;
        }
        input_file.close();
    }
    return tree_count;
}

int main(int argc, char* argv[]) {
    std::string filename = "";
    std::string multiple = "";
    int step;
    int down;
    std::string usage = "Help/Usage Example: ";
    std::string script_name = "toboggan_trajectory.o ";
    std::string args = "-f <filename> -s <step> -d <down> -m <multiple> ";
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
                if (optarg) step = std::stoi(optarg);
                break;
            case 'd':
                if (optarg) down = std::stoi(optarg);
                break;
            case 'm':
                if (optarg) multiple = optarg;
                break;
            case '?':
            case 'h':
            default:
                std::cout << usage << std::endl;
                return 1;
                break;
        }
    }

    struct stat buffer;
    if (stat(filename.c_str(), &buffer) != 0) {
        std::cerr << "Provided filename does not exist." << std::endl;
        return -1;
    }
    
    if (!multiple.empty()) {
        uint32_t tree_product = 1;
        if (multiple.length() % 2 != 0) {
            std::cerr << "Invalid value passed to multiple - "
                      << "must be pairs of step/down values, i.e. 1131517112"
                      << std::endl;
            return -1;
        }
        for (int i = 0; i < multiple.length(); i += 2) {
            tree_product *= count_trees(filename,
                                        static_cast<int>(multiple.at(i)) - 48,
                                        static_cast<int>(multiple.at(i + 1)) - 48);
        }
        std::cout << "Tree product: " << tree_product
                  << std::endl;
    } else {
        uint32_t tree_count = count_trees(filename, step, down);
        std::cout << "Number of trees encountered: " << tree_count
                  << std::endl;
    }
    

    return 0;
}
