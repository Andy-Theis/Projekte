#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

std::mutex mtx;

bool scan_port(const std::string& ip, int port, int timeout_ms = 500) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) return false;

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &addr.sin_addr);

    // Timeout setzen
    timeval timeout;
    timeout.tv_sec = timeout_ms / 1000;
    timeout.tv_usec = (timeout_ms % 1000) * 1000;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));
    setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (const char*)&timeout, sizeof(timeout));

    bool is_open = connect(sock, (sockaddr*)&addr, sizeof(addr)) == 0;
    close(sock);
    return is_open;
}

void scan_range(const std::string& ip, int start, int end) {
    for (int port = start; port <= end; ++port) {
        if (scan_port(ip, port)) {
            std::lock_guard<std::mutex> lock(mtx);
            std::cout << "[OPEN] Port " << port << "\n";
        }
    }
}

int main() {
    std::string target_ip = "127.0.0.1";
    int start_port = 1;
    int end_port = 1024;

    std::cout << "Scanning " << target_ip << " from port " << start_port << " to " << end_port << "\n";

    std::thread t1(scan_range, target_ip, 1, 256);
    std::thread t2(scan_range, target_ip, 257, 512);
    std::thread t3(scan_range, target_ip, 513, 768);
    std::thread t4(scan_range, target_ip, 769, 1024);

    t1.join(); t2.join(); t3.join(); t4.join();

    return 0;
}
