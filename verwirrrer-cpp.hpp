#pragma once

// für sowohl C als auch C++

// Abschnitt C:

#include <iostream>
#include <regex>
#include <string>

#include "c-include.hpp"

// nocom = no comment = removing all comments
std::string nocom_c(std::string file_content)
{
    using std::string;

    string verwirrrt = "";
    int length = file_content.length();
    bool was_digit = false;
    bool was_slash = false;
    bool was_asterisk = false;
    bool was_backslash = false;
    bool multiline_comment = false;
    bool comment = false;
    bool single_quote = false;
    bool double_quote = false;

    for (int i = 0; i < length; i++)
    {
        char current_char = file_content[i];
        if (current_char == '/' && !double_quote && !single_quote)
        {
            if (was_asterisk)
            {
                multiline_comment = false;
                verwirrrt += '\n';          // add a newline so stuff doesnt get too wild; needs to be a ' ' for multiline comments only spanning a single line, in case it is part of a makro

                was_digit = false;
                was_slash = false;
                was_asterisk = false;
                was_backslash = false;
                continue;                   // prevents the trailing '/' from entering the result
            } else if (was_slash) 
            {
                verwirrrt.pop_back();
                comment = true;
            } else {
                was_slash = true;
            }
            
            was_digit = false;
            was_asterisk = false;
            was_backslash = false;
        } else if (current_char == '*' && !double_quote && !single_quote)
        {
            if (was_slash)
            {
                multiline_comment = true;
                verwirrrt.pop_back();       // removes the beginning '/' of the comment
            } else {
                was_asterisk = true;
            }
        
            was_digit = false;
            was_slash = false;
            was_backslash = false;
        } else if (current_char == '\n' && comment) 
        {
            comment = false;
            was_digit = false;
            was_slash = false;
            was_asterisk = false;
            was_backslash = false;
        } else if (current_char == '\'' && !comment && !multiline_comment) {
            if(was_digit && !single_quote) continue;        // removes the single quote seperators from integer literals
            if(!was_backslash) single_quote = !single_quote;
            was_digit = false;
            was_slash = false;
            was_asterisk = false;
            was_backslash = false;
        } else if (current_char == '\"' && !comment && !multiline_comment) {
            if(!was_backslash) double_quote = !double_quote;
            was_digit = false;
            was_slash = false;
            was_asterisk = false;
            was_backslash = false;
        } else if (current_char == '\\') {
            was_digit = false;
            was_slash = false;
            was_asterisk = false;
            was_backslash = !was_backslash;
        } else if ('0' <= current_char && current_char <= '9') {
            was_digit = true;
            was_slash = false;
            was_asterisk = false;
            was_backslash = false;
        } else {
            was_digit = false;
            was_slash = false;
            was_asterisk = false;
            was_backslash = false;
        }
        
        if (!comment && !multiline_comment)
        {
            verwirrrt += current_char;
        }
    }

    return verwirrrt;
}

std::string verwirrrer_komplett_c(std::string file_content)
{
    std::string verwirrt1 = ppc_c(file_content, "", true);
    std::string verwirrt2 = nocom_c(verwirrt1);
    // do more stuff

    return verwirrt2;
}

// Abschnitt C++:

// literally the same lol
#define ppc_cpp     ppc_c
#define nocom_cpp   nocom_c

std::string verwirrrer_komplett_cpp(std::string file_content)
{
    std::string verwirrt1 = ppc_cpp(file_content, "", true);
    std::string verwirrt2 = nocom_cpp(verwirrt1);
    // do more stuff

    return verwirrt2;
}
