#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>
#include <sstream>
#include <random>
#include <cctype>

void addRequiredHeaders(std::string& cppCode, const std::string& line) {
    if (line.find("std::transform(") != std::string::npos) {
        cppCode += "#include <algorithm>\n";
    }
    else if (line.find("std::ostringstream") != std::string::npos) {
        cppCode += "#include <sstream>\n";
    }
    else if (line.find("std::cout") != std::string::npos)
    {
        cppCode += "#include <iostream>\n";
    }
    else if (line.find("random_device") != std::string::npos ||
            line.find("mt19937") != std::string::npos ||
            line.find("uniform_int_distribution") != std::string::npos) {
        cppCode += "#include <random>\n";
            }
}


std::string translatePythonToCpp(const std::string& pythonCode) {
    std::string cppCode;
    size_t pos = 0;

    while (pos < pythonCode.length()) {
        size_t end = pythonCode.find('\n', pos); 

        if (end == std::string::npos)
            end = pythonCode.length();

        std::string line = pythonCode.substr(pos, end - pos); 

        try {
            addRequiredHeaders(cppCode, line);
            
            if (line.find(" = ") != std::string::npos) {
                size_t equalPos = line.find(" = ");
                std::string variableName = line.substr(0, equalPos);
                std::string variableValue = line.substr(equalPos + 3);
                cppCode += "int " + variableName + " = " + variableValue + ";\n";
            }
            // Print statement translation
            else if (line.find("print(") != std::string::npos) {
                size_t openParenPos = line.find('(');
                size_t closeParenPos = line.find(')');
                if (openParenPos == std::string::npos || closeParenPos == std::string::npos)
                    throw std::runtime_error("Invalid print statement syntax");

                std::string message = line.substr(openParenPos + 1, closeParenPos - openParenPos - 1);
                std::istringstream iss(message);
                std::string token;
                std::vector<std::string> tokens;
                while (std::getline(iss, token, ',')) {
                    tokens.push_back(token);
                }
                cppCode += "std::cout";
                for (const std::string& t : tokens) {
                    cppCode += " << " + t;
                }
                cppCode += " << std::endl;\n";
            }
            // If statement translation
            else if (line.find("if ") != std::string::npos) {
                size_t colonPos = line.find(':');
                if (colonPos == std::string::npos)
                    throw std::runtime_error("Invalid if statement syntax");

                std::string condition = line.substr(3, colonPos - 3);
                cppCode += "if (" + condition + ") {\n";
            }
            // While loop translation
            else if (line.find("while ") != std::string::npos) {
                size_t colonPos = line.find(':');
                if (colonPos == std::string::npos)
                    throw std::runtime_error("Invalid while loop syntax");

                std::string condition = line.substr(6, colonPos - 6);
                cppCode += "while (" + condition + ") {\n";
            }
            // len() function translation
            else if (line.find("len(") != std::string::npos) {
                size_t openParenPos = line.find('(');
                size_t closeParenPos = line.find(')');
                if (openParenPos == std::string::npos || closeParenPos == std::string::npos)
                    throw std::runtime_error("Invalid len() function syntax");

                std::string argument = line.substr(openParenPos + 1, closeParenPos - openParenPos - 1);
                cppCode += argument + ".size()";
            }
            // range() function translation
            else if (line.find("range(") != std::string::npos) {
                size_t openParenPos = line.find('(');
                size_t closeParenPos = line.find(')');
                if (openParenPos == std::string::npos || closeParenPos == std::string::npos)
                    throw std::runtime_error("Invalid range() function syntax");

                std::string argument = line.substr(openParenPos + 1, closeParenPos - openParenPos - 1);
                cppCode += "for (int i = 0; i < " + argument + "; i++) {\n";
            }
            // upper() method translation
            else if (line.find(".upper()") != std::string::npos) {
                size_t dotPos = line.find('.');
                std::string targetVariable = line.substr(0, dotPos);
                cppCode += "std::string upperString = " + targetVariable + ";\n";
                cppCode += "std::transform(upperString.begin(), upperString.end(), upperString.begin(), ::toupper);\n";
                cppCode += "std::cout << \"Uppercase: \" << upperString << std::endl;\n";
            }
            // lower() method translation
            else if (line.find(".lower()") != std::string::npos) {
                size_t dotPos = line.find('.');
                std::string targetVariable = line.substr(0, dotPos);
                cppCode += "std::string lowerString = " + targetVariable + ";\n";
                cppCode += "std::transform(lowerString.begin(), lowerString.end(), lowerString.begin(), ::tolower);\n";
                cppCode += "std::cout << \"Lowercase: \" << lowerString << std::endl;\n";
            }
            // append() method translation
            else if (line.find(".append(") != std::string::npos) {
                size_t dotPos = line.find('.');
                size_t openParenPos = line.find('(');
                size_t closeParenPos = line.find(')');
                if (dotPos == std::string::npos || openParenPos == std::string::npos || closeParenPos == std::string::npos)
                    throw std::runtime_error("Invalid append() method syntax");

                std::string container = line.substr(0, dotPos);
                std::string value = line.substr(openParenPos + 1, closeParenPos - openParenPos - 1);
                cppCode += container + ".push_back(" + value + ");";
            }
            // pop() method translation
            else if (line.find(".pop()") != std::string::npos) {
                size_t dotPos = line.find('.');
                cppCode += line.substr(0, dotPos) + ".pop_back()";
            }
            // format() method translation
            else if (line.find(".format(") != std::string::npos) {
                size_t openParenPos = line.find('(');
                size_t closeParenPos = line.find(')');
                if (openParenPos == std::string::npos || closeParenPos == std::string::npos)
                    throw std::runtime_error("Invalid format() method syntax");

                std::string formatString = line.substr(openParenPos + 1, closeParenPos - openParenPos - 1);
                std::string formattedOutput = "";

                size_t startPos = 0;
                size_t placeholderPos = formatString.find('{}', startPos);
                while (placeholderPos != std::string::npos) {
                    formattedOutput += formatString.substr(startPos, placeholderPos - startPos);
                    formattedOutput += " << ";

                    size_t closingBracketPos = formatString.find('}', placeholderPos + 2);
                    if (closingBracketPos == std::string::npos)
                        throw std::runtime_error("Invalid placeholder syntax");

                    std::string placeholder = formatString.substr(placeholderPos + 2, closingBracketPos - placeholderPos - 2);
                    formattedOutput += placeholder;

                    startPos = closingBracketPos + 1;
                    placeholderPos = formatString.find('{}', startPos);
                }
                
                formattedOutput += formatString.substr(startPos);
                cppCode += "std::cout << " + formattedOutput + " << std::endl;\n";
            }
        
            // join() method translation
            else if (line.find(".join(") != std::string::npos) {
                size_t dotPos = line.find('.');
                size_t openParenPos = line.find('(');
                size_t closeParenPos = line.find(')');
                if (dotPos == std::string::npos || openParenPos == std::string::npos || closeParenPos == std::string::npos)
                    throw std::runtime_error("Invalid join() method syntax");

                std::string delimiter = line.substr(openParenPos + 1, closeParenPos - openParenPos - 1);
                std::string container = line.substr(0, dotPos);
                cppCode += "std::ostringstream oss;\n";
                cppCode += "for (size_t i = 0; i < " + container + ".size(); i++) {\n";
                cppCode += "  if (i != 0)\n";
                cppCode += "    oss << " + delimiter + ";\n";
                cppCode += "  oss << " + container + "[i];\n";
                cppCode += "}\n";
                cppCode += "std::cout << oss.str() << std::endl;\n";
            }
            // split() method translation
            else if (line.find(".split()") != std::string::npos) {
                size_t dotPos = line.find('.');
                cppCode += "std::istringstream iss" + line.substr(0, dotPos) + ";\n";
                cppCode += "std::string token;\n";
                cppCode += "while (std::getline(iss" + line.substr(0, dotPos) + ", token, ' ')) {\n";
                cppCode += "  std::cout << token << std::endl;\n";
                cppCode += "}\n";
            }
            // random() module translation
            else if (line.find("random.") != std::string::npos) {
                size_t dotPos = line.find('.');
                cppCode += "std::random_device rd;\n";
                cppCode += "std::mt19937 gen(rd());\n";

                if (line.find("random.randint(") != std::string::npos) {
                    size_t openParenPos = line.find('(');
                    size_t commaPos = line.find(',');
                    size_t closeParenPos = line.find(')');
                    if (openParenPos == std::string::npos || commaPos == std::string::npos || closeParenPos == std::string::npos)
                        throw std::runtime_error("Invalid random.randint() function syntax");

                    std::string lower = line.substr(openParenPos + 1, commaPos - openParenPos - 1);
                    std::string upper = line.substr(commaPos + 1, closeParenPos - commaPos - 1);
                    cppCode += "std::uniform_int_distribution<int> dist(" + lower + ", " + upper + ");\n";
                    cppCode += "std::cout << dist(gen);\n";
                }
            }
            // Arithmetic operations translation
            else if (line.find('+') != std::string::npos ||
                     line.find('-') != std::string::npos ||
                     line.find('*') != std::string::npos ||
                     line.find('/') != std::string::npos ||
                     line.find('%') != std::string::npos) {
                cppCode += line + "\n";
            }
            // Logical operator translation
            else if (line.find(" and ") != std::string::npos) {
                size_t andPos = line.find(" and ");
                std::string leftOperand = line.substr(0, andPos);
                std::string rightOperand = line.substr(andPos + 5);
                cppCode += "(" + leftOperand + ") && (" + rightOperand + ")";
            }
            else if (line.find(" or ") != std::string::npos) {
                size_t orPos = line.find(" or ");
                std::string leftOperand = line.substr(0, orPos);
                std::string rightOperand = line.substr(orPos + 4);
                cppCode += "(" + leftOperand + ") || (" + rightOperand + ")";
            }
            // Invalid syntax
            else {
                throw std::runtime_error("Invalid syntax");
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }

        pos = end + 1;
    }

    return cppCode;
}

int main() {
    std::string pythonCode = R"(

# Variable declaration
x = 5
y = 10

# Print statement
print("Sum:", x + y)

# If statement
if x > y:
    print("x is greater than y")
else:
    print("x is not greater than y")

# While loop
while x < 10:
    print(x)
    x += 1

# len() function
myList = [1, 2, 3, 4, 5]
length = len(myList)
print("Length:", length)

# range() function
for i in range(5):
    print(i)

# upper() method
myString = "hello"
upperString = myString.upper()
print("Uppercase:", upperString)

# lower() method
lowerString = upperString.lower()
print("Lowercase:", lowerString)

# append() method
myList.append(6)
print("Updated List:", myList)

# pop() method
myList.pop()
print("Updated List:", myList)

# format() method
name = "Alice"
age = 25
print("My name is {} and I'm {} years old".format(name, age))

# join() method
myList = ["Hello", "world"]
joinedString = "-".join(myList)
print("Joined String:", joinedString)

# split() method
myString = "Hello world"
splittedString = myString.split()
for word in splittedString:
    print(word)

# random() module
import random
randomNumber = random.randint(1, 10)
print("Random number:", randomNumber)

# and or operator
x > 5 and y < 10

    )";

    std::string cppCode = translatePythonToCpp(pythonCode);
    std::cout << cppCode;

    return 0;
}
