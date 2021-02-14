#include <iostream>
#include <string>
#include <regex>
#include <cmath>

std::string tobinary(const std::string& decimal);
std::string todecimal(const std::string& binary);
std::string bigint(const std::string& decimal);

int main(int argc, char* argv[]) {

    if (argc == 3) {
        std::string arg1 = argv[1];
        std::string arg2 = argv[2];

        if ((arg1 == "-b") || (arg1 == "--binary")) {
            std::cout << tobinary(arg2) << "\n";
            exit (0);
        }

        if ((arg1 == "-d") || (arg1 == "--decimal")) {
            std::cout << todecimal(arg2) << "\n";
            exit (0);
        }

        if (arg1 == "--bigint") {
            std::cout << bigint(arg2);
            exit(0);
        }

        std::string man_main = "Usage: \n"
                               "\t --binary -b <ip>            Translating your ip address into binary view.\n"
                               "\t --decimal -d <binary_ip>    Translating your binary id address into decimal view.\n"
                               "\t --bigint <ip>               The assignment itself. <ip> is a decimal ip, which will be converted into a glued binary and decimal number\n";
        std::cout << man_main;
        return 0;
    }

    std::string man_main = "Usage: \n"
                           "\t --binary -b <ip>            Translating your ip address into binary view.\n"
                           "\t --decimal -d <binary_ip>    Translating your binary id address into decimal view.\n";
    std::cout << man_main;
    return 0;
}

std::string tobinary(const std::string& decimal) {
    try {
        std::regex ip_regex (R"(([1](\d\d)|(2\d\d)|(\d?\d))\.([1](\d\d)|(2\d\d)|(\d?\d))\.([1](\d\d)|(2\d\d)|(\d?\d))\.([1](\d\d)|(2\d\d)|(\d?\d)))");
        if (std::regex_match(decimal, ip_regex)) {
            std::string output;
            std::string ipbinary[4];
            std::string subdecimal = decimal;
            for (int i = 0; i < 4; ++i) {
                //----- < Parsing > -----
                unsigned char byte = std::stoi(subdecimal.substr(0, subdecimal.find('.')));
                subdecimal = subdecimal.substr(subdecimal.find('.' ) + 1);

                //----- < Converting > -----
                unsigned char maxPow = std::floor(std::log2(byte));
                for (int j = 0; j < (7 - maxPow); ++j) {
                    ipbinary[i] += "0";
                }
                while (byte > 0) {
                    auto powered = (int)pow(2, maxPow);
                    if (byte < powered) {
                        --maxPow;
                        ipbinary[i] += "0";
                    } else {
                        byte -= powered;
                        --maxPow;
                        ipbinary[i] += "1";
                    }
                }
                std::string *currentbyte = &ipbinary[i];
                if (currentbyte->length() != 8) {
                    int originalLength = currentbyte->length();
                    for (int j = 0; j < (8 - originalLength); ++j) {
                        ipbinary[i] += "0";
                    }
                }

                //----- < Linking > -----
                output += ipbinary[i];
                if (i != 3) {
                    output += ".";
                }
            }
            return output;
        } else {
            throw std::runtime_error("Incorrect ip address");
        }
    } catch (std::regex_error &regexError) {
        std::cout << regexError.what();
        exit(-1);
    }
    return "";
}

std::string todecimal(const std::string& binary) {
    try {
        std::regex ip_regex (R"([01]{8}\.[01]{8}\.[01]{8}\.[01]{8})");
        if (std::regex_match(binary, ip_regex)) {
            std::string output;
            std::string ipbinary[4];
            std::string subbinary = binary;
            for (int i = 0; i < 4; ++i) {
                //----- < Parsing > -----
                std::string byte = subbinary.substr(0, subbinary.find('.'));
                subbinary = subbinary.substr(subbinary.find('.') + 1);

                //----- < Converting > -----
                int byteInt = 0;
                for (int i = 0; i < byte.length(); ++i) {
                    std::string digit = byte.substr(i, 1);
                    byteInt += std::stoi(digit) * pow (2, 7 - i);
                }
                output += std::to_string(byteInt);
                if (i != 3) {
                    output += ".";
                }
            }
            return output;
        } else {
            throw std::runtime_error("Incorrect binary ip address");
        }
    } catch (std::regex_error &ex) {
        std::cout << ex.what();
    }

    return "";
}

std::string bigint(const std::string& decimal) {
    std::string binary_ip = tobinary(decimal);
    std::string output;
    binary_ip.erase(std::remove(binary_ip.begin(), binary_ip.end(), '.'), binary_ip.end());
    output += binary_ip;

    unsigned int converted_number = 0;
    for (int i = 0; i < binary_ip.length(); ++i) {
        std::string digit = binary_ip.substr(i, 1);
        converted_number += std::stoi(digit) * (unsigned int)pow(2, 31 - i);
    }

    output += "\n" + std::to_string(converted_number);
    return output;
}