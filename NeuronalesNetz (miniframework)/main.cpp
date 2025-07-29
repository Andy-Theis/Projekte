#include <iostream>
#include "neural_net.hpp"

int main() {
    NeuralNet net(2, 4, 1);

    // Trainingsdaten: XOR
    std::vector<std::vector<double>> inputs = {
        {0, 0}, {0, 1}, {1, 0}, {1, 1}
    };
    std::vector<std::vector<double>> targets = {
        {0}, {1}, {1}, {0}
    };

    net.train(inputs, targets, 5000, 0.1);

    std::cout << "Ausgabe nach Training:\n";
    for (const auto& input : inputs) {
        auto output = net.feedforward(input);
        std::cout << input[0] << " XOR " << input[1]
                  << " = " << output[0] << "\n";
    }

    return 0;
}
// Include the necessary directories for MinGW in your VS Code settings
// Open the settings.json file in VS Code and add the following paths:  
// "C_Cpp.default.includePath": [
//     "C:/mingw64/include",
//     "C:/mingw64/x86_64-w64-mingw32/include",
//     "C:/mingw64/x86_64-w64-mingw32/include/c++/<version>",
//     "C:/mingw64/x86_64-w64-mingw32/include

//     "C:/mingw64/x86_64-w64-mingw32/include/c++/<version>/backward"
// ]
