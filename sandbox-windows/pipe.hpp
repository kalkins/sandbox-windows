#pragma once

#ifndef PIPE_HELPER_H
#define PIPE_HELPER_H

#include <string>
#include <vector>

#include <windows.h>
#include <tchar.h>
#include <strsafe.h>

namespace sandbox {
	class Pipe {
	public:
		const int BUFSIZE = 1;

		Pipe(std::string name);
		~Pipe();

		template<typename T>
		T read();
		std::string read();
		std::string readLine();

		template<typename T>
		void write(T data) {
			WriteFile(pipe, &data, sizeof(T), &bytesWritten, NULL);
		}

		template<typename T>
		void write(std::vector<T> data) {
			for (T& entry : data) {
			    write<T>(entry);
			}
		}

		template<typename T>
		void write(T* data, size_t length) {
			WriteFile(pipe, data, sizeof(T) * length, &bytesWritten, NULL);
			/*
			for (size_t i = 0; i < length; i++) {
			    write<T>(data[i]);
			}
			*/
		}

		void write(std::string data) {
			for (char c : data) {
			    write(c);
			}
			write('\0');
		}

	private:
		std::string path;
		HANDLE pipe = INVALID_HANDLE_VALUE;
		DWORD bytesRead = 0;
		DWORD bytesWritten = 0;
	};
}

#endif
