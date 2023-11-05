# FileManager
Minimalistic C++ header-only library for managing configurations.

# Requirements
- C++20 Or above for `to_string` and `getline`.

# Usage
```cpp
#include "filesystem/filesystem.h"

namespace gameVars {
	float healthPoints = 100;
	int armorPoints = 50;
	bool isMoving = true;
}

void handleFirstFile() {
	FileManager* fm = new FileManager("D:\\game.ini");

	fm->createFile();
	fm->saveVariable("health_value", gameVars::healthPoints);
	fm->saveVariable("armor_points", gameVars::armorPoints);
	fm->saveVariable("is_moving", gameVars::isMoving);
	fm->saveDataToFile();

	gameVars::healthPoints = NULL;
	gameVars::armorPoints = NULL;
	gameVars::isMoving = false;

	fm->loadVariable("health_value", gameVars::healthPoints);
	fm->loadVariable("armor_points", gameVars::armorPoints);
	fm->loadVariable("is_moving", gameVars::isMoving);

	std::cout << gameVars::healthPoints << std::endl;
	std::cout << gameVars::armorPoints << std::endl;
	std::cout << gameVars::isMoving << std::endl;

	delete fm;
}

int main() {
	handleFirstFile();

	std::cin.get();
	return 0;
}
```

# Example File
```
health_value: 100.000000
armor_points: 50
is_moving: 1
```
