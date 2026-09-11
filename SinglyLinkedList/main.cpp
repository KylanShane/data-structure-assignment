#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

int add(int a, int b) {
    return a + b;
}

int main() {
    cout << "=================================\n";
    cout << "   C++ / VS Code Test Program\n";
    cout << "=================================\n\n";

    // 1. Basic output
    cout << "[OK] C++ program is running!\n";

    // 2. Variables
    string name = "VS Code";
    int number = 10;

    cout << "[OK] Variables work: " << name
         << " / " << number << "\n";

    // 3. Function test
    int result = add(5, 7);

    if (result == 12) {
        cout << "[OK] Functions work: 5 + 7 = " << result << "\n";
    } else {
        cout << "[FAIL] Function test failed!\n";
    }

    // 4. Vector / STL test
    vector<int> numbers = {5, 2, 9, 1, 7};
    sort(numbers.begin(), numbers.end());

    cout << "[OK] Standard Library works: ";

    for (int n : numbers) {
        cout << n << " ";
    }

    cout << "\n";

    // 5. Loop test
    cout << "[OK] Loop test: ";

    for (int i = 1; i <= 5; i++) {
        cout << i << " ";
    }

    cout << "\n";

    // 6. C++17 test
    if constexpr (true) {
        cout << "[OK] C++17 features are working!\n";
    }

    // 7. User input test
    string input;

    cout << "\nType something and press Enter: ";
    getline(cin, input);

    cout << "[OK] Input received: " << input << "\n";

    cout << "\n=================================\n";
    cout << "   ALL BASIC TESTS PASSED!\n";
    cout << "=================================\n";

    return 0;
}