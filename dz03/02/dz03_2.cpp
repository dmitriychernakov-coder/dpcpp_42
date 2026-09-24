#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <algorithm>


class Observer {
public:
    virtual ~Observer() = default;
    virtual void onWarning(const std::string& message) {}
    virtual void onError(const std::string& message) {}
    virtual void onFatalError(const std::string& message) {}
};

class Observed {
    std::vector<std::weak_ptr<Observer>> observers_;

public:
    void addObserver(std::weak_ptr<Observer> observer) {
        observers_.push_back(observer);
    }

    void warning(const std::string& message) {
        for (auto& obs : observers_) {
            if (auto observer = obs.lock()) {
                observer->onWarning(message);
            }
        }
    }

    void error(const std::string& message) {
        for (auto& obs : observers_) {
            if (auto observer = obs.lock()) {
                observer->onError(message);
            }
        }
    }

    void fatalError(const std::string& message) {
        for (auto& obs : observers_) {
            if (auto observer = obs.lock()) {
                observer->onFatalError(message);
            }
        }
    }
};

class ConsoleWarning : public Observer {
public:
    void onWarning(const std::string& message) override {
        std::cout << "[WARNING] " << message << "\n";
    }
};

class FileError : public Observer {
    std::string filePath_;
public:
    explicit FileError(std::string filePath) : filePath_(std::move(filePath)) {}

    void onError(const std::string& message) override {
        std::ofstream file(filePath_, std::ios::app);
        if (file.is_open()) {
            file << "[ERROR] " << message << "\n";
        }
    }
};

class FatalError : public Observer {
    ConsoleWarning consoleWarning_;
    FileError fileError_;

public:
    FatalError(std::string filePath) : fileError_(std::move(filePath)) {}

    void onFatalError(const std::string& message) override {
        consoleWarning_.onWarning(message); 
        fileError_.onError(message);
    }
};


int main() {
    Observed obs;
    auto warningCons = std::make_shared<ConsoleWarning>();
    auto errorLog = std::make_shared<FileError>("error.log");
    auto fatalLog = std::make_shared<FatalError>("fatal_error.log");

    obs.addObserver(warningCons);
    obs.addObserver(errorLog);
    obs.addObserver(fatalLog);

    obs.warning("Warning");
    obs.error("Error");
    obs.fatalError("Fatal error");

    return 0;
}
