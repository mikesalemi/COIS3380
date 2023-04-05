// socket_packet_info.h

// This file contains the struct used to send and receive data between the soket created in 
// the client and server programs. The macro N_BYTES defines the size of packet.data, which 
// is used to transfer to the opposing end. packet.nbytes is an integer containing the number
// of chars that are in packet.data (used for recv function).

#define N_BTYTES 1024 // max chars to read per data packet

typedef struct {
    int nbytes; // number of chars read
    char data[N_BTYTES]; // char buffer
} packet;