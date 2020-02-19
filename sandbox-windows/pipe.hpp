#pragma once

#ifndef PIPE_HELPER_H
#define PIPE_HELPER_H

#include <string>

#include <windows.h>
#include <tchar.h>
#include <strsafe.h>

namespace sandbox {
	class Pipe {
	public:
		const int BUFSIZE = 1;

		Pipe(std::string name);
		~Pipe();

		char read();
		void write(char data);
		void write(std::string data);

	private:
		std::string path;
		HANDLE pipe = INVALID_HANDLE_VALUE;
		DWORD bytesRead = 0;
		DWORD bytesWritten = 0;
	};
}

#endif
