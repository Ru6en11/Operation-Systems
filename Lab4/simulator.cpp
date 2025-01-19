#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <chrono>

void simulate_temperature_device(const std::string& port) {
    std::ofstream output(port);
    if (!output.is_open()) {
        std::cerr << "Failed to open port: " << port << std::endl;
        return;
    }

    int temperature = 20;
    while (true) {
        temperature += (std::rand() % 8 - 2);

        output << std::to_string(temperature) + "\n";
        output.flush();

        std::cout << "Sended: " << temperature << std::endl;

        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
}

int main(int argc, char* argv[]) {

    if (argc <= 1) {
        std::cout << "The port must be passed" << std::endl;

        return 0;
    }

    const std::string port = argv[1];

    simulate_temperature_device(port);

    return 0;
}

