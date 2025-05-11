#pragma once

// für sowohl C als auch C++

// Abschnitt C:

#include <iostream>
#include <regex>
#include <string>

// nocom = no comment = removing all comments
std::string nocom_c(std::string file_content)
{
    using std::string;

    string verwirrrt = "";
    int length = file_content.length();
    bool was_slash = false;
    bool was_asterisk = false;
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
                verwirrrt += '\n';         // add a newline so stuff doesnt get too wild
                continue;                   // prevents the trailing '/' from entering the result
            } else if (was_slash) 
            {
                verwirrrt.pop_back();
                comment = true;
            } else {
                was_slash = true;
            }
            
            was_asterisk = false;
        } else if (current_char == '*' && !double_quote && !single_quote)
        {
            if (was_slash)
            {
                multiline_comment = true;
                verwirrrt.pop_back();      // removes the beginning '/' of the comment
            } else {
                was_asterisk = true;
            }
        
            was_slash = false;
        } else if (current_char == '\n' && comment) 
        {
            comment = false;
        } else if (current_char == '\'') 
        {
            single_quote = !single_quote;
        } else if (current_char == '"') {
            double_quote = !double_quote;
        } else {
            was_slash = false;
            was_asterisk = false;
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
    std::string verwirrt = "";
    verwirrt = nocom_c(file_content);
    // do more stuff

    return verwirrt;
}

// Abschnitt C++:

// literally the same lol
#define nocom_cpp nocom_c

std::string verwirrrer_komplett_cpp(std::string file_content)
{
    std::string verwirrt = "";
    verwirrt = nocom_cpp(file_content);
    // do more stuff
    
    return verwirrt;
}
