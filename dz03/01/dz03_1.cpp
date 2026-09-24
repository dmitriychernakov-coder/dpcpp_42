#include <iostream>
#include <string>
#include <fstream>
#include <memory>

class LogCommand {
public:
    virtual ~LogCommand() = default;
    virtual void print(const std::string& message) = 0;
};

class ConsoleLog : public LogCommand {
public:
    void print(const std::string& message) {
        std::cout << message << std::endl;
    }
};

class FileLog : public LogCommand {
    std::string fpath;
public:
    FileLog(std::string file_path) : fpath(std::move(file_path)) {}

    void print(const std::string& message) {
        std::ofstream file(fpath, std::ios::app);
        if (file.is_open()) {
            file << message << std::endl;;
            std::cout << fpath<< std::endl;
        } else {
            std::cout << "Not possible write " << fpath << std::endl;
        }
    }
};

void print(LogCommand& cmd) {
    cmd.print("Message ");
}

int main() {
    ConsoleLog console_log;
    FileLog file_log("log.txt");

    std::cout << "Print console" << std::endl;
    print(console_log);

    std::cout << "Print file" << std::endl;
    print(file_log);

    return 0;
}
