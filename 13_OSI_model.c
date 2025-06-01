/*
OSI Model - Graphical Representation with Layer Descriptions

    +----------------------+
7   |  Application Layer   |  <-- Provides network services to end-user applications 
    +----------------------+
6   | Presentation Layer   |  <-- Translates data formats, encrypts/decrypts, compresses data
    +----------------------+
5   |    Session Layer     |  <-- Manages sessions and controls dialogues between computers
    +----------------------+
4   |   Transport Layer    |  <-- Ensures reliable data transfer with error checking and flow control
    +----------------------+
3   |    Network Layer     |  <-- Handles routing and forwarding of data packets
    +----------------------+
2   | Data Link Layer      |  <-- Provides node-to-node data transfer and error detection
    +----------------------+
1   |   Physical Layer     |  <-- Transmits raw bit streams over physical medium
    +----------------------+


RECAP EASY:
- Layer 7: is what the user sees and interacts with, like web browsers or email, app.
- Layer 6: translates data formats, encrypts/decrypts, compresses data.
- Layer 5: manages sessions and controls dialogues between computers.
- Layer 4: ensures reliable data transfer (TCP, UDP).
- Layer 3: handles routing and forwarding of data packets (IP).
- Layer 2: provides physical node-to-node data transfer and error detection (Ethernet, MAC address).
- Layer 1: transmits raw bit(1|0) streams over physical medium (cables, radio waves).

COMMUNICATION OVER A NETWORK:
 When comunicating over a network, each packets contains an header and a payload for each layer.
 While sending it, the physical layer is the outermost layer, the packet is sent through
 the internet, through routers, that read only up to layer 3 (Network Layer, IP) till the destination.
 Once it arrives, the packet is read from the Physical Layer (1) to the Application Layer (7), like peeling an onion.

These protocols are programmed into routers, firewalls, and you computer's operating system so they can communicate.
*/

