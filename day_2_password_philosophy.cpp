/* day_2_password_philosophy.cpp
 * Day 2 of advent of code 2020
 * Currently being built using g++
 * Linux Build:
 *  `g++ day_2_password_philosophy.cpp -o day_2_password_philosophy.o`
 */
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <getopt.h>
#include <sys/stat.h>

#define no_argument 0
#define required_argument 1
#define optional_argument 2

static const char *optstring = "f:ph";

static struct option longopts[] = {
    { "filename", required_argument, NULL, 'f' },
    { "position", no_argument, NULL, 'p' },
    { "help", no_argument, NULL, 'h' },
    { NULL, 0, NULL, 0}
};

void validate_passwords(std::string filename, bool position) {
    uint32_t valid_password_count = 0;
    std::fstream input_file;
    input_file.open(filename);
    if (input_file.is_open()) {
        std::string read_string;
        while (getline(input_file, read_string)) {
            std::vector<std::string> password_info;
            std::istringstream password_info_stream(read_string);
            for (std::string part; password_info_stream >> part;) {
                password_info.push_back(part);
            }
            std::string password = password_info[2];
            char policy_char = password_info[1].at(0);
            int char_count = std::count(password.begin(),
                                        password.end(),
                                        policy_char);
            std::vector<uint16_t> count_bounds;
            std::string count_bounds_str = password_info[0];
            size_t last = 0;
            size_t next = 0;
            while ((next = count_bounds_str.find("-", last)) !=
                    std::string::npos) {
                count_bounds.push_back(
                        std::stoi(count_bounds_str.substr(last, next - last)));
                last = next + 1;
            }
            count_bounds.push_back(std::stoi(count_bounds_str.substr(last)));
            uint16_t lower_bound = count_bounds[0];
            uint16_t upper_bound = count_bounds[1];
            if (!position) {
                if (char_count > 0 && lower_bound <= char_count
                        && char_count <= upper_bound) {
                    valid_password_count++;
                }
            } else {
                char lb_char = ' ';
                if (lower_bound <= password.length()) {
                    lb_char = password.at(lower_bound - 1);
                }
                char ub_char = ' ';
                if (upper_bound <= password.length()) {
                    ub_char = password.at(upper_bound - 1);
                }
                if ((lb_char == policy_char && ub_char != policy_char)
                        || (ub_char == policy_char && lb_char != policy_char)) {
                    valid_password_count++;
                }
            }       
        }
        input_file.close();
    }
    std::cout << "Number of valid passwords: " << valid_password_count
              << std::endl;
}

int main(int argc, char* argv[]) {
    std::string filename = "";
    bool password_position = false;
    std::string usage = "Help/Usage Example: ";
    std::string script_name = "day_1_report_repair.o ";
    std::string args = "-f <filename> ";
    std::string bools = "-p -h";
    usage.append(filename);
    usage.append(args);
    usage.append(bools);
    int cmd;
    while ((cmd = getopt_long(argc, argv, optstring, longopts, NULL)) != -1) {
        switch (cmd) {
            case 'f':
                if (optarg) filename = optarg;
                break;
            case 'p':
                password_position = true;
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

    validate_passwords(filename, password_position);

    return 0;
}
