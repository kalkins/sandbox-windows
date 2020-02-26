#include <iostream>
#include <Windows.h>
#include <stdio.h>
#include <tchar.h>
#include <strsafe.h>
#include "pipe.hpp"


namespace sandbox {
    Pipe::Pipe(std::string name)
    {
        path = "\\\\.\\pipe\\" + name;

        int len;
        int slength = (int)path.length() + 1;
        len = MultiByteToWideChar(CP_ACP, 0, path.c_str(), slength, 0, 0);
        wchar_t* buf = new wchar_t[len];
        MultiByteToWideChar(CP_ACP, 0, path.c_str(), slength, buf, len);
        std::wstring r(buf);
        delete[] buf;

        LPTSTR pipename = const_cast<LPTSTR>(r.c_str());
        pipe = CreateNamedPipe(
            pipename,                   // pipe name
            PIPE_ACCESS_DUPLEX,         // read/write access
            PIPE_TYPE_MESSAGE |         // message type pipe
            PIPE_READMODE_MESSAGE |     // message-read mode
            PIPE_WAIT,                  // blocking mode
            PIPE_UNLIMITED_INSTANCES,   // max. instances
            BUFSIZE,                    // output buffer size
            BUFSIZE,                    // input buffer size
            0,                          // client time-out
            NULL
        );

        bool connected = ConnectNamedPipe(pipe, NULL) ? TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);

        if (!connected) {
            CloseHandle(pipe);
            std::cout << "Could not open named pipe" << std::endl;
            throw;
        }
    }

    Pipe::~Pipe()
    {
        CloseHandle(pipe);
    }

    template<typename T>
    T Pipe::read()
    {
        bool success = false;
        T data;

        success = ReadFile(pipe, &data, sizeof(T), &bytesRead, NULL);

        return data;
    }

    std::string Pipe::read()
    {
        std::string str;
        char c = read<char>();

        while (c != '\0') {
            str += c;
            c = read<char>();
        }

        return str;
    }

    std::string Pipe::readLine()
    {
        std::string str;
        char c = read<char>();

        while (c != '\0' && c != '\n') {
            str += c;
            c = read<char>();
        }

        return str;
    }
}
