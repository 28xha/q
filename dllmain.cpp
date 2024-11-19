// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;

struct vec3 {

    double pt1;
    double pt2;
    double pt3;


};

enum retcodes : int {

    status_working,
    status_error,
    status_success,
    status_bad_load,
    status_kicked,
    status_badinternet,
    status_successfulinit,
    status_badinit,
    status_couldnotloadgame
    





};

int CurrentRetCode;


extern "C" __declspec(dllexport) int GetLastCode() {
    return CurrentRetCode;
}

extern "C" __declspec(dllexport) int LoadGame(const std::string& filename) {
    fstream file(filename, std::ios::in);

    if (!file.is_open()) {
        return status_couldnotloadgame;
        HANDLE hProcess = GetCurrentProcess();
        TerminateProcess(hProcess, 0);
    }

    std::string line;

    while (getline(file, line)) {
        if (line.empty() || line.length() == 0) {
            continue;
        }

        if (line.find('\xBF') != std::string::npos) {  // '¿' -> '\xBF'
            size_t pos;
            string name;
            string alpha;
            string beta;
            string gamma;

            for (auto i = 0; i < line.length(); i++) {
                if (line[i] == '\x39\x34') {  // 'Δ' -> '\x39\x34'
                    name = line.substr(1, i);
                    continue;
                }

                if (line[i] == '\x03\xB1') {  // 'α' -> '\x03\xB1'
                    size_t betaPos = line.find('\x03\xB2', i);  // 'β' -> '\x03\xB2'
                    alpha = line.substr(i + 1, betaPos - (i + 1));
                    continue;
                }

                if (line[i] == '\x03\xB2') {  // 'β' -> '\x03\xB2'
                    size_t gammaPos = line.find('\x03\xB3', i);  // 'γ' -> '\x03\xB3'
                    alpha = line.substr(i + 1, gammaPos - (i + 1));
                    continue;
                }

                if (line[i] == '?') {
                    continue;
                }
            }

            string EndingString = "id: " + name + '\n' + "1st point: " + alpha + '\n' + "2nd point: " + beta + '\n' + "3rd point: " + gamma;

            std::cout << EndingString;
        }

        if (line.length() == 1 || line == "\x00DF") {  // '\x00DF' is the hex for 'þ'
            break;
        }
    }

    

    file.close();

    return status_success;
}

























BOOL APIENTRY DllMain( HMODULE hModule,DWORD  ul_reason_for_call,LPVOID lpReserved)              
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CurrentRetCode = status_successfulinit;
        Sleep(3000);
        CurrentRetCode = NULL;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
