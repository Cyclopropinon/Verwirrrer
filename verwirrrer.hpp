#pragma once

#include "verwirrrer-cpp.hpp"

// language codes
#define lang_inval -1
#define lang_c      0
#define lang_cpp    1
//#define lang_bash   2 //not yet implemented

int Verwirrrer(int lang, std::string& file_content)
{
    switch (lang)
    {
        case 0:
            file_content = verwirrrer_komplett_c(file_content);
            //file_content = kompilierungstest_c(file_content);
            if (file_content == "")
            {
                return 1;
            }
            break;
        case 1:
            file_content = verwirrrer_komplett_cpp(file_content);
            //file_content = kompilierungstest_cpp(file_content);
            if (file_content == "")
            {
                 return 1;
            }
            break;
        /*case 2: //bash noch nicht implementiert
            file_content = nocom_sh(file_content);
            break;*/
        default:
            std::cerr << "Error: language does not exist!" << std::endl;
            return 1;
    }

    return 0;
}

int Verwirrrer_debug(int lang, std::string& file_content0, std::string& file_content1, std::string& file_content2, std::string& file_content3)
{
    switch (lang)
    {
        case 0:
            file_content1 = ppc_c(file_content0, "", true);
            file_content2 = nocom_c(file_content1);
            file_content3 = nocom_c(file_content2);
            if (file_content3 == "")
            {
                return 1;
            }
            break;
        case 1:
            file_content1 = ppc_cpp(file_content0, "", true);
            file_content2 = nocom_cpp(file_content1);
            file_content3 = nocom_cpp(file_content2);
            if (file_content3 == "")
            {
                 return 1;
            }
            break;
        default:
            std::cerr << "Error: language does not exist!" << std::endl;
            return 1;
    }

    return 0;
}
