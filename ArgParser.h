#pragma once

#include <cstring>
#include <string>
#include <vector>
#include <variant>
#include <stdexcept>
#include <sstream>

int ZERO = 48;
int NINE = 57;
std::vector<std::string> chack;

namespace ArgumentParser {

    struct the_help {
        std::vector<std::string> names_possible;
        bool active_status = false;

        the_help() {

        }
    };

    struct flags {
        std::vector<std::string> names_possible;
        bool active_status = false;
        bool* storing_value;
        bool need_to_store = false;

        flags() {

        }

//        flags(std::string& the_flag) {
//            names_possible.push_back(the_flag);
//        }
//
//        flags(std::string& the_flag, std::string& the_flag2) {
//            names_possible.push_back(the_flag);
//            names_possible.push_back(the_flag2);
//        }
//
//        flags(char& the_flag, std::string& the_flag2) {
//            std::string the_flag_str;
//            the_flag_str += the_flag;
//            names_possible.push_back(the_flag_str);
//            names_possible.push_back(the_flag2);
//        }

        flags& Default(bool default_value) {
            active_status = default_value;
            return *this;
        }

        flags& StoreValue(bool& basic_bool) {
            storing_value = &basic_bool;
            need_to_store = true;
            return *this;
        }
    };

    struct arguments {
        std::vector<std::string> names_possible;
        std::vector<std::string> meanings_of_values;
        std::string* storing_value;
        bool need_to_store = false;
        int multivalues = 1;
        bool positional = false;

        arguments(std::string& name) {
            names_possible.push_back(name);
        }

        arguments(char name1, std::string& name2) {
            std::string name1_str;
            name1_str += name1;
            names_possible.push_back(name1_str);
            names_possible.push_back(name2);
        }

        arguments& StoreValue(std::string& basic_string_1) {
            storing_value = &basic_string_1;
            need_to_store = true;
            return *this;
        }

        void restoring() {
            *storing_value = meanings_of_values[0];
        }

        arguments& Default(std::string default_value) {
            meanings_of_values.push_back(default_value);
            return *this;
        }

        arguments& MultiValue() {
            multivalues = -1;
            return *this;
        }

        arguments& MultiValue(int number_of_values) {
            multivalues = number_of_values;
            return *this;
        }

        arguments& Positional() {
            positional = true;
            return *this;
        }
    };

    struct arguments_int {
        std::vector<std::string> names_possible;
        std::vector<int> meanings_of_values;
        bool need_to_store = false;
        std::vector<int>* storing_value_vector;
        bool need_to_store_vector = false;
        int* storing_value;
        int multivalues = 1;
        bool positional = false;

        arguments_int(std::string& name) {
            names_possible.push_back(name);
        }

        arguments_int(char name1, std::string& name2) {
            std::string name1_str;
            name1_str += name1;
            names_possible.push_back(name1_str);
            names_possible.push_back(name2);
        }

        arguments_int& StoreValue(int& basic_int) {
            storing_value = &basic_int;
            need_to_store = true;
            return *this;
        }

        arguments_int& StoreValues(std::vector<int>& basic_int) {
            storing_value_vector = &basic_int;
            need_to_store_vector = true;
            return *this;
        }

        void restoring() {
            *storing_value = meanings_of_values[0];
        }

        void restoring_vector() {
            *storing_value_vector = meanings_of_values;
        }

        arguments_int& Default(int default_value) {
            meanings_of_values.push_back(default_value);
            return *this;
        }

        arguments_int& MultiValue() {
            multivalues = -1;
            return *this;
        }

        arguments_int& MultiValue(int number_of_values) {
            multivalues = number_of_values;
            return *this;
        }

        arguments_int& Positional() {
            positional = true;
            return *this;
        }
    };

    class ArgParser {
     private:
        std::string name;
        std::vector<arguments> values;
        std::vector<arguments_int> values_int;
        std::vector<flags> values_flags;
        the_help The_Helper;

     public:
        ArgParser(std::string input_name) {
            name = input_name;
        }

        bool Parse(std::vector<std::string> parsed_line, std::variant<int, int*> int_get) {
            int i = 1;
            while (i < parsed_line.size()) {
                std::string temp = parsed_line[i];
                while (temp[0] == '-') {
                    temp.erase(0, 1);
                }
                if (temp.find('=') != std::string::npos) {
                    //std::stringstream ss(temp);

                    std::string name_of_argument = temp.substr(0, temp.find('='));
                    std::string meaning_of_argument_given = temp.substr(temp.find('=') + 1, temp.size());
                    bool in_string = false;
                    for (auto j : values) {
                        for (auto k : j.names_possible) {
                            if (strcmp(k.c_str(), name_of_argument.c_str()) == 0) {
                                j.meanings_of_values.push_back(meaning_of_argument_given);
                                in_string = true;
                            }
                        }
                    }

                    if (!in_string) {
                        for (auto j : values_int) {
                            for (auto k : j.names_possible) {
                                if (strcmp(k.c_str(), name_of_argument.c_str()) == 0) {
                                    j.meanings_of_values.push_back(stoi(meaning_of_argument_given));
                                    chack.push_back(j.names_possible[0]);
                                }
                            }
                        }
                    }

                } else if (temp[0] < ZERO || temp[0] > NINE) {
                    for (auto j : values_flags) {
                        for (auto k : j.names_possible) {
                            if (strcmp(k.c_str(), temp.c_str()) == 0) {
                                j.active_status = true;
                            }
                        }
                    }
                    for (auto j : The_Helper.names_possible) {
                        if (strcmp(j.c_str(), temp.c_str()) == 0) {
                            The_Helper.active_status = true;
                        }
                    }

                } else {
                    if (values.size() != 0) {
                        values[0].meanings_of_values.push_back(temp);
                    } else if (values_int.size() != 0) {
                        values_int[0].meanings_of_values.push_back(std::atoi(temp.c_str()));
                    }
                }
                i++;
            }
            if (The_Helper.names_possible.size() != 0 && !The_Helper.active_status) {
                return false;
            }
            for (auto j : values) {
                if (j.multivalues != -1) {
                    if (j.meanings_of_values.size() != j.multivalues) {
                        return false;
                    }
                }
                if (j.need_to_store) {
                    j.restoring();
                }
            }
            chack.push_back(values_int[0].names_possible[0]);
            for (auto j : values_int) {
                if (j.multivalues != -1) {
                    if (j.meanings_of_values.size() != j.multivalues) {
                        chack.push_back(j.names_possible[0]);
                        return false;
                    }
                }
                if (j.need_to_store) {
                    j.restoring();
                }
                if (j.need_to_store_vector) {
                    j.restoring_vector();
                }
            }
            return true;
        }

        arguments& AddStringArgument(std::string input_argument) {
            arguments argument_number_1(input_argument);
            values.push_back(argument_number_1);
            return values[values.size() - 1];
        }

        arguments& AddStringArgument(char input_argument1, std::string input_argument2) {
            arguments argument_number_2(input_argument1, input_argument2);
            values.push_back(argument_number_2);
            return values[values.size() - 1];
        }

        arguments& AddStringArgument(char input_argument1, std::string input_argument2, std::string input_argument3) {
            arguments argument_number_2(input_argument1, input_argument2);
            argument_number_2.names_possible.push_back(input_argument3);
            values.push_back(argument_number_2);
            return values[values.size() - 1];
        }

        arguments_int& AddIntArgument(std::string input_argument) {
            arguments_int argument_number_1(input_argument);
            values_int.push_back(argument_number_1);
            return values_int[values_int.size() - 1];
        }

        arguments_int& AddIntArgument(char input_argument1, std::string input_argument2) {
            arguments_int argument_number_2(input_argument1, input_argument2);
            values_int.push_back(argument_number_2);
            return values_int[values_int.size() - 1];
        }

        arguments_int& AddIntArgument(std::string input_argument, std::string input_argument2) {
            arguments_int argument_number_1(input_argument);
            argument_number_1.names_possible.push_back(input_argument2);
            values_int.push_back(argument_number_1);
            return values_int[values_int.size() - 1];
        }

        std::string GetStringValue(std::string argument_name) {
            for (auto j : values) {
                for (auto k : j.names_possible) {
                    if (strcmp(k.c_str(), argument_name.c_str()) == 0) {
                        return j.meanings_of_values[0];
                    }
                }
            }
            throw std::runtime_error("Could not find the value");
        }

        int GetIntValue(std::string argument_name) {
            for (auto j : values_int) {
                for (auto k : j.names_possible) {
                    if (strcmp(k.c_str(), argument_name.c_str()) == 0) {
                        return j.meanings_of_values[0];
                    }
                }
            }
            throw std::runtime_error("Could not find the value");
        }

        bool Gdet(std::string argument_name) {
            for (auto j : values_int) {
                for (auto k : j.names_possible) {
                    if (strcmp(k.c_str(), argument_name.c_str()) == 0) {
                        return j.meanings_of_values.size();
                    }
                }
            }
        }

        int GetIntValue(std::string argument_name, int position) {
            for (auto j : values_int) {
                for (auto k : j.names_possible) {
                    if (strcmp(k.c_str(), argument_name.c_str()) == 0) {
                        return j.meanings_of_values[position];
                    }
                }
            }
            throw std::runtime_error("Could not find the value");
        }

        flags& AddFlag(char flag1 = '0', std::string flag2 = "", std::string flag3 = "") {
            flags new_flag;
            if (flag1 != 0) {
                std::string flag1_str;
                flag1_str += flag1;
                new_flag.names_possible.push_back(flag1_str);
            }
            if (flag2 != "") {
                new_flag.names_possible.push_back(flag2);
            }
            if (flag3 != "") {
                new_flag.names_possible.push_back(flag3);
            }
            values_flags.push_back(new_flag);
            return values_flags[values_flags.size() - 1];
        }

        flags& AddFlag(std::string flag2 = "", std::string flag3 = "") {
            flags new_flag;
            new_flag.names_possible.push_back(flag2);
            new_flag.names_possible.push_back(flag3);
            values_flags.push_back(new_flag);
            return values_flags[values_flags.size() - 1];
        }

//        flags AddFlag(std::string flag1, std::string flag2) {
//            flags new_flag(flag1, flag2);
//            values_flags.push_back(new_flag);
//            return values_flags[values_flags.size() - 1];
//        }
//
//        flags AddFlag(char flag1, std::string flag2) {
//            flags new_flag(flag1, flag2);
//            values_flags.push_back(new_flag);
//            return values_flags[values_flags.size() - 1];
//        }

        bool GetFlag(std::string flag_name) {
            for (auto j : values_flags) {
                for (auto k : j.names_possible) {
                    if (strcmp(k.c_str(), flag_name.c_str()) == 0) {
                        return j.active_status;
                    }
                }
            }
            throw std::runtime_error("Could not find the value");
        }

        void AddHelp(char help1 = 0, std::string help2 = "", std::string help3 = "") {
            the_help new_helper;
            if (help1 != 0) {
                std::string help1_str;
                help1_str += help1;
                new_helper.names_possible.push_back(help1_str);
            }
            if (help2 != "") {
                new_helper.names_possible.push_back(help2);
            }
            if (help3 != "") {
                new_helper.names_possible.push_back(help2);
            }
            The_Helper = new_helper;
        }

        bool Help() {
            return The_Helper.active_status;
        }

        std::string HelpDescription() {
            return "Smthn good";
        }

    };

} // namespace ArgumentParser