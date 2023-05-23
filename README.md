# Code Translation System
This project requires advanced knowledge in multiple programming languages, a deep understanding of the specific syntax, semantics, and underlying concepts of each language (or the languages to be added), and familiarity with the rules and logic unique to each language. It also requires a thorough understanding of the syntax and semantic rules for the translation rules, and a comprehensive set of tests to ensure the translation engine works efficiently and accurately (with a robust error-handling mechanism).


Due to the time constraints and the complexity and intricacy of the project, I currently do not have sufficient time to dedicate to it. Therefore, I will temporarily set this project as a "ToDo" item. However, I have provided an example C++ code snippet above that demonstrates the conversion of Python & JavaScript code to C++. This can serve as a starting point and provide some insights into the logic for those who are interested in pursuing such a project.

@Update:

- Added a code sample that allows you to convert the Javascript code.
- I've added as many comments as I can for clarity.
- I updated the translation of "print" for the py translator and got the bug fix. It can now do a translation that supports comma-separated expressions to output multiple values.
- It was using parentheses when translating "console.log" for the js translator. I prevented it from using parentheses after the std::cout statement.
- Added len, range, upper, lower, append, pop, format, join, split, random functions to py file.
- Added String Length, Array Length, Array Access, String Concatenation, String Comparison, For Loop, While Loop, Arithmetic Operators, Object Creation to js file.

Ps:
If your terminal/console kills itself while trying the js conversion, delete a few examples from "int main".

# Created By Silent Death_53
Keep in mind that this is a simple infrastructure, it needs more progress to be efficient and smooth. Free to ask & use.
