#include "expression.h"


std::string join(const std::vector<std::string> &strings, const std::string& delim)
{
    std::stringstream ss;
    std::copy(strings.begin(), strings.end(),
        std::ostream_iterator<std::string>(ss, delim.c_str()));
    return ss.str();
}

Expression::Expression(const std::string& input)
    : postfix_tokens(std::nullopt)
    , identifiers({})
{
    std::vector<Token> tokens = Lexer(input).get_tokens();
    if (!ExpressionValidator::is_valid(tokens)) {
        throw std::invalid_argument("Expression is not valid");
    }
    this->infix_tokens = tokens;
}


const std::vector<Token>& Expression::get_infix_tokens() const {
    return this->infix_tokens;
}

std::string Expression::get_infix() const {
    std::string out;
    for (const Token& token : this->get_infix_tokens()) {
        out += token.literal;
    }
    return out;
}

const std::vector<Token>& Expression::get_postfix_tokens() {
    if (this->postfix_tokens.has_value()) {
        return this->postfix_tokens.value();
    }
    this->postfix_tokens = std::make_optional<std::vector<Token>>({});
    Stack<Token> stack;
    for (const Token& token : this->infix_tokens) {
        // TODO: consider introducing separate TokenType for left and right paren
        switch (token.type) {
            case TokenType::LeftParen: {
                stack.push(token);
                break;
            }
            case TokenType::RightParen: {
                Token item = stack.pop();
                while (item.literal != "(") {
                    this->postfix_tokens.value().push_back(item);
                    item = stack.pop();
                }
                break;
            }
            case TokenType::Operation: {
                while (!stack.is_empty()) {
                    Token item = stack.pop();
                    if (token.priority <= item.priority) {
                        this->postfix_tokens.value().push_back(item);
                    } else {
                        stack.push(item);
                        break;
                    }
                }
                stack.push(token);
                break;
            }
            case TokenType::Number: {
                this->postfix_tokens.value().push_back(token);
                break;
            }
            case TokenType::Identifier: {
                this->identifiers.insert(token.literal);
                this->postfix_tokens.value().push_back(token);
                break;
            }
        }
    }
    while(!stack.is_empty()) {
        this->postfix_tokens.value().push_back(stack.pop());
    }
    return this->postfix_tokens.value();
}

std::string Expression::get_postfix() {
    std::vector<std::string> out;
    for (const Token& token : this->get_postfix_tokens()) {
        out.push_back(token.literal);
    }
    return join(out, " ");
}

const std::set<std::string>& Expression::get_identifiers() const {
    return this->identifiers;
}

double Expression::calculate(const std::map<std::string, double>& values) {
    for (const std::string& ident : this->identifiers) {
        if (!values.count(ident)) {
            throw std::invalid_argument(
                "You have to provide the value of `" + ident + "` in the argument"
            );
        }
    }
    Stack<double> stack;
    for (const Token& token : this->get_postfix_tokens()) {
        switch (token.type) {
        case TokenType::Operation: {
            if (token.literal == "sin") {
                double operand = stack.pop();
                stack.push(std::sin(operand));
            } else if (token.literal == "cos") {
                double operand = stack.pop();
                stack.push(std::cos(operand));
            } else if (token.literal == "neg") {
                double operand = stack.pop();
                stack.push(-operand);
            } else {
                double right_operand = stack.pop();
                double left_operand = stack.pop();
                if (token.literal == "+") {
                    stack.push(left_operand + right_operand);
                } else if (token.literal == "-") {
                    stack.push(left_operand - right_operand);
                } else if (token.literal == "*") {
                    stack.push(left_operand * right_operand);
                } else if (token.literal == "/") {
                    if (right_operand == 0) {
                        throw std::logic_error("Can't divide by zero");
                    }
                    stack.push(left_operand / right_operand);
                } else if (token.literal == "^") {
                    stack.push(std::pow(left_operand, right_operand));
                }
            }
            break;
        }
        case TokenType::Number: {
            stack.push(std::stod(token.literal));
            break;
        }
        case TokenType::Identifier: {
            stack.push(values.at(token.literal));
            break;
        }
        }
    }
    return stack.pop();
}
