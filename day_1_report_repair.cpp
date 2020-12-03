/* day_1_report_repair.cpp */
/* Day 1 of advent of code 2020 */
/* Currently being built using g++ */
/* Linux Build: `g++ day_1_report_repair.cpp -o day_1_report_repair.o` */
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <getopt.h>

#define no_argument 0
#define required_argument 1
#define optional_argument 2

static const char *optstring = "f:s:h";

static struct option longopts[] = {
    { "filename", required_argument, NULL, 'f' },
    { "sum", required_argument, NULL, 's' },
    { "help", no_argument, NULL, 'h' },
    { NULL, 0, NULL, 0}
};

void calculate_sum(std::string filename, uint32_t sum) {
    std::vector<int> read_values;
    std::fstream input_file;
    input_file.open(filename);
    if (input_file.is_open()) {
        std::string read_string;
        while(getline(input_file, read_string)) {
            int read_int = std::stoi(read_string);
            read_values.push_back(read_int);
        }
        input_file.close();
    }
    for (uint16_t i = 0; i < read_values.size(); i++) {
        uint16_t index = i;
        while (index + 1 < read_values.size()) {
            if ((read_values[i] + read_values[index + 1]) == sum) {
                std::cout << read_values[i] << " + " << read_values[index + 1]
                          << " equals " << sum << std::endl;
                uint64_t product = read_values[i] * read_values[index + 1];
                std::cout << read_values[i] << " * " << read_values[index + 1]
                          << " equals " << product << std::endl;
            }
            index++;
        }
    }
}

int main(int argc, char* argv[]) {
    std::string filename = "";
    uint32_t sum = 0;
    std::string usage = "Help/Usage Example: ";
    std::string script_name = "day_1_report_repair.o ";
    std::string args = "-f <filename> -s <sum> ";
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
            case '?':
            case 'h':
            default:
                std::cout << usage << std::endl;
                return 1;
                break;
        }
    }
    calculate_sum(filename, sum);
    return 0;
}
