#include <iostream>
#include <string>

class animal {
public:
    // Constructor
    animal(std::string name, int age) : name(name), age(age) {}

    // Member function to display information
    void displayInfo() {
        std::cout << "Name: " << name << ", Age: " << age << std::endl;
    }

    // Getter for age
    int getAge() const {
        return age;
    }

private:
    // Private member variables
    std::string name;
    int age;
};

int main() {
    // Creating an instance of the Person class
    animal animal1("John Doe", 25);

    // Accessing member functions
    animal1.displayInfo();
    std::cout << "Age: " << animal1.getAge() << std::endl;

    return 0;
    
}
