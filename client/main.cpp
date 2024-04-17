#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <thread>
#include <chrono>

void log(const std::string& message) {
    std::ofstream logfile("client.log", std::ios_base::app);
    if (logfile.is_open()) {
        time_t now = time(0);
        char* dt = ctime(&now);
        logfile << dt << "\t" << message << std::endl;
        logfile.close();
    } else {
        std::cerr << "Error opening log file" << std::endl;
    }
}

int main() {
    int client_socket;
    struct sockaddr_in server_address;
    char buffer[1024] = {0};
    
    std::ifstream file("config.txt");
    if (!file.is_open()) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    
    std::string server_ip;
    if (!std::getline(file, server_ip)) {
        perror("Error reading server address");
        exit(EXIT_FAILURE);
    }

    int server_port;
    std::string server_port_string;
    if (std::getline(file, server_port_string)) {
      try {
        server_port = std::stoi(server_port_string);
      } catch (const std::invalid_argument& e) {
          std::cerr << "Invalid argument: " << e.what() << std::endl;
      } 
    } else {
        perror("Error reading server port");
        exit(EXIT_FAILURE);
    }
    
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }
    
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(server_port);
    
    if (inet_pton(AF_INET, server_ip.c_str(), &server_address.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        exit(EXIT_FAILURE);
    }
    
    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Connection Failed");
        exit(EXIT_FAILURE);
    }

    log("Connected to " + server_ip + ":" + std::to_string(server_port));

    std::this_thread::sleep_for(std::chrono::seconds(5));
    
    const char *message = "Polienko Veniamin Sergevich M3O-310-21";
    send(client_socket, message, strlen(message), 0);
    printf("Message sent to server\n");
    log("Message '" + std::string(message) + "' sent to server");

    
    read(client_socket, buffer, 1024);
    printf("Server response: %s\n", buffer);
    log("Server said: '" + std::string(buffer) + "'");
    
    close(client_socket);
    log("Disconnected from the server");
    return 0;
}