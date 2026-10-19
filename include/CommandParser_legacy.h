#pragma once
#include <string>

class CommandParser {
public:

    struct Command {
        std::string verb;
        std::string argument;
    };

    Command parse(const std::string& input) const;
};
