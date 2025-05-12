#include <iostream>
#include <fstream>
#include <filesystem>
#include <map>
#include <string>
#include "verwirrrer.hpp"


// -1 = Invalid
//  0 = C
//  1 = C++
//  2 = Bash
int global_lang = lang_inval;
int global_debug = false;
std::string global_out_file;
std::string global_file_name;
std::string global_file_content;

int parse_params(int argc, char* argv[])
{
    using std::string, std::cin, std::cout, std::cerr, std::endl;

    if (argc < 2)
    {
        cout << "Error: Expected at least 1 parameter!\n"
                "Use --help or -h to get some help." << endl;
        return 1;
    }

    int i = 1;
    bool help = false;
    bool was_flag = false;
    string file_string;
    namespace fs = std::filesystem;

    while (i < argc)
    {
        string arg = argv[i];

        if (arg == "-h" || arg == "--help")
        {
            help = true;
            was_flag = true;
        } else if (arg == "-d" || arg == "--debug")
        {
            global_debug = true;
            was_flag = true;
        } else if (arg == "-l" || arg == "--language")
        {
            was_flag = true;

            if (argc > i + 1)
            {
                string lang_string = argv[i + 1];

                std::map<string, int> lang_map;
                lang_map["c"] = lang_c;
                lang_map["cpp"] = lang_c;
                lang_map["sh"] = lang_c;

                std::map<string, int>::iterator i = lang_map.begin();

                while (i != lang_map.end())
                {
                    if (i->first == lang_string)
                    {
                        global_lang = i->second;
                        break;
                    }
                    i++;
                }

                if (global_lang == lang_inval)
                {
                    cout << "Error: filetype does not exist!" << endl;
                    return 1;
                }
            } else {
                cout << "Error: parameter expected for the type." << endl;
                return 1;
            }
        } else if (arg == "-o" || arg == "--out") 
        {
            was_flag = true;

            if (argc > i + 1)
            {
                global_out_file = argv[i + 1];
            } else {

                cout << "Error: parameter expected for the output file." << endl;
                return 1;
            }
        } else {
            if (!was_flag)
            {
                global_file_name = argv[i];

                fs::path file{ global_file_name };

                if (!fs::exists(file))
                {
                    cout << "Error: file not found!" << endl;
                    return 1;
                }
            }

            was_flag = false;
        }

        i++;
    }

    if (help == true)
    {
        cout <<  "Usage: ./Verwirrrer [options] <file>"
            "\n" "Options:"
            "\n" "  -h, --help               Show this help message and exit"
            "\n" "  -l, --language <type>    Specify the language (optional)"
            "\n" "  -o, --out      <file>    Specify the output file (optional)"         
            "\n"
            "\n" "Examples:"
            "\n" "  ./Verwirrrer -f main.cpp"
            "\n" "  ./Verwirrrer --file main.sh --type sh"
            "\n"
            "\n" "Supported file types:"
            "\n" "  C/C++ (c, cpp)"
            //"\n" "  Bash/Shell (sh)"
            "\n";
        return 2;
    }
    
    if (global_file_name.empty())
    {
        cout << "Error: No input file specified.\n"
                "Use the -f or --file flag to specify an input file." << endl;
        return 1;
    }

    string file_extension = global_file_name.substr(global_file_name.rfind(".") + 1);

    if (global_lang == lang_inval)
    {
        cout << "No type has been specified, attempting to auto-detect the type of \"" << global_file_name << "\"...\nExtension: \"" << file_extension << "\"\n";

        std::map<string, int> lang_map;
        lang_map["c"]   = lang_c;
        lang_map["h"]   = lang_c;
        lang_map["cpp"] = lang_cpp;
        lang_map["hpp"] = lang_cpp;
        lang_map["hh"]  = lang_cpp;
        //lang_map["sh"]  = lang_bash;

        std::map<string, int>::iterator i = lang_map.begin();

        while (i != lang_map.end())
        {
            if (i->first == file_extension)
            {
                global_lang = i->second;
                cout << "Detected language: " << file_extension << "." << endl;
                break;
            }
            i++;
        }
    }

    if (global_out_file == "") {
        global_out_file = "verwirrrt." + file_extension;
    }

    return 0;
}

int main(int argc, char* argv[])
{
    using std::string, std::cin, std::cout, std::cerr, std::endl;

    int parse_params_return = parse_params(argc, argv);

    if (parse_params_return == 1)
    {
        return 1;
    } else if (parse_params_return == 2) 
    {
        return 0;
    }

    string line;
    std::ifstream file_stream(global_file_name);

    while (getline (file_stream, line))
    {
        global_file_content.append(line + '\n');
    }
    file_stream.close();

    if (global_debug)
    {
        string o1, o2, o3;
        Verwirrrer_debug(global_lang, global_file_content, o1, o2, o3);

        std::ofstream of1("vd_1");
        of1 << o1;
        of1.close();
        cout << "Successfully wrote to \"vd_1\"!\n";    

        std::ofstream of2("vd_2");
        of2 << o2;
        of2.close();
        cout << "Successfully wrote to \"vd_2\"!\n";    

        std::ofstream of3("vd_3");
        of3 << o2;
        of3.close();
        cout << "Successfully wrote to \"vd_3\"!\n";    
    } else if (Verwirrrer(global_lang, global_file_content) == 1) {
        return 1;
    }

    std::ofstream OutFile(global_out_file);
    OutFile << global_file_content;
    OutFile.close();
    cout << "Successfully wrote to \"" << global_out_file << "\"!" << endl;


    return 0;
}
