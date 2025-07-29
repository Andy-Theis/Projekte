#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>

std::vector<double> read_column(const std::string& filename, int column) {
    std::vector<double> values;
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string item;
        int i = 0;
        while (std::getline(ss, item, ',')) {
            if (i == column) {
                values.push_back(std::stod(item));
                break;
            }
            ++i;
        }
    }
    return values;
}

double mean(const std::vector<double>& data) {
    double sum = 0;
    for (double val : data) sum += val;
    return sum / data.size();
}

double variance(const std::vector<double>& data) {
    double m = mean(data);
    double sum = 0;
    for (double val : data) sum += (val - m) * (val - m);
    return sum / data.size();
}

int main() {
    std::string filename = "data/sample.csv";
    int col = 0;

    std::vector<double> data = read_column(filename, col);

    std::cout << "Anzahl Werte: " << data.size() << "\n";
    std::cout << "Mittelwert: " << mean(data) << "\n";
    std::cout << "Varianz: " << variance(data) << "\n";

    return 0;
}
