#include <iostream>
#include <numeric>
//#include "SFML Game engine/Utilities/Font string names.h"
#include "SFML Game engine/States/StateManager.h"
#include "SFML Game engine/Asset Manager/AssetManager.h"
#include "SFML Game engine/Utilities/Timer.h"

const std::pair<int, int> &getNumValleys(const std::string &s) {
	int numValleys = 0, numMountains = 0, seaLevel = 0;

	for (char ch : s) {
		if (ch == 'U') {
			seaLevel++;
			if (seaLevel == 0)
				numValleys++;
		}
		else {
			seaLevel--;
			if (seaLevel == 0)
				numMountains++;
			
		}
	}

	return std::make_pair(numValleys, numMountains);
}

int main() {
	using namespace std;
	using namespace sf;

	StateManager m(800, 900, "Frogger");	

	[]() {cout << "hi\n"; };
	m.runGame();
}