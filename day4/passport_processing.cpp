/* passport_processing.cpp
 * Day 4 of advent of code 2020
 * Currently being built using g++
 * Linux Build:
 * `g++ passport_processing.cpp -o passport_processing.o`
 * Copyright: Alex Thola 12/04/2020
 */

#include <getopt.h>
#include <sys/stat.h>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <sstream>
#include <vector>

#define no_argument 0
#define required_argument 1
#define optional_argument 2

static const char *optstring = "f:vh";

static struct option longopts[] = {
    { "filename", required_argument, NULL, 'f' },
    { "values", no_argument, NULL, 'v' },
    { "help", no_argument, NULL, 'h' },
    { NULL, 0, NULL, 0}
};

void validate_passports(std::string filename, bool validate_values = false) {
    std::fstream input_file;
    input_file.open(filename);
    uint16_t invalid_passport_count = 0;
    uint16_t passports_checked = 0;
    if (input_file.is_open()) {
        std::string read_string;
        std::vector<std::string> passport_info;
        std::vector<std::string> fields = {"byr", "iyr", "eyr", "hgt", "hcl",
                                           "ecl", "pid", "cid"};
        while (getline(input_file, read_string)) {
            if (read_string != "") {
                std::istringstream passport_info_stream(read_string);
                for (std::string part; passport_info_stream >> part;) {
                    passport_info.push_back(part);
                }
            } else {
                std::map<std::string, std::string> passport_items;
                std::vector<std::string> passport_keys;
                bool invalid = false;
                for (std::string passport_field : passport_info) {
                    size_t next = 0, last = 0;
                    std::string field_key = "", field_value = "";
                    while ((next = passport_field.find(":", last)) !=
                            std::string::npos) {
                        field_key = passport_field.substr(last, next - last);
                        passport_keys.push_back(field_key);
                        last = next + 1;
                    }
                    field_value = passport_field.substr(last);
                    passport_items.insert(
                            std::pair<std::string, std::string>(
                                    field_key, field_value));
                }
                for (std::string field : fields) {
                    if ((std::find(passport_keys.begin(),
                                   passport_keys.end(),
                                   field) == passport_keys.end())
                            && (field != "cid")) {
                        std::cout << "invalid field " << field
                                  << " at passport count " << passports_checked + 1
                                  << std::endl;
                        invalid = true;
                        break;
                    }
                }
                if (invalid) {
                    invalid_passport_count++;
                    passport_info.clear();
                    passports_checked++;
                    continue;
                }
                if (validate_values) {
                    //  Checking BYR field.
                    std::map<std::string, std::string>::iterator byr_value =
                            passport_items.find("byr");
                    if (byr_value != passport_items.end()) {
                        int byr_int = std::stoi(byr_value->second);
                        if (byr_int < 1920 || byr_int > 2002) {
                            std::cout << "invalid byr " << byr_int
                                      << " at passport count "
                                      << passports_checked + 1
                                      << std::endl;
                            invalid_passport_count++;
                            passport_info.clear();
                            passports_checked++;
                            continue;
                        }
                    }
                    //  Checking IYR field.
                    std::map<std::string, std::string>::iterator iyr_value =
                            passport_items.find("iyr");
                    if (iyr_value != passport_items.end()) {
                        int iyr_int = std::stoi(iyr_value->second);
                        if (iyr_int < 2010 || iyr_int > 2020) {
                            std::cout << "invalid iyr " << iyr_int
                                      << " at passport count "
                                      << passports_checked + 1
                                      << std::endl;
                            invalid_passport_count++;
                            passport_info.clear();
                            passports_checked++;
                            continue;
                        }
                    }
                    //  Checking EYR field.
                    std::map<std::string, std::string>::iterator eyr_value =
                            passport_items.find("eyr");
                    if (eyr_value != passport_items.end()) {
                        int eyr_int = std::stoi(eyr_value->second);
                        if (eyr_int < 2020 || eyr_int > 2030) {
                            std::cout << "invalid eyr " << eyr_int
                                      << " at passport count "
                                      << passports_checked + 1
                                      << std::endl;
                            invalid_passport_count++;
                            passport_info.clear();
                            passports_checked++;
                            continue;
                        }
                    }
                    //  Checking HGT field.
                    std::map<std::string, std::string>::iterator hgt_value =
                            passport_items.find("hgt");
                    if (hgt_value != passport_items.end()) {
                        std::string hgt_str = hgt_value->second;
                        int found_cm = hgt_str.find("cm");
                        int found_in = hgt_str.find("in");
                        if ((found_cm == -1 && found_in == -1) ||
                                (found_cm != -1 && found_in != -1)) {
                            std::cout << "missing cm or in from " << hgt_str
                                      << " at passport count "
                                      << passports_checked + 1
                                      << std::endl;
                            invalid_passport_count++;
                            passport_info.clear();
                            passports_checked++;
                            continue;
                        }
                        int hgt_value_int =
                                std::stoi(hgt_str.substr(0,
                                          hgt_str.size() - 2));
                        if (found_in != -1) {
                            if (hgt_value_int < 59 || hgt_value_int > 76) {
                                std::cout << "hgt " << hgt_str
                                          << " is invalid"
                                          << " at passport count "
                                          << passports_checked + 1
                                          << std::endl;
                                invalid_passport_count++;
                                passport_info.clear();
                                passports_checked++;
                                continue;
                            }
                        } else {
                            if (hgt_value_int < 150 || hgt_value_int > 193) {
                                std::cout << "hgt " << hgt_str
                                          << " is invalid"
                                          << " at passport count "
                                          << passports_checked + 1
                                          << std::endl;
                                invalid_passport_count++;
                                passport_info.clear();
                                passports_checked++;
                                continue;
                            }
                        }
                    }
                    //  Checking HCL field.
                    std::map<std::string, std::string>::iterator hcl_value =
                            passport_items.find("hcl");
                    if (hcl_value != passport_items.end()) {
                        std::string hcl_str = hcl_value->second;
                        int found_hashtag = hcl_str.find("#");
                        if (found_hashtag == -1) {
                            std::cout << "missing # from " << hcl_str
                                      << " at passport count "
                                      << passports_checked + 1
                                      << std::endl;
                            invalid_passport_count++;
                            passport_info.clear();
                            passports_checked++;
                            continue;
                        }
                        if (hcl_str.size() != 7) {
                            std::cout << "invalid size of string " << hcl_str
                                      << " at passport count "
                                      << passports_checked + 1
                                      << std::endl;
                            invalid_passport_count++;
                            passport_info.clear();
                            passports_checked++;
                            continue;
                        }
                        invalid = false;
                        uint16_t char_count = 0;
                        for (char hcl_char : hcl_str) {
                            uint16_t hcl_char_int =
                                    static_cast<uint16_t>(hcl_char);
                            if (char_count == 0) {
                                char_count++;
                                continue;
                            }
                            if (hcl_char_int < 48) {
                                std::cout << "invalid char " << hcl_char
                                          << "in hcl str " << hcl_str
                                          << " at passport count "
                                          << passports_checked + 1
                                          << std::endl;
                                invalid = true;
                                break;
                            }
                            if (hcl_char_int > 57) {
                                if (hcl_char_int < 97 || hcl_char_int > 102) {
                                    std::cout << "invalid char " << hcl_char
                                              << "in hcl str " << hcl_str
                                              << " at passport count "
                                              << passports_checked + 1
                                              << std::endl;
                                    invalid = true;
                                    break;
                                }
                            }
                            char_count++;
                        }
                        if (invalid) {
                            invalid_passport_count++;
                            passport_info.clear();
                            passports_checked++;
                            continue;
                        }
                    }
                    //  Checking ECL field.
                    std::map<std::string, std::string>::iterator ecl_value =
                            passport_items.find("ecl");
                    std::vector<std::string> eye_colors = {"amb", "blu", "brn",
                                                           "gry", "grn", "hzl",
                                                           "oth"};
                    if (ecl_value != passport_items.end()) {
                        std::string ecl_str = ecl_value->second;
                        if (ecl_str.size() != 3) {
                            std::cout << "invalid size of string " << ecl_str
                                      << " at passport count "
                                      << passports_checked + 1
                                      << std::endl;
                            invalid_passport_count++;
                            passport_info.clear();
                            passports_checked++;
                            continue;
                        }
                        if (std::find(eye_colors.begin(),
                                      eye_colors.end(),
                                      ecl_str) == eye_colors.end()) {
                            std::cout << "invalid ecl " << ecl_str
                                      << " at passport count "
                                      << passports_checked + 1
                                      << std::endl;
                            invalid_passport_count++;
                            passport_info.clear();
                            passports_checked++;
                            continue;
                        }
                    }
                    //  Checking PID field.
                    std::map<std::string, std::string>::iterator pid_value =
                            passport_items.find("pid");
                    if (pid_value != passport_items.end()) {
                        std::string pid_str = pid_value->second;
                        if (pid_str.size() != 9) {
                            std::cout << "invalid size of string " << pid_str
                                      << " at passport count "
                                      << passports_checked + 1
                                      << std::endl;
                            invalid_passport_count++;
                            passport_info.clear();
                            passports_checked++;
                            continue;
                        }
                        invalid = false;
                        for (char pid_char : pid_str) {
                            uint16_t pid_char_int =
                                    static_cast<uint16_t>(pid_char);
                            if (pid_char_int < 48) {
                                std::cout << "invalid char " << pid_char
                                          << "in pid str " << pid_str
                                          << " at passport count "
                                          << passports_checked + 1
                                          << std::endl;
                                invalid = true;
                                break;
                            }
                            if (pid_char_int > 57) {
                                std::cout << "invalid char " << pid_char
                                          << "in pid str " << pid_str
                                          << " at passport count "
                                          << passports_checked + 1
                                          << std::endl;
                                invalid = true;
                                break;
                            }
                        }
                        if (invalid) {
                            invalid_passport_count++;
                            passport_info.clear();
                            passports_checked++;
                            continue;
                        }
                    }
                }
                passport_info.clear();
                passports_checked++;
            }
        }
        //  Need to increment by one since the final line does not
        //  have an empty string.
        passports_checked++;
        input_file.close();
    }
    std::cout << "Total passport count: " << passports_checked
              << std::endl;
    std::cout << "Invalid passport count : " << invalid_passport_count
              << std::endl;
    std::cout << "Number of valid passports: "
              << (passports_checked - invalid_passport_count)
              << std::endl;
}

int main(int argc, char* argv[]) {
    std::string filename = "";
    bool validate = false;
    std::string usage = "Help/Usage Example: ";
    std::string script_name = "day_1_report_repair.o ";
    std::string args = "-f <filename> ";
    std::string bools = "-v -h";
    usage.append(filename);
    usage.append(args);
    usage.append(bools);
    int cmd;
    while ((cmd = getopt_long(argc, argv, optstring, longopts, NULL)) != -1) {
        switch (cmd) {
            case 'f':
                if (optarg) filename = optarg;
                break;
            case 'v':
                validate = true;
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

    validate_passports(filename, validate);

    return 0;
}
