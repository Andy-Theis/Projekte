#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <filesystem>
#include <openssl/sha.h>

std::string hash_file(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file) return "";

    SHA256_CTX ctx;
    SHA256_Init(&ctx);

    char buffer[4096];
    while (file.read(buffer, sizeof(buffer)))
        SHA256_Update(&ctx, buffer, file.gcount());
    if (file.gcount())
        SHA256_Update(&ctx, buffer, file.gcount());

    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_Final(hash, &ctx);

    std::ostringstream result;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i)
        result << std::hex << (int)hash[i];
    return result.str();
}

bool is_malicious(const std::string& hash, const std::vector<std::string>& db) {
    for (const auto& h : db)
        if (h == hash) return true;
    return false;
}

std::vector<std::string> load_hashes(const std::string& db_path) {
    std::vector<std::string> hashes;
    std::ifstream f(db_path);
    std::string line;
    while (std::getline(f, line)) hashes.push_back(line);
    return hashes;
}

int main() {
    auto db = load_hashes("data/malware_hashes.txt");
    std::string folder = "data/testfiles/";

    for (const auto& entry : std::filesystem::directory_iterator(folder)) {
        std::string path = entry.path().string();
        std::string file_hash = hash_file(path);
        if (is_malicious(file_hash, db))
            std::cout << "[!] MALWARE FOUND: " << path << "\n";
        else
            std::cout << "[OK] " << path << "\n";
    }
    return 0;
}
// C++20
// This code uses C++20 features such as std::filesystem for directory iteration.
// Ensure you have OpenSSL installed and linked properly to compile this code.
// Compile with: g++ main.cpp -o minivirenscanner -lssl -lcrypto -std=c++20
// Ensure you have OpenSSL installed and linked properly to compile this code.
// Compile with: g++ main.cpp -o minivirenscanner -lssl -lcrypto -std=c++20
// Ensure you have OpenSSL installed and linked properly to compile this code.
// Compile with: g++ main.cpp -o minivirenscanner -lssl -lcrypto -std=c++20
// Ensure you have OpenSSL installed and linked properly to compile this code.

// Compile with: g++ main.cpp -o minivirenscanner -lssl -lcrypto -std=c++20
// Ensure you have OpenSSL installed and linked properly to compile this code.
// Compile with: g++ main.cpp -o minivirenscanner -lssl -l
// crypto -std=c++20
// Ensure you have OpenSSL installed and linked properly to compile this code.
