#include <iostream>
#include <string>

std::string translatePythonToCpp(const std::string& pythonCode) {
    std::string cppCode;
    size_t pos = 0;

    while (pos < pythonCode.length()) {
        size_t end = pythonCode.find('\n', pos);

        if (end == std::string::npos)
            end = pythonCode.length();

        std::string line = pythonCode.substr(pos, end - pos);

        // Variable declaration translation
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
            std::string message = line.substr(openParenPos + 1, closeParenPos - openParenPos - 2);
            cppCode += "std::cout << " + message + " << std::endl;\n";
        }
        // If statement translation
        else if (line.find("if ") != std::string::npos) {
            size_t colonPos = line.find(':');
            std::string condition = line.substr(3, colonPos - 3);
            cppCode += "if (" + condition + ") {\n";
        }
        // While loop translation
        else if (line.find("while ") != std::string::npos) {
            size_t colonPos = line.find(':');
            std::string condition = line.substr(6, colonPos - 6);
            cppCode += "while (" + condition + ") {\n";
        }
        // Arithmetic operation translation
        else if (line.find('+') != std::string::npos || line.find('-') != std::string::npos ||
                 line.find('*') != std::string::npos || line.find('/') != std::string::npos) {
            cppCode += line + ";\n";
        }
        // Add more translation rules for other statements as needed

        pos = end + 1;
    }

    return cppCode;
}

int main() {
    std::string pythonCode = R"(
        x = 10
        y = 20
        print("Sum:", x + y)

        if x > y:
            print("x is greater than y")
        else:
            print("y is greater than x")

        while x > 0:
            print(x)
            x -= 1
    )";

    std::string cppCode = translatePythonToCpp(pythonCode);
    std::cout << "Translated C++ code:\n" << cppCode << std::endl;

    return 0;
}
