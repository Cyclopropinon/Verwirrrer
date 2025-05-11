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
            file_content = nocom_c(file_content);
            //file_content = kompilierungstest_c(file_content);
            if (file_content == "")
            {
                return 1;
            }
            break;
        case 1:
            file_content = nocom_cpp(file_content);
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
            cout << "Error: language does not exist!" << endl;
            return 1;
    }

    return 0;
}
