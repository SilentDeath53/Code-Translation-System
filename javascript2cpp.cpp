#include <iostream>
#include <string>
#include <map>

std::string translateJavaScriptToCpp(const std::string& jsCode) {
    std::string cppCode;

    cppCode = jsCode;

    // Rule: Replace 'let' with 'int'
    size_t pos = cppCode.find("let");
    while (pos != std::string::npos) {
        cppCode.replace(pos, 3, "int");
        pos = cppCode.find("let", pos + 3);
    }

    // Rule: Replace 'const' with 'const int'
    pos = cppCode.find("const");
    while (pos != std::string::npos) {
        cppCode.replace(pos, 5, "const int");
        pos = cppCode.find("const", pos + 9);
    }

    // Rule: Replace 'var' with 'auto'
    pos = cppCode.find("var");
    while (pos != std::string::npos) {
        cppCode.replace(pos, 3, "auto");
        pos = cppCode.find("var", pos + 4);
    }

    // Rule: Replace 'console.log' with 'std::cout <<'
    pos = cppCode.find("console.log");
    while (pos != std::string::npos) {
        size_t openParenPos = cppCode.find('(', pos);
        size_t closeParenPos = cppCode.find(')', openParenPos);
        if (openParenPos == std::string::npos || closeParenPos == std::string::npos)
            throw std::runtime_error("Invalid print statement syntax");

        std::string message = cppCode.substr(openParenPos + 1, closeParenPos - openParenPos - 1);
        cppCode.replace(pos, closeParenPos - pos + 1, "std::cout << " + message + " << std::endl;");
        
        pos = cppCode.find("console.log", pos + 1);
    }

    // Rule: String Length
    pos = cppCode.find(".length");
    while (pos != std::string::npos) {
        cppCode.replace(pos, 7, ".size()");
        pos = cppCode.find(".length", pos + 7);
    }

    // Rule: Array Length
    pos = cppCode.find(".length");
    while (pos != std::string::npos) {
        cppCode.replace(pos, 7, ".size()");
        pos = cppCode.find(".length", pos + 7);
    }

    // Rule: Array Access
    pos = cppCode.find("[");
    while (pos != std::string::npos) {
        size_t closingBracketPos = cppCode.find("]", pos + 1);
        cppCode.replace(pos, closingBracketPos - pos + 1, cppCode.substr(pos, closingBracketPos - pos + 1));
        pos = cppCode.find("[", pos + 1);
    }

    // Rule: String Concatenation
    pos = cppCode.find("+");
    while (pos != std::string::npos) {
        cppCode.replace(pos, 1, " + ");
        pos = cppCode.find("+", pos + 3);
    }

    // Rule: String Comparison
    pos = cppCode.find("===");
    while (pos != std::string::npos) {
        cppCode.replace(pos, 3, "==");
        pos = cppCode.find("===", pos + 2);
    }

    // Rule: For Loop
    pos = cppCode.find("for");
    while (pos != std::string::npos) {
        size_t openingParenPos = cppCode.find("(", pos);
        size_t closingParenPos = cppCode.find(")", openingParenPos);
        cppCode.replace(openingParenPos, closingParenPos - openingParenPos + 1, cppCode.substr(openingParenPos, closingParenPos - openingParenPos + 1));
        pos = cppCode.find("for", closingParenPos);
    }

    // Rule: While Loop
    pos = cppCode.find("while");
    while (pos != std::string::npos) {
        size_t openingParenPos = cppCode.find("(", pos);
        size_t closingParenPos = cppCode.find(")", openingParenPos);
        cppCode.replace(openingParenPos, closingParenPos - openingParenPos + 1, cppCode.substr(openingParenPos, closingParenPos - openingParenPos + 1));
        pos = cppCode.find("while", closingParenPos);
    }

    // Rule: Arithmetic Operators
    pos = cppCode.find("+");
    while (pos != std::string::npos) {
        cppCode.replace(pos, 1, " + ");
        pos = cppCode.find("+", pos + 3);
    }
    pos = cppCode.find("-");
    while (pos != std::string::npos) {
        cppCode.replace(pos, 1, " - ");
        pos = cppCode.find("-", pos + 3);
    }
    pos = cppCode.find("*");
    while (pos != std::string::npos) {
        cppCode.replace(pos, 1, " * ");
        pos = cppCode.find("*", pos + 3);
    }
    pos = cppCode.find("/");
    while (pos != std::string::npos) {
        cppCode.replace(pos, 1, " / ");
        pos = cppCode.find("/", pos + 3);
    }
    pos = cppCode.find("%");
    while (pos != std::string::npos) {
        cppCode.replace(pos, 1, " % ");
        pos = cppCode.find("%", pos + 3);
    }

    // Rule: Object Creation
    pos = cppCode.find("{}");
    while (pos != std::string::npos) {
        cppCode.replace(pos, 2, "std::map<std::string, int>{}");
        pos = cppCode.find("{}", pos + 23);
    }

    if (cppCode == jsCode) {
        throw std::runtime_error("Undefined translation rule");
    }

    return cppCode;
}

int main() {
    std::string jsCode = R"(
        let x = 5;
        const y = 10;
        var z = "Hello";
        console.log(x + y);
        console.error("Error occurred!");
        let myList = [1, 2, 3, 4, 5];
        console.log("Length:", myList.length);
        console.log("Array Access:", myList[2]);
        let message = "Hello";
        let name = "Alice";
        console.log(message + " " + name);
        for (let i = 0; i < 10; i++) {
            console.log("Index:", i);
        }
        while (x < 100) {
            console.log("x:", x);
            x += 10;
        }
        console.log(x / y);
        let obj = {};
    )";

    try {
        std::string cppCode = translateJavaScriptToCpp(jsCode);
        std::cout << "Translated C++ code:\n" << cppCode << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Translation error: " << e.what() << std::endl;
    }

    return 0;
}
