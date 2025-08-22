#ifndef CHAT_SESSION_H
#define CHAT_SESSION_H

#include <string>
#include <memory>
#include <thread> // Add this include

namespace chat_room {

/**
 * @brief Manages a single client connection in the chat server.
 * 
 * This class handles communication with a single client, including
 * receiving messages, sending messages, and managing the client's
 * state (e.g., nickname).
 */
class ChatSession : public std::enable_shared_from_this<ChatSession> {
public:
    /**
     * @brief Constructs a ChatSession.
     * 
     * @param id Unique identifier for this session.
     * @param socket Client socket file descriptor.
     */
    ChatSession(int id, int socket);

    /**
     * @brief Destructor. Closes the client socket.
     */
    ~ChatSession();

    /**
     * @brief Gets the unique identifier for this session.
     * 
     * @return The session ID.
     */
    int GetId() const { return id_; }

    /**
     * @brief Gets the client's socket file descriptor.
     * 
     * @return The socket file descriptor.
     */
    int GetSocket() const { return socket_; }

    /**
     * @brief Gets the client's nickname.
     * 
     * @return The nickname.
     */
    const std::string& GetNickname() const { return nickname_; }

    /**
     * @brief Sets the client's nickname.
     * 
     * @param nickname The new nickname.
     */
    void SetNickname(const std::string& nickname) { nickname_ = nickname; }

    /**
     * @brief Starts the session.
     * 
     * This method starts the threads for receiving and sending messages.
     * It should be called after the object is fully constructed.
     */
    void Start();

    /**
     * @brief Stops the session.
     * 
     * This method signals the session to stop and waits for the
     * receive and send threads to finish.
     */
    void Stop();

    /**
     * @brief Sends a message to the client.
     * 
     * @param message The message to send.
     */
    void Send(const std::string& message);

    /**
     * @brief Receives and processes messages from the client.
     * 
     * This method runs in a separate thread and continuously
     * reads messages from the client socket.
     */
    void ReceiveMessages();

private:
    int id_;                    ///< Unique identifier for this session
    int socket_;                ///< Client socket file descriptor
    std::string nickname_;      ///< Client's nickname
    bool running_;              ///< Flag to indicate if the session is running

    std::thread receive_thread_; ///< Thread for receiving messages from the client
    std::thread send_thread_;    ///< Thread for sending messages to the client (if needed)

    // Queue for outgoing messages (if needed)
    // std::queue<std::string> send_queue_;
    // std::mutex send_queue_mutex_;
    // std::condition_variable send_condition_;
};

} // namespace chat_room

#endif // CHAT_SESSION_H
