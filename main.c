#include <stdio.h>
#include <stdlib.h>
#include <enet/enet.h>

#define BUFFER_SIZE 1024

int main(int argc, char **argv)
{
    printf("Hello world!\n");

    if (enet_initialize() != 0)
    {
        printf("An error occurred while initializing ENet.\n");
        return EXIT_FAILURE;
    }

    ENetAddress address;
    ENetHost *server;
    ENetEvent enetEvent;
    char buffer[BUFFER_SIZE];
    uint16_t serverPort = 30287;

    address.host = ENET_HOST_ANY;
    address.port = serverPort;

    server = enet_host_create(&address, 2, 2, 0, 0);

    if (server == NULL)
    {
        printf("An error occurred while trying to create an ENet server host.\n");
        enet_deinitialize();
        return EXIT_FAILURE;
    }

    printf("Server started on port %d\n", serverPort);

    int quit = 0;

    while (!quit)
    {
        while (enet_host_service(server, &enetEvent, 0) > 0)
        {
            switch (enetEvent.type)
            {
            case ENET_EVENT_TYPE_CONNECT:
                printf("A new client connected from %x:%u.\n",
                       enetEvent.peer->address.host,
                       enetEvent.peer->address.port);
                enetEvent.peer->data = "Client information";
            case ENET_EVENT_TYPE_RECEIVE:
                printf("A packet of length %u containing %s was received from %s on channel %u.\n",
                       enetEvent.packet->dataLength,
                       enetEvent.packet->data,
                       enetEvent.peer->data,
                       enetEvent.channelID);
                enet_packet_destroy(enetEvent.packet);
            case ENET_EVENT_TYPE_DISCONNECT:
                printf("%s disconnected.\n", enetEvent.peer->data);
                enetEvent.peer->data = NULL;
            }
        }
    }

    enet_host_destroy(server);
    enet_deinitialize();
    return 0;
}