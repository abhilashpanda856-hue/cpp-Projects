#include <SFML/Graphics.hpp>
#include <optional>
#include <exception>
#include <string>
#include <vector>


//  EXCEPTION HANDLING

class DisconnectedPinException : public std::exception {
private:
    std::string message;
public:
    explicit DisconnectedPinException(const std::string& gateName) 
        : message("Error: Disconnected input pin detected at " + gateName) {}
    
    const char* what() const noexcept override {
        return message.c_str();
    }
};


//  INHERITANCE: BASE CLASS

class LogicGate {
protected:
    std::string name;
    sf::RectangleShape shape;
    LogicGate* input1;
    LogicGate* input2;
    int maxInputs;
    int connectedInputs;

public:
    LogicGate(std::string gateName, int inputs, sf::Vector2f position) 
        : name(gateName), maxInputs(inputs), connectedInputs(0), input1(nullptr), input2(nullptr) 
    {
        shape.setSize(sf::Vector2f(100.f, 60.f)); 
        shape.setPosition(position);
        shape.setOutlineThickness(4.f);
        shape.setOutlineColor(sf::Color::Red); // Default output is false (Red)
    }

    virtual ~LogicGate() = default;

    virtual bool evaluate() = 0; 

    
    //  OPERATOR OVERLOADING (Connecting Gates)
   
    LogicGate& operator>>(LogicGate& receiver) {
        if (receiver.connectedInputs == 0 && receiver.maxInputs > 0) {
            receiver.input1 = this;
            receiver.connectedInputs++;
        } 
        else if (receiver.connectedInputs == 1 && receiver.maxInputs > 1) {
            receiver.input2 = this;
            receiver.connectedInputs++;
        }
        return receiver;
    }

    void draw(sf::RenderWindow& window) {
        try {
            bool outputState = evaluate();
            shape.setOutlineColor(outputState ? sf::Color::Green : sf::Color::Red);
        } 
        catch (const DisconnectedPinException& e) {
            shape.setOutlineColor(sf::Color::Yellow);
        }
        window.draw(shape);
    }
};


// INHERITANCE: DERIVED CLASSES

class Switch : public LogicGate {
private:
    bool state;
public:
    Switch(sf::Vector2f pos) : LogicGate("Switch", 0, pos), state(false) {
        shape.setFillColor(sf::Color(100, 100, 100));
    }
    bool evaluate() override { return state; }
    void toggle() { state = !state; }
};

class AndGate : public LogicGate {
public:
    AndGate(sf::Vector2f pos) : LogicGate("AND Gate", 2, pos) {
        shape.setFillColor(sf::Color(50, 100, 200)); 
    }
    bool evaluate() override {
        if (input1 == nullptr || input2 == nullptr) throw DisconnectedPinException(name);
        return input1->evaluate() && input2->evaluate();
    }
};

class OrGate : public LogicGate {
public:
    OrGate(sf::Vector2f pos) : LogicGate("OR Gate", 2, pos) {
        shape.setFillColor(sf::Color(200, 100, 50)); 
    }
    bool evaluate() override {
        if (input1 == nullptr || input2 == nullptr) throw DisconnectedPinException(name);
        return input1->evaluate() || input2->evaluate();
    }
};

class NotGate : public LogicGate {
public:
    NotGate(sf::Vector2f pos) : LogicGate("NOT Gate", 1, pos) {
        shape.setFillColor(sf::Color(200, 50, 50)); 
    }
    bool evaluate() override {
        if (input1 == nullptr) throw DisconnectedPinException(name);
        return !(input1->evaluate()); 
    }
};

class XorGate : public LogicGate {
public:
    XorGate(sf::Vector2f pos) : LogicGate("XOR Gate", 2, pos) {
        shape.setFillColor(sf::Color(150, 50, 200)); 
    }
    bool evaluate() override {
        if (input1 == nullptr || input2 == nullptr) throw DisconnectedPinException(name);
        return input1->evaluate() != input2->evaluate(); 
    }
};

class NandGate : public LogicGate {
public:
    NandGate(sf::Vector2f pos) : LogicGate("NAND Gate", 2, pos) {
        shape.setFillColor(sf::Color(50, 150, 200)); 
    }
    bool evaluate() override {
        if (input1 == nullptr || input2 == nullptr) throw DisconnectedPinException(name);
        return !(input1->evaluate() && input2->evaluate());
    }
};


//  MAIN APPLICATION (THE SANDBOX)

int main() {
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(800, 600)), "Logic Circuit Simulator");
    window.setFramerateLimit(60);

    // Instantiate our Input Switches
    Switch switch1(sf::Vector2f(100.f, 150.f));
    Switch switch2(sf::Vector2f(100.f, 350.f));

    // Instantiate all available Logic Gates
    AndGate  andGate(sf::Vector2f(400.f, 250.f));
    OrGate   orGate(sf::Vector2f(400.f, 250.f));
    NotGate  notGate(sf::Vector2f(400.f, 250.f));
    XorGate  xorGate(sf::Vector2f(400.f, 250.f));
    NandGate nandGate(sf::Vector2f(400.f, 250.f));

    // =========================================================
    //  CIRCUIT WIRING SANDBOX
    // Uncomment ONLY ONE of the configurations below to test it!
    // =========================================================

    // CONFIG 1: Standard AND Gate (Default)
    LogicGate* activeGate = &andGate;
    switch1 >> andGate;
    switch2 >> andGate;

    // CONFIG 2: The OR Gate
    // LogicGate* activeGate = &orGate;
    // switch1 >> orGate;
    // switch2 >> orGate;

    // CONFIG 3: The NOT Gate (Only needs 1 switch!)
    // LogicGate* activeGate = &notGate;
    // switch1 >> notGate;

    // CONFIG 4: The XOR Gate
    // LogicGate* activeGate = &xorGate;
    // switch1 >> xorGate;
    // switch2 >> xorGate;

    // =========================================================

    bool key1WasPressed = false;
    bool key2WasPressed = false;

    while (window.isOpen()) {
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        // Toggle switches with keyboard (1 and 2)
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num1)) {
            if (!key1WasPressed) { switch1.toggle(); key1WasPressed = true; }
        } else { key1WasPressed = false; }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num2)) {
            if (!key2WasPressed) { switch2.toggle(); key2WasPressed = true; }
        } else { key2WasPressed = false; }

        window.clear(sf::Color(30, 30, 30));

        // Draw gates 
        switch1.draw(window);
        switch2.draw(window);
        activeGate->draw(window); 

        window.display();
    }

    return 0;
}