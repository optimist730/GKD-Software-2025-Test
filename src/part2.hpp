#include <fstream>
#include "json.hpp"
#include "part1.hpp"

using namespace std;

model loadmodel();

void loadmatrix(matrix& a, const string& filename);