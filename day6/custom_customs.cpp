/* custom_customs.cpp
 * Day 6 of advent of code 2020
 * Currently being built using g++
 * Linux Build:
 * `g++ custom_customs.cpp -o custom_customs.o`
 * Copyright: Alex Thola 12/08/2020
 */

#include <getopt.h>
#include <sys/stat.h>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <set>
#include <string>
#include <vector>

#define no_argument 0
#define required_argument 1
#define optional_argument 2

static const char *optstring = "f:eh";

static struct option longopts[] = {
    { "filename", required_argument, NULL, 'f' },
    { "everyone", no_argument, NULL, 'e' },
    { "help", no_argument, NULL, 'h' },
    { NULL, 0, NULL, 0}
};

uint16_t get_unique_answers(std::vector<std::string>& answer_group) {
    std::string answer_line = answer_group[0];
    for (int i = 0; i < answer_group.size() - 1; i++) {
        std::string answer_line_next = answer_group[i + 1];
        std::sort(std::begin(answer_line),
                  std::end(answer_line));
        std::sort(std::begin(answer_line_next),
                  std::end(answer_line_next));
        std::string answer_intersection;
        std::set_intersection(std::begin(answer_line),
                              std::end(answer_line),
                              std::begin(answer_line_next),
                              std::end(answer_line_next),
                              std::back_inserter(
                                    answer_intersection));
        answer_line = answer_intersection;
    }
    answer_group.clear();
    return answer_line.size();
}

void count_answers(std::string filename, bool everyone_answer) {
    std::fstream input_file;
    input_file.open(filename);
    uint16_t total_answers = 0;
    if (input_file.is_open()) {
        std::string read_string;
        if (!everyone_answer) {
            std::set<char> answers;
            while (getline(input_file, read_string)) {
                if (read_string != "") {
                    for (char answer : read_string) {
                        answers.insert(answer);
                    }
                } else {
                    total_answers += answers.size();
                    answers.clear();
                }
            }
            //  Need to append final sum of answers as file ending does not
            //  have an empty string.
            total_answers += answers.size();
        } else {
            std::vector<std::string> answer_group;
            while (getline(input_file, read_string)) {
                if (read_string != "") {
                    answer_group.push_back(read_string);
                } else {
                    total_answers += get_unique_answers(answer_group);
                }
            }
            total_answers += get_unique_answers(answer_group);
        }
        input_file.close();
    }
    std::cout << "Number of answers: " << total_answers << std::endl;
}

int main(int argc, char* argv[]) {
    std::string filename = "";
    bool everyone_answer = false;
    std::string usage = "Help/Usage Example: ";
    std::string script_name = "custom_customs.o ";
    std::string args = "-f <filename> ";
    std::string bools = "-e -h";
    usage.append(filename);
    usage.append(args);
    usage.append(bools);
    int cmd;
    while ((cmd = getopt_long(argc, argv, optstring, longopts, NULL)) != -1) {
        switch (cmd) {
            case 'f':
                if (optarg) filename = optarg;
                break;
            case 'e':
                everyone_answer = true;
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

    count_answers(filename, everyone_answer);

    return 0;
}
