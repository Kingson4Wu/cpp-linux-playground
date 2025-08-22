#ifndef CHAT_CLIENT_H
#define CHAT_CLIENT_H

#include <string>
#include <thread>
#include <atomic>

namespace chat_room {

/**
 * @brief A TCP chat client.
 * 
 * This client connects to a chat server and allows users to send
 * and receive messages. It runs two threads: one for receiving
 * messages from the server and one for reading user input.
 */
class ChatClient {
public:
    /**
     * @brief Constructs a ChatClient.
     * 
     * @param server_ip The IP address of the server to connect to.
     * @param server_port The port number of the server to connect to.
     */
    ChatClient(const std::string& server_ip, int server_port);

    /**
     * @brief Destructor. Disconnects from the server.
     */
    ~ChatClient();

    /**
     * @brief Connects to the chat server.
     * 
     * @return True if the connection was successful, false otherwise.
     */
    bool Connect();

    /**
     * @brief Disconnects from the chat server.
     */
    void Disconnect();

    /**
     * @brief Starts the client and begins sending/receiving messages.
     * 
     * This method will block until the client is disconnected.
     */
    void Run();

private:
    std::string server_ip_;     ///< IP address of the server
    int server_port_;            ///< Port number of the server
    int client_socket_;         ///< Client socket file descriptor
    bool connected_;             ///< Flag to indicate if the client is connected
    std::atomic<bool> running_; ///< Flag to indicate if the client is running

    std::thread receive_thread_; ///< Thread for receiving messages from the server
    std::thread input_thread_;   ///< Thread for reading user input

    std::string nickname_;       ///< Nickname of the client

    /**
     * @brief Receives messages from the server.
     * 
     * This method runs in a separate thread and continuously
     * reads messages from the server socket, displaying them
     * to the user.
     */
    void ReceiveMessages();

    /**
     * @brief Reads user input and sends messages to the server.
     * 
     * This method runs in a separate thread and continuously
     * reads user input from stdin, sending it to the server.
     */
    void ReadInput();

    /**
     * @brief Sends a message to the server.
     * 
     * @param message The message to send.
     */
    void SendMessage(const std::string& message);
};

} // namespace chat_room

#endif // CHAT_CLIENT_H
