#include <iostream>
#include <string>
#include <vector>
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

    // Rule: Replace 'length' property for strings with 'size()' function
    pos = cppCode.find(".length");
    while (pos != std::string::npos) {
        cppCode.replace(pos, 7, ".size()");
        pos = cppCode.find(".length", pos + 7);
    }

    // Rule: Replace 'length' property for arrays with 'size()' function
    pos = cppCode.find(".length");
    while (pos != std::string::npos) {
        cppCode.replace(pos, 7, ".size()");
        pos = cppCode.find(".length", pos + 7);
    }

    // Rule: Replace array access using square brackets with 'at()' function
    pos = cppCode.find("[");
    while (pos != std::string::npos) {
        size_t closeBracketPos = cppCode.find("]", pos);
        if (closeBracketPos != std::string::npos) {
            std::string index = cppCode.substr(pos + 1, closeBracketPos - pos - 1);
            cppCode.replace(pos, closeBracketPos - pos + 1, ".at(" + index + ")");
        }
        pos = cppCode.find("[", pos + 1);
    }

    // Rule: Replace string concatenation with '+'
    pos = cppCode.find("+");
    while (pos != std::string::npos) {
        size_t nextPos = cppCode.find("+", pos + 1);
        if (nextPos != std::string::npos) {
            cppCode.replace(pos, nextPos - pos + 1, "+");
            pos = cppCode.find("+", pos + 1);
        } else {
            break;
        }
    }
        // Rule: Replace 'str.toUpperCase()' with 'std::toupper(str)'
    pos = cppCode.find(".toUpperCase()");
    while (pos != std::string::npos) {
        size_t dotPos = cppCode.rfind(".", pos);
        if (dotPos == std::string::npos)
            throw std::runtime_error("Invalid toUpperCase() syntax");

        cppCode.replace(dotPos, pos - dotPos + 14, "std::toupper(");
        cppCode.insert(pos + 14, ")");
        pos = cppCode.find(".toUpperCase()", pos + 14);
    }

    // Rule: Replace 'str.toLowerCase()' with 'std::tolower(str)'
    pos = cppCode.find(".toLowerCase()");
    while (pos != std::string::npos) {
        size_t dotPos = cppCode.rfind(".", pos);
        if (dotPos == std::string::npos)
            throw std::runtime_error("Invalid toLowerCase() syntax");

        cppCode.replace(dotPos, pos - dotPos + 14, "std::tolower(");
        cppCode.insert(pos + 14, ")");
        pos = cppCode.find(".toLowerCase()", pos + 14);
    }

    // Rule: Replace string comparison with '=='
    pos = cppCode.find("===");
    while (pos != std::string::npos) {
        cppCode.replace(pos, 3, "==");
        pos = cppCode.find("===", pos + 2);
    }

    // Rule: Replace 'for' loop with 'for' loop syntax
    pos = cppCode.find("for");
    while (pos != std::string::npos) {
        size_t openParenPos = cppCode.find("(", pos);
        size_t closeParenPos = cppCode.find(")", openParenPos);
        if (openParenPos != std::string::npos && closeParenPos != std::string::npos) {
            cppCode.replace(openParenPos, closeParenPos - openParenPos + 1, "");
        }
        pos = cppCode.find("for", pos + 1);
    }

    // Rule: Replace 'while' loop with 'while' loop syntax
    pos = cppCode.find("while");
    while (pos != std::string::npos) {
        size_t openParenPos = cppCode.find("(", pos);
        size_t closeParenPos = cppCode.find(")", openParenPos);
        if (openParenPos != std::string::npos && closeParenPos != std::string::npos) {
            cppCode.replace(openParenPos, closeParenPos - openParenPos + 1, "");
        }
        pos = cppCode.find("while", pos + 1);
    }

    // Rule: Replace arithmetic operators with equivalent C++ operators
    pos = cppCode.find("++");
    while (pos != std::string::npos) {
        cppCode.replace(pos, 2, "+ 1");
        pos = cppCode.find("++", pos + 3);
    }

    pos = cppCode.find("--");
    while (pos != std::string::npos) {
        cppCode.replace(pos, 2, "- 1");
        pos = cppCode.find("--", pos + 3);
    }

    // Rule: Replace object creation with variable declaration
    pos = cppCode.find("new");
    while (pos != std::string::npos) {
        size_t openParenPos = cppCode.find("(", pos);
        size_t closeParenPos = cppCode.find(")", openParenPos);
        if (openParenPos != std::string::npos && closeParenPos != std::string::npos) {
            cppCode.replace(pos, closeParenPos - pos + 1, "");
        }
        pos = cppCode.find("new", pos + 1);
    }

    if (cppCode == jsCode) {
        throw std::runtime_error("Undefined translation rule");
    }

    return cppCode;
}

int main() {
    std::string jsCode = R"(
        let str = "Hello";
        const arr = [1, 2, 3];
        console.log(str.length);
        console.log(arr.length);
        console.log(arr[0]);
        console.log("Uppercase:", str.toUpperCase());
        console.log("Lowercase:", str.toLowerCase());
        let concatStr = "Hello" + "World";
        console.log(concatStr);
        let isSame = "Hello" === "World";
        let i = 0;
        while (i < 10) {
            console.log("Index:", i);
            i++;
        }
        for (let i = 0; i < 10; i++) {
            console.log("Index:", i);
        }
        let num = 5 + 3;
        let obj = new Object();
    )";

    try {
        std::string cppCode = translateJavaScriptToCpp(jsCode);
        std::cout << "Translated C++ code:\n" << cppCode << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Translation error: " << e.what() << std::endl;
    }

    return 0;
}
