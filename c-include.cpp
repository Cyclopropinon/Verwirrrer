#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <regex>

std::unordered_set<std::string> pragma_once_dateien;

std::string lade_datei(const std::string& dateiname)
{
    std::ifstream file(dateiname);
    if (!file) return "";
    std::ostringstream ss;
    ss << file.rdbuf();
    return ss.str();
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

std::string ppc(const std::string& dateiinhalt, const std::string& aktueller_dateiname = "")
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
            std::string includetext = lade_datei(includepfad);
            if (!includetext.empty())
            {
                result << "// Begin Include: " << includepfad << "\n";
                result << ppc(includetext, includepfad);
                result << "// End Include: " << includepfad << "\n";
                continue;
            }
        }

        if (std::regex_match(zeile, system_include_regex));

        result << zeile << '\n';
    }

    return result.str();
}

int main() {
    std::string quell_datei;
    std::cout << "Dateiname eingeben: ";
    std::getline(std::cin, quell_datei);

    std::string dateiinhalt = lade_datei(quell_datei);
    if (dateiinhalt.empty()) return 1;

    std::string ergebnis = ppc(dateiinhalt, quell_datei);

    std::ofstream ausgabe("ausgabe.cpp");
    if (!ausgabe) {
        std::cerr << "Konnte Ausgabedatei nicht schreiben.\n";
        return 1;
    }

    ausgabe << ergebnis;
    std::cout << "Verarbeitung abgeschlossen. Ergebnis in ausgabe.cpp.\n";
    return 0;
}

