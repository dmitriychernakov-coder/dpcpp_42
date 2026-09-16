#include <string>
#include <iostream>
#include <algorithm>

class Text {
public:
    virtual void render(const std::string& data) const {
        std::cout << data;
    }
    virtual ~Text() = default;
};

class DecoratedText : public Text {
public:
    explicit DecoratedText(Text* text) : text_(text) {}
    Text* text_;
};

class ItalicText : public DecoratedText {
public:
    explicit ItalicText(Text* text) : DecoratedText(text) {}
    void render(const std::string& data) const override {
        std::cout << "<i>";
        text_->render(data);
        std::cout << "</i>";
    }
};

class BoldText : public DecoratedText {
public:
    explicit BoldText(Text* text) : DecoratedText(text) {}
    void render(const std::string& data) const override {
        std::cout << "<b>";
        text_->render(data);
        std::cout << "</b>";
    }
};


class Paragraph : public DecoratedText {
public:
    explicit Paragraph(Text* text) : DecoratedText(text) {}
    void render(const std::string& data) const override {
        std::cout << "<p>";
        text_->render(data);
        std::cout << "</p>";
    }
};

class Reversed : public DecoratedText {
public:
    explicit Reversed(Text* text) : DecoratedText(text) {}
    void render(const std::string& data) const override {
        std::string reversed_data = data;
        std::reverse(reversed_data.begin(), reversed_data.end());
        std::cout << "<p>";
        text_->render(reversed_data);
        std::cout << "</p>";
    }
};

class Link : public DecoratedText {
public:
    explicit Link(Text* text) : DecoratedText(text) {}
    
    void render(const std::string& url, const std::string& data) const {
        std::cout << "<a href=" << url << ">";
        text_->render(data);
        std::cout << "</a>";
    }
};


int main() {
    auto text_p = new Paragraph(new Text());
    text_p->render("Hello world");
    std::cout << "\n";
    std::cout << std::endl;


    auto text_rev = new Reversed(new Text());
    text_rev->render("Hello world");
        std::cout << "\n";
    std::cout << std::endl;


    auto text_link = new Link(new Text());
    text_link->render("netology.ru", "Hello world");
    std::cout << std::endl;

    return 0;
}
