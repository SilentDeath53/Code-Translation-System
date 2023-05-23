#include <iostream>
#include <string>
#include <stdexcept>

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

    // Add more translation rules here

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
        invalidStatement; // This is an undefined statement
    )";

    try {
        std::string cppCode = translateJavaScriptToCpp(jsCode);
        std::cout << "Translated C++ code:\n" << cppCode << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Translation error: " << e.what() << std::endl;
    }

    return 0;
}
