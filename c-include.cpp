#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

std::unordered_set<std::string> pragma_once_dateien;

std::string lade_datei(const std::string& dateiname, bool rekursiv_suchen = false)
{
    namespace fs = std::filesystem;

    std::ifstream file(dateiname);
    if (file)
    {
        std::cout << "Datei inkludiert: \"" << dateiname << "\"\n";
        std::ostringstream ss;
        ss << file.rdbuf();
        return ss.str();
    }

    if (!rekursiv_suchen) return "";

    // Rekursive Suche in Unterordnern
    for (const auto& eintrag : fs::recursive_directory_iterator("."))
    {
        if (!eintrag.is_regular_file()) continue;
        if (eintrag.path().filename() == dateiname)
        {
            std::ifstream f(eintrag.path());
            if (f)
            {
                std::cout << "Datei inkludiert: " << eintrag.path() << '\n';
                std::ostringstream ss;
                ss << f.rdbuf();
                return ss.str();
            }
        }
    }

    return "";
}

bool ist_gueltiges_include(const std::string& zeile, std::string& dateiname_out, bool& im_block_comment)
{
    enum Zustand { NORMAL, STRING, CHAR, LINE_COMMENT };
    Zustand zustand = NORMAL;

    for (size_t i = 0; i < zeile.length(); ++i)
    {
        char c = zeile[i];
        char next = (i + 1 < zeile.length()) ? zeile[i + 1] : '\0';

        if (im_block_comment)
        {
            if (c == '*' && next == '/')
            {
                im_block_comment = false;
                ++i;
            }
            continue;
        }

        switch (zustand)
        {
            case NORMAL:
                if (c == '/' && next == '/')
                {
                    return false;
                } else if (c == '/' && next == '*') {
                    im_block_comment = true;
                    ++i;
                } else if (c == '"') {
                    zustand = STRING;
                } else if (c == '\'') {
                    zustand = CHAR;
                } else if (c == '#' && zeile.substr(i, 8) == "#include") {
                    std::smatch match;
                    std::string rest = zeile.substr(i);
                    std::regex include_regex("^#\\s*include\\s*\"([^\"]+)\"");
                    if (std::regex_search(rest, match, include_regex)) {
                        dateiname_out = match[1];
                        return true;
                    }
                }
                break;

            case STRING:
                if (c == '\\') ++i;
                else if (c == '"') zustand = NORMAL;
                break;

            case CHAR:
                if (c == '\\') ++i;
                else if (c == '\'') zustand = NORMAL;
                break;

            case LINE_COMMENT:
                return false;
        }
    }

    return false;
}

std::string ppc(const std::string& dateiinhalt, const std::string& aktueller_dateiname = "", bool rekursiv_suchen = false)
{
    std::istringstream iss(dateiinhalt);
    std::ostringstream result;
    std::string zeile;

    bool hat_pragma_once = false;
    bool im_block_comment = false;

    std::regex pragma_once_regex("^\\s*#\\s*pragma\\s+once\\s*");
    std::regex system_include_regex("^\\s*#\\s*include\\s*<[^>]+>\\s*");

    while (std::getline(iss, zeile))
    {
        std::string includepfad;

        // Prüfe auf pragma once
        if (std::regex_match(zeile, pragma_once_regex))
        {
            hat_pragma_once = true;
            if (pragma_once_dateien.count(aktueller_dateiname))
            {
                return "";
            }
            pragma_once_dateien.insert(aktueller_dateiname);
            continue;
        }

        // Prüfe auf gültiges #include "..."
        if (ist_gueltiges_include(zeile, includepfad, im_block_comment))
        {
            std::string includetext = lade_datei(includepfad, rekursiv_suchen);
            if (!includetext.empty())
            {
                result << "// Begin Include: " << includepfad << "\n";
                result << ppc(includetext, includepfad, rekursiv_suchen);
                result << "// End Include: " << includepfad << "\n";
                continue;
            } else {
                std::cerr << "Header \"" << includepfad << "\" nicht gefunden.";
            }
        }

        if (std::regex_match(zeile, system_include_regex));

        result << zeile << '\n';
    }

    return result.str();
}

int main()
{
    std::string quell_datei;
    std::cout << "Dateiname eingeben: ";
    std::getline(std::cin, quell_datei);

    bool rekursiv_suchen = true; // oder über Benutzerabfrage setzen

    std::string dateiinhalt = lade_datei(quell_datei, rekursiv_suchen);
    if (dateiinhalt.empty())
    {
        std::cerr << "Datei nicht gefunden.\n";
        return 1;
    }

    std::string ergebnis = ppc(dateiinhalt, quell_datei, rekursiv_suchen);

    std::ofstream ausgabe("ausgabe.cpp");
    if (!ausgabe)
    {
        std::cerr << "Konnte Ausgabedatei nicht schreiben.\n";
        return 1;
    }

    ausgabe << ergebnis;
    std::cout << "Verarbeitung abgeschlossen. Ergebnis in ausgabe.cpp.\n";
    return 0;
}
