#include <iostream>
#include <memory>
#include <fstream>
#include <stdexcept>

enum class Type {
    WARNING,
    ERROR,
    FATAL_ERROR,
    UNKNOW
};

class LogMessage {
private:
    Type type_;
    std::string message_;
public:
    LogMessage(Type type, std::string message) 
        : type_(type), message_(std::move(message)) {}

    Type type() const { return type_; }
    const std::string& message() const { return message_; }
};

class Logger {
protected:
    std::shared_ptr<Logger> next_;
public:
    virtual ~Logger() = default;

    void setNext(std::shared_ptr<Logger> next) { 
        next_ = next; 
    }

    void log(const LogMessage& message) {
        if (canHandle(message.type())) {
            handle(message);
        } else if (next_) {
            next_->log(message);
        }
    }
    virtual bool canHandle(Type type) const = 0;
    virtual void handle(const LogMessage& msg) = 0;
};

class FatalErrorLog : public Logger {
public:
    bool canHandle(Type type) const override { 
        return type == Type::FATAL_ERROR; 
    }
    void handle(const LogMessage& msg) override {
        throw std::runtime_error("Fatal Error: " + msg.message());
    }
};

class FileErrorLog : public Logger {
private:
    std::string filePath_;
public:
    FileErrorLog(std::string filePath) : filePath_(std::move(filePath)) {}

    bool canHandle(Type type) const override { 
        return type == Type::ERROR; 
    }
    void handle(const LogMessage& msg) override {
        std::ofstream file(filePath_, std::ios::app);
        if (file.is_open()) {
            file << "Error: " << msg.message() << "\n";
            std::cout << "Error! See file " << filePath_ << "\n";
        } else {
            std::cerr << "NO write file " << filePath_ << "\n";
        }
    }
};

class WarningLog : public Logger {
public:
    bool canHandle(Type type) const override { 
        return type == Type::WARNING; 
    }
    void handle(const LogMessage& msg) override {
        std::cout << "Warning: " << msg.message() << "\n";
    }
};

class UnknowLog : public Logger {
public:
    bool canHandle(Type type) const override { 
        return type == Type::UNKNOW; 
    }
    void handle(const LogMessage& msg) override {
        throw std::runtime_error("Undefined message: " + msg.message());
    }
};

int main() {
    auto fatal = std::make_shared<FatalErrorLog>();
    auto error = std::make_shared<FileErrorLog>("error.log");
    auto warning = std::make_shared<WarningLog>();
    auto unknow = std::make_shared<UnknowLog>();

    fatal->setNext(error);
    error->setNext(warning);
    warning->setNext(unknow);

    fatal->log(LogMessage(Type::WARNING, "WARNING!"));
    fatal->log(LogMessage(Type::ERROR, "ERROR!"));
    
    try {
        fatal->log(LogMessage(Type::FATAL_ERROR, "FATAL ERROR!"));
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }
    
    try {
        fatal->log(LogMessage(Type::UNKNOW, "UNKNOWN!"));
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }



    return 0;
}
