/*MIT License

Copyright (c) 2023 Landon

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma once

#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>

#include <cstring>
#include <type_traits> // For std::is_same

class FileManager {
private:
	const char* _filePath;
	std::vector<std::pair<std::string, std::string>> _data;

public:
	FileManager() = default;
	FileManager(const char* inFilePath) : _filePath{ inFilePath } {}
	FileManager(std::string inFilePath) : _filePath{ inFilePath.c_str() } {}

	const std::string getFile() {
		return std::string(_filePath);
	}

	bool createFile() {
		std::string filePath = getFile();
		if (filePath.empty())
			return false;

		// If the file already exists, don't try to create a new one
		if (std::filesystem::exists(filePath))
			return false;

		// Create
		std::ofstream{ filePath }.close();
		return true;
	}

	template <typename t>
	bool saveVariable(std::string variableName, t originalValue) {
		std::string filePath = getFile();
		if (filePath.empty())
			return false;

		std::string strValue;
		if constexpr (std::is_same_v<t, std::string>) {
			strValue = originalValue; // No need to use std::to_string
		} else {
			strValue = std::to_string(originalValue);
		}

		_data.push_back(std::make_pair(variableName, strValue));
	}

	void saveDataToFile() {
		std::string filePath = getFile();
		if (filePath.empty())
			return;

		std::ofstream fileStream{ filePath, std::ios::out };

		for (const auto& curItem : _data) {
			fileStream << curItem.first << "#" << curItem.second << std::endl;
		}

		fileStream.close();
	}

	std::string findVariableValue(std::string variableName) {
		std::string outValue;

		for (const auto& curItem : _data) {
			std::string curItemName = curItem.first;
			if (curItemName.empty())
				continue;

			if (curItemName == variableName)
				outValue = curItem.second;
		}

		return outValue;
	}

	template <typename t>
	void loadVariable(std::string variableName, t& var) {
		std::string filePath = getFile();
		if (filePath.empty())
			return;

		std::ifstream fileStream{ filePath, std::ios::in };
		std::string line;

		while (std::getline(fileStream, line)) {
			size_t curPos = line.find("#");
			if (curPos != std::string::npos) {
				std::string curName = line.substr(0, curPos);
				if (curName == variableName) {
					std::string curValue = line.substr(curPos + 1);
					if constexpr (std::is_same_v<t, std::string>) {
						var = curValue; // Assign the entire line as a string
					} else {
						std::istringstream stringStream(curValue);
						stringStream >> var;
					}
					break; // No need to continue searching once we've found the value.
				}
			}
		}

		fileStream.close();
	}
};
