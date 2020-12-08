/* binary_boarding.cpp
 * Day 5 of advent of code 2020
 * Currently being built using g++
 * Linux Build:
 * `g++ binary_boarding.cpp -o binary_boarding.o`
 * Copyright: Alex Thola 12/08/2020
 */

#include <getopt.h>
#include <sys/stat.h>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#define no_argument 0
#define required_argument 1
#define optional_argument 2

static const char *optstring = "f:F:B:L:R:h";

static struct option longopts[] = {
    { "filename", required_argument, NULL, 'f' },
    { "first_row", optional_argument, NULL, 'F' },
    { "row_count", optional_argument, NULL, 'B' },
    { "first_column", optional_argument, NULL, 'L' },
    { "column_count", optional_argument, NULL, 'R' },
    { "help", no_argument, NULL, 'h' },
    { NULL, 0, NULL, 0}
};

void check_boarding_passes(std::string filename,
                           uint16_t first_row,
                           uint16_t row_count,
                           uint16_t first_column,
                           uint16_t column_count) {
    std::fstream input_file;
    input_file.open(filename);
    if (input_file.is_open()) {
        std::string read_string;
        std::vector<uint32_t> seat_ids;
        while (getline(input_file, read_string)) {
            uint16_t row_upper = row_count;
            uint16_t row_lower = first_row;
            uint16_t column_upper = column_count;
            uint16_t column_lower = first_column;
            std::string rows = read_string.substr(0, read_string.size() - 3);
            std::string cols = read_string.substr(read_string.size() - 3, read_string.size());
            for (char row_partition : rows) {
                if (row_partition == 'F') {
                    row_upper = (row_upper + row_lower) / 2;
                } else if (row_partition == 'B') {
                    row_lower = (row_upper + row_lower) / 2;
                }
            }
            for (char col_partition : cols) {
                if (col_partition == 'L') { 
                    column_upper = (column_upper + column_lower) / 2;
                } else if (col_partition == 'R') {
                    column_lower = (column_upper + column_lower) / 2;
                }
            }
            uint32_t seat_id = row_lower * 8 + column_lower;
            seat_ids.push_back(seat_id);
        }
        std::sort(seat_ids.begin(), seat_ids.end());
        uint32_t largest_seat_id = seat_ids.back();
        std::cout << "Largest seat ID is: " << largest_seat_id << std::endl;
        uint32_t temp_id = 0;
        for (uint32_t id : seat_ids) {
            if ((id - temp_id) == 2) {
                std::cout << "Your seat ID is: " << id - 1 << std::endl;
            }
            temp_id = id;;
        }
        input_file.close();
    }
}

int main(int argc, char* argv[]) {
    std::string filename = "";
    uint16_t first_row = 0;
    uint16_t row_count = 128;
    uint16_t first_column = 0;
    uint16_t column_count = 8;
    std::string usage = "Help/Usage Example: ";
    std::string script_name = "binary_boarding.o ";
    std::string args = "-f <filename> -F <first_row> -B <row_count> ";
    std::string args2 = "-L <first_column> -R <column_count> ";
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
            case 'F':
                if (optarg) first_row = std::stoi(optarg);
                break;
            case 'B':
                if (optarg) row_count = std::stoi(optarg);
                break;
            case 'L':
                if (optarg) first_column = std::stoi(optarg);
                break;
            case 'R':
                if (optarg) column_count = std::stoi(optarg);
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

    check_boarding_passes(filename, first_row, row_count, first_column, column_count);

    return 0;
}
