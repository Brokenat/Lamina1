//
// Created by meian on 2026/4/4.
//

#pragma once
#include <iostream>
#include <string>
#include <unordered_map>

enum class ErrorType {
    Unknown,
    Parse,
    Analysis,
    Generate,
};
extern bool errd;
inline std::unordered_map<ErrorType, std::string> error_type_map = {
    {ErrorType::Unknown, "Unknown"},
    {ErrorType::Parse, "Parse"},
    {ErrorType::Analysis, "Analysis"},
    {ErrorType::Generate, "Generate"}
};
inline constexpr bool P_DEBUG = true;
#define throw_error(error_type, message, line, col) \
    errd = true;\
if constexpr (P_DEBUG) {\
    std::cerr << "[" << __FILE__ << ":" << __LINE__ << "]";\
}\
    std::cerr << error_type_map[error_type] << "Error at " <<  line << ", " << col << ":\n\t" << message << std::endl;
