# SHUTARUBETAARJAAL

**Vendor-Agnostic Self-Forming Tree Wireless Networking Framework**

**Developer:** Shubhendu Bikash Banerjee

**Language:** C11

## Project Identity

SHUTARUBETAARJAAL is the project name for this vendor-agnostic self-forming tree wireless networking framework. The networking port is intended to be implemented by the user for the selected hardware, RTOS, SDK, and networking technology.

## Current Implementation Structure

The initial implementation is maintained as a single C source/header pair:

```text
SHUTARUBETAARJAAL
├── shutarubetaarjaal.c
└── shutarubetaarjaal.h
```

Amalgamation of multiple source files is not part of the current build or
development workflow. It may be introduced later if the project grows and
there is a clear requirement for a generated single-file distribution.

The `port` implementation remains outside the core library and is implemented
by the consuming project for the selected hardware, RTOS, SDK, and networking
technology.

# Wi-Fi Local Mesh Architecture

## 1. Purpose

The system provides a self-forming local wireless network between
multiple nodes with different characteristics, such as controllers,
sensors, and gateways.

The network continuously attempts to maintain the best available
compatible connection between nodes.

Internet/infrastructure-router connectivity is optional. Local node
connectivity must remain possible even when no infrastructure router is
available.

The architecture keeps the application independent of the selected
networking technology and vendor-specific implementation.

## 2. Layered Architecture

``` text
+---------------------------------------------------------------+
|                         APPLICATION                            |
|                                                               |
|  Product/application logic, node behaviour, sensor/control    |
|  logic, application protocol and business functionality       |
+-------------------------------+-------------------------------+
                                |
+-------------------------------+-------------------------------+
|            SHUTARUBETAARJAAL, the self-forming tree networking stack            |
+---------------------------------------------------------------+ 
|         CONNECTION MANAGEMENT | PACKET MANAGEMENT             |
|                               |                               |
|  Connection Management        | Packet Management             |
|  - Node discovery             | - Packet framing              |
|  - Node characteristics       | - Packet validation           |
|  - Compatibility matching     | - Packet TX/RX                |
|  - Candidate selection        | - Sequence management         |
|  - RSSI evaluation            | - Duplicate handling          |
|  - Upstream selection         | - Forwarding where required   |
|  - Connection establishment   | - Application packet delivery |
|  - Recovery / switching       |                               |
+-------------------------------+-------------------------------+
                                |
+---------------------------------------------------------------+
|                 NETWORKING STACK - TCP/IP                     |
|                                                               |
|  Vendor-agnostic networking interface                         |
|                                                               |
|  - IPv4 / IPv6                                                |
|  - TCP                                                        |
|  - UDP                                                        |
|  - IP addressing                                               |
|  - Socket abstraction                                          |
|  - Network-interface abstraction                               |
+---------------------------------------------------------------+
                                |
+---------------------------------------------------------------+
|                      NETWORKING PORT                          |
|                                                               |
|  Hardware / RTOS / SDK / vendor-specific implementation       |
|                                                               |
|  Example: ESP32 / ESP-IDF Wi-Fi                               |
|                                                               |
|  - Wi-Fi initialization                                        |
|  - STA/AP operation                                            |
|  - Wi-Fi scanning                                              |
|  - Advertisement/vendor-IE handling                            |
|  - Association/authentication                                  |
|  - RSSI acquisition                                            |
|  - Link/interface events                                       |
|  - TCP/IP integration                                          |
+---------------------------------------------------------------+
```

## 3. Application

The application contains product-specific functionality:

-   Sensor acquisition
-   Controller operation
-   Local control logic
-   Measurement processing
-   Application protocol
-   Cloud communication
-   Device-specific behaviour

The application shall not directly depend on vendor-specific Wi-Fi APIs.

``` text
Application
    |
    +--> Connection Management
    |
    +--> Packet Management
```

## 4. Connection Management

Connection management controls node relationships and upstream
selection.

### Responsibilities

-   Infrastructure AP discovery
-   Node advertisement discovery
-   Node-characteristic parsing
-   Compatibility evaluation
-   Candidate management
-   RSSI evaluation
-   Connection selection
-   Connection establishment
-   Connection-loss detection
-   Connection recovery
-   Connection switching
-   Upstream selection
-   Local topology state

A connection candidate may be either:

``` text
Connection Candidate
    |
    +-- Infrastructure Router / AP
    |
    +-- Compatible Node
```

The infrastructure router is therefore not a mandatory dependency.

## 5. Node Advertisement

Every node continuously advertises its characteristics independently of
whether it has an upstream connection.

The advertisement may contain:

``` text
Protocol version
Node ID
Node type
Capability bitmap
Product/application ID
Firmware version
Network ID
Connection state
Upstream type
Hop count
Sequence number
```

Example:

``` text
Node ID       : 0x0104
Node Type     : SENSOR
Capability    : LEVEL | FLOW
Protocol      : 1
Firmware      : 0x0203
Network ID    : 0x01
Upstream      : NODE
Hop Count     : 2
```

For the initial ESP32 implementation, node advertisement can use Wi-Fi
management-frame vendor-specific information elements.

## 6. Candidate Selection

Connection management maintains a list of discovered candidates.

``` text
+-----------+------------+----------+----------------+
| Candidate | Type       | RSSI     | Compatibility  |
+-----------+------------+----------+----------------+
| Router    | INFRA      | -67 dBm  | YES            |
| Node 001  | CONTROLLER | -42 dBm  | YES            |
| Node 003  | SENSOR     | -58 dBm  | YES            |
| Node 004  | SENSOR     | -71 dBm  | NO             |
+-----------+------------+----------+----------------+
```

Candidates are filtered by compatibility before connection selection.

RSSI is one input to the connection-quality score. The scoring algorithm
should remain replaceable.

## 7. Connection Hysteresis

A node must not switch connections because of a small RSSI difference.

Example:

``` text
Current:   Node A = -50 dBm
Candidate: Node B = -48 dBm

Difference = 2 dB
Action     = keep Node A
```

Use a configurable switching margin:

``` text
switch if:

new_score > current_score + SWITCH_MARGIN
```

This prevents connection oscillation.

## 8. Packet Management

Packet management handles packets after a connection has been selected.

Responsibilities:

-   Packet creation
-   Packet framing
-   Packet validation
-   Packet transmission
-   Packet reception
-   Packet sequencing
-   Packet length validation
-   Duplicate detection where required
-   Forwarding where required
-   Delivery to the application

Packet management does not decide which node should be connected.

``` text
Connection Management
        |
        | selected connection
        v
Packet Management
        |
        v
Networking Stack
```

## 9. Networking Stack

The networking stack provides a vendor-agnostic TCP/IP abstraction.

``` text
+--------------------------------+
|       Connection Management    |
+--------------------------------+
|          Packet Management     |
+--------------------------------+
|                                |
|          TCP / UDP             |
|          IPv4 / IPv6           |
|          Socket API            |
|                                |
+--------------------------------+
|       Networking Port          |
+--------------------------------+
|     Vendor-specific Wi-Fi      |
+--------------------------------+
```

The upper layers shall not directly use vendor-specific APIs such as:

``` text
esp_wifi_*
esp_netif_*
esp_event_*
```

Those belong below the networking-port boundary.

## 10. Event-Driven Architecture

The framework is event driven. External events are generated by the port or
application and enter the framework through a single event-post path.

The event flow is:

```text
PORT / APPLICATION
        |
        v
sbj_evt_gnrt_XXXX
        |
        v
SINGLE EVENT POST FUNCTION
        |
        v
SINGLE INTERNAL EVENT HANDLER
        |
        +-------------------+-------------------+
        |                   |                   |
        v                   v                   v
 CONNECTION SM        PACKET SM          OTHER STATE MACHINES
```

`sbj_evt_gnrt_XXXX` identifies the event-generation/event-enumeration side of
the framework. It does not represent separate event-consumer functions.

There is one internal event handler. The handler dispatches events to the
relevant state machines. Multiple state machines may independently consume
events according to their responsibilities.

The networking port is responsible for invoking the framework's event path
when platform or networking events occur. The framework owns the event
definitions, event-post interface, internal event handling, and state-machine
processing.

The port interface is defined by SHUTARUBETAARJAAL and implemented by the
consuming project. Platform-specific callbacks, function-pointer tables, and
required port-side structures are supplied by that implementation.

## 11. Networking Port

The networking port adapts the selected hardware/SDK implementation to
the vendor-independent networking stack.

Conceptually:

``` c
typedef struct {
    int (*init)(void);
    int (*start)(void);

    int (*scan)(void);
    int (*connect)(const void *pConfig);
    int (*disconnect)(void);

    int (*get_rssi)(int *pRssi);

    int (*start_advertisement)(const void *pData, uint32_t dataLen);
    int (*stop_advertisement)(void);

    int (*register_advertisement_callback)(void *pCallback);
} networking_PortApi_t;
```

The exact interface is to be defined separately from the ESP32
implementation.

``` text
                    Vendor-independent API
                             |
              +--------------+--------------+
              |                             |
       networking_port_esp32        networking_port_other
              |                             |
          ESP-IDF Wi-Fi               Other vendor SDK
```

## 12. Concurrent Operation

AP/beacon operation must not depend on successful infrastructure
connectivity.

These functions operate concurrently:

``` text
+------------------------------------------------+
|                    NODE                        |
|                                                |
|  +------------------+  +--------------------+  |
|  | STA connectivity |  | Node advertisement |  |
|  |                  |  |                    |  |
|  | Router candidate |  | SoftAP / beacon    |  |
|  | Node candidate   |  | characteristics    |  |
|  | Connection       |  | discovery          |  |
|  +------------------+  +--------------------+  |
|                                                |
|              Connection Manager                |
+------------------------------------------------+
```

### Infrastructure available

``` text
Node
 |
 +-- STA --> Router
 |
 +-- AP  --> Local nodes
```

### Infrastructure unavailable

``` text
Node
 |
 +-- STA --> Best compatible node
 |
 +-- AP  --> Local nodes
```

### All infrastructure connections unavailable

``` text
             Local Network
                  |
          +-------+-------+
          |               |
        Node A           Node B
          |               |
        Node C           Node D
```

The local network remains operational.

## 13. Self-Forming Network

Nodes dynamically select an upstream connection.

Example:

``` text
                    ROUTER
                       |
                 +-----+-----+
                 |           |
              NODE A       NODE D
                 |
              NODE B
                 |
              NODE C
```

If the router becomes unavailable:

``` text
                 NODE A
                /                   NODE B   NODE D
                |
             NODE C
```

The local network remains operational.

When the router becomes available again, connection management may
select it according to the configured connection policy.

## 14. Logical State Machine

``` text
                         POWER UP
                            |
                            v
                   START AP + STA
                            |
             +--------------+--------------+
             |                             |
             v                             v
       START NODE                    STA DISCOVERY
       ADVERTISEMENT                       |
             |                    +--------+--------+
             |                    |                 |
             |                 ROUTER             NODE
             |                 FOUND              FOUND
             |                    |                 |
             |                    +--------+--------+
             |                             |
             |                       COMPATIBILITY
             |                             |
             |                             v
             |                       SCORE CANDIDATE
             |                             |
             |                             v
             |                       SELECT BEST
             |                             |
             |                             v
             |                       CONNECT STA
             |                             |
             +-------------+---------------+
                           |
                           v
                    MONITOR CONNECTION
                           |
                +----------+----------+
                |                     |
             ACCEPTABLE           DEGRADED/LOST
                |                     |
                |                     v
                |               RE-EVALUATE
                |                     |
                +<--------------------+
```

The node-advertisement path remains active throughout the state machine.
State transitions are driven through the framework event path rather than
direct vendor-specific calls from upper layers.

## 15. Separation of Concerns

The intended dependency direction is:

``` text
APPLICATION
    |
    | product behaviour
    v
CONNECTION MANAGEMENT
    |
    | node relationship
    v
PACKET MANAGEMENT
    |
    | packet transport
    v
NETWORKING STACK
    |
    | TCP/IP abstraction
    v
NETWORKING PORT
    |
    | vendor-specific implementation
    v
WIFI / HARDWARE
```

The application shall not directly call vendor Wi-Fi APIs.

## 16. Initial ESP32 Implementation

The first implementation target is ESP32 with ESP-IDF:

``` text
Application
    |
Connection Management
    |
Packet Management
    |
TCP/IP
    |
Networking Port
    |
ESP-IDF Wi-Fi
    |
ESP32 Wi-Fi hardware
```

Initial networking mechanisms:

-   STA mode for upstream connectivity
-   SoftAP mode for local node connectivity
-   AP+STA concurrent operation
-   Wi-Fi management-frame/vendor-specific IE for node advertisement
-   RSSI from received node advertisements
-   TCP/UDP for established node communication

The upper layers shall not depend on these ESP32-specific mechanisms.

## 17. Core Design Objective

The system shall provide:

``` text
                    Internet
                       |
                    Router
                       |
              +--------+--------+
              |                 |
           Controller         Sensor
              |                 |
              +-------+---------+
                      |
                    Sensor
```

while also supporting:

``` text
              Router unavailable
                      X
                      |
                 Controller
                 /                     Sensor      Controller
                |
             Sensor
```

The essential property is:

> Internet connectivity is an optional upstream service. Local node
> connectivity is independent and shall remain available whenever
> compatible nodes can establish a local wireless path.

The architecture deliberately separates:

-   Application
-   Connection Management
-   Packet Management
-   TCP/IP Networking Stack
-   Networking Port

so that application and networking logic remain portable across Wi-Fi
vendors and platforms.

## 18. License

SHUTARUBETAARJAAL is released under the **Apache License, Version 2.0**.

Copyright © 2026 Shubhendu Bikash Banerjee.

The Apache License 2.0 is an OSI-approved open source license. It permits
use, reproduction, modification, distribution, sublicensing, and commercial
use of the software, subject to the terms and conditions of the license.

The license also provides an express patent license from contributors,
subject to its terms.

The complete license text is provided in the `LICENSE` file in this
repository and is also available from:

https://www.apache.org/licenses/LICENSE-2.0

SPDX-License-Identifier: Apache-2.0

### Commercial Use

Commercial use is permitted under the Apache License 2.0. No separate
commercial license is required for the software itself when it is used in
accordance with the Apache License 2.0.

### Contributions

Unless a contributor explicitly states otherwise, contributions submitted
to this project are intended to be licensed under the same Apache License
2.0 terms as the project. Contributors retain copyright in their original
contributions while granting the permissions required by the license.

### Trademark

The project name **SHUTARUBETAARJAAL** and associated project branding are
not granted as trademarks by the Apache License 2.0. Use of the project
name or branding does not imply endorsement by the copyright holder.

