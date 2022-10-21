# EfEfNet-Library
Game development focused Networking Library, C++ library that aims to simplyfy acces to network programming

Features:
  - Object-oriented networking structures (udp, tcp, addresses...)
  - Reliable UDP socket
  - Input and output streams for serialization
  - Manager class to update the librarys objects and ease multi-socket programming
  - Server class that provides part of the server side logic
  
 How to use:
 
  Basic functionality:
  - Call Init() to start the library, using library functionality before this call will cause errors
  - Call CleanUp() to stop the library, using library functionality after this call will cause errors
  - Call Poll() to check if a socket is available for a certain action, sending, receiving or exeption.
  - Call Select() to check which sockets in a set of sockets are available for a certain action, sending, receiving or exeption.
  
  Socket Address:
    - Stores a IPv4 address
    - Call fill() or use Constructor to input the IP and port
    
  UDP Socket:
    - Creates and allows the use of a socket using the UDP protocol, use CreateUDPSocket() to create one
    - Call bind() to assign a local Socket Address to the socket
    - Call send_to() to send an array of bytes to a desired remote Socket Address
    - Call receive_from() to gather any data that arrived to the socket
    
  TCP Socket:
    - Creates and allows the use of a socket using the TCP protocol, use CreateTCPSocket() to create one
    - Call bind() to assign a local Socket Address to the socket
    - Call listen() to set the socket on listen mode
    - Call connect() to send a connection request to a remote address on listen mode
    - Call accept() from a listen mode socket to accept connection requests
    - Call send() from a connected socket to send an array of bytes to the other end-point of the connection
    - Call receive() to gather any data that arrived to the socket
    
  Fast Socket (reliable UDP):
    - Creates and allows the use of a socket using the UDP protocol with extra reliability mesures, use CreateFastSocket() to create one
    - Call bind() to assign a local Socket Address to the socket
    - Call connect() to assign a remote Socket Address to the socket
    - Change the send_rate_time to modify how often the socket sends packets
    - Call send() to schedule an array of bytes to be sent
    - Call force_send() to force the socket to send any scheduled messages ignoring the send rate
    - Call receive() to get a vector of the messages that have been received, this messages are dynamically alocated so they should be deleted by the user
    - Call disconnect() to start the disconnect procces
    - Call unilateral_disconnect() to disconnect disregarding the acknowledgement of the other end-point
    - Change accesState to set the socket as PUBLIC or PRIVATE
    - Use add_to_list(), erase_from_list(), erase_from_list, and erase_list() to manage what addresses can send data to the socket
    
  Server:
    - Multi-socket structure that waits for incoming clients and assigns them a socket to communicate with
    - Call bind() to assign a local Socket Address to the listen Fast Socket
    - Send a packet with a name to the listen Fast Socket from a remote Fast Socket to register in the server, then you will receive a message with the new socket           assigned to you as a client
    - Call send_to() to send an array of bytes to a certain client
    - Call pendant_clients() to get a vector of all the clients with unread messages, the memory of this messages is managed by the server
    - Change first_port to set from which port the server will start assignin the new client Fast Sockets
    - Use clients to get acces to all the clients registered in the server
    
  Manager:
    - Created when initializing the library with Init()
    - Call instance() to get a pointer to the manager
    - Call update() to update all the sockets and servers you created
    - Use the sockets vector to have acces to all the sockets you created
    - Use the servers vector to have acces to all the server you created

Developed by David Rami Rodriguez
