#ifndef NETWORK_H
#define NETWORK_H

#include "Serializer.h"
#include "Socket.h"

namespace UDP_TransferNT {
	class Network : public Serializer {
	 public:

		/**
		 * Network Type
		 */
		enum class Type {
			SERVER = 0,
			CLIENT
		};

		/**
		 * Connection type
		 * Meant for client connection to either a specific ip address or any ip address on the same port
		 */
		enum class ConnectionType {
			ANY = 0,
			IP_SPECIFIC
		};

		/**
		 * Connection Status
		 */
		enum class ConnectionStatus {
			DISCONNECTED = 0,
			CONNECTING,
			CONNECTED
		};

		/**
		 * Network state
		 */
		enum class State {
			UN_INITIALIZED = 0,
			IDLE,
			RUNNING
		};

		/**
		 * Main network, initialize as either server or client with optional override values
		 */
		Network(Type type, ConnectionType connType, int port = DEFAULT_NT_PORT, const char *ip = DEFAULT_NT_IP) : _type(type), _connType(connType) {
			// Set the socket vals
			_socket.setPort(port);
			_socket.setIP(ip);

			DEFAULT_NT_LOGGER("Network Constructed");
		}

		~Network() {
			_socket.killSocket();
			DEFAULT_NT_LOGGER("Network Destroyed");
		}

		Socket &getSocket() {
			return _socket;
		}

		/**
		 * Initialize connection using defined socket value set by user and constructed network instance
		 */
		void init() {
			while (_connStat != ConnectionStatus::CONNECTED) {
				int programValue = 0;
				if (_connStat == ConnectionStatus::CONNECTING) {
					DEFAULT_NT_LOGGER("Closing Socket, reconnecting...");
					_socket.killSocket();
				}

				_connStat = ConnectionStatus::CONNECTING;

				DEFAULT_NT_LOGGER("Opening socket...");

				switch (_type) {
					case Type::SERVER: // SERVER
						if (programValue + _socket.createSocket() != 0) {
							DEFAULT_NT_LOGGER("Error creating socket");
						} else {
							DEFAULT_NT_LOGGER("Created socket");
						}

						if (_connType == ConnectionType::ANY) {
							_socket.prepSocketStructure();
						} else if (_connType == ConnectionType::IP_SPECIFIC) {
							_socket.prepSocketStructure(false, true);
						}

						if (programValue + _socket.bindSocket() != 0) {
							DEFAULT_NT_LOGGER("Error binding socket");
						} else {
							DEFAULT_NT_LOGGER("Socket Binded");
						}
						break; // SERVER END

					case Type::CLIENT: // CLIENT
						if (programValue + _socket.createSocket(true) != 0) {
							DEFAULT_NT_LOGGER("Error creating socket");
						} else {
							DEFAULT_NT_LOGGER("Socket created");
						}

						if (_connType == ConnectionType::ANY) {
							_socket.prepSocketStructure(true);
						} else if (_connType == ConnectionType::IP_SPECIFIC) {
							_socket.prepSocketStructure(true, true);
							_socket.connectSocket(true);
						}

						break; // CLIENT END
				}

				#ifdef NT_UDP_PLATFORM_WINDOWS
					DWORD timeout = _socket.getRecvTimeout();
					setsockopt(_socket.getSocket(), SOL_SOCKET, SO_RCVTIMEO, (const char *)&timeout, sizeof(timeout));
				#elif defined(NT_UDP_PLATFORM_UNIX)
					struct timeval tv;
					tv.tv_sec = 0;
					tv.tv_usec = _socket.getRecvTimeout() * 1000;
					setsockopt(_socket.getSocket(), SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv, sizeof(tv));
				#endif

				if (programValue == 0) {
					// Display the vals
					DEFAULT_NT_LOGGER("Network Initialized with values");
					DEFAULT_NT_LOGGER("Type: " + std::to_string((int)_type));
					DEFAULT_NT_LOGGER("Connection Type: " + std::to_string((int)_connType));
					DEFAULT_NT_LOGGER("Port: " + std::to_string(_socket.getPort()));
					DEFAULT_NT_LOGGER(_socket.getIP());
					DEFAULT_NT_LOGGER("Buffer Size: " + std::to_string(DEFAULT_BUFFER_SIZE));
					DEFAULT_NT_LOGGER("Packet size: " + std::to_string(PACKETSIZE));
					_connStat = ConnectionStatus::CONNECTED;
					_state = State::IDLE;
				} else {
					DEFAULT_NT_LOGGER("Init Issue, trying again...");
				}
			}
		}

		/**
		 * Reset network
		 * Closes socket and re-runs the connection process
		 */
		void reset() {
			_state = State::UN_INITIALIZED;
			_connStat = ConnectionStatus::CONNECTING;
			init();
		}


		/**
		 * Raw send
		 * Send a buffer with max size of the bufferSize
		 */
		int raw_send(const char buffer[DEFAULT_BUFFER_SIZE]) {
			int programValue = 0;
			int sendVal = 0;
			if (_connStat == ConnectionStatus::CONNECTED) {
				_state = State::RUNNING;
				switch (_type) {
					case Type::SERVER:
						#ifdef NT_UDP_PLATFORM_WINDOWS
						if (sendto(_socket.getSocket(), buffer, DEFAULT_BUFFER_SIZE, 0, (struct sockaddr *)&_socket.getOtherAddress(), *_socket.getOtherAddressLength()) == SOCKET_ERROR) {
							DEFAULT_NT_LOGGER("Server Send error windows: " + std::to_string(WSAGetLastError()));
							programValue = 1;
						}
						#elif defined(NT_UDP_PLATFORM_UNIX)
						if ((sendVal = sendto(_socket.getSocket(), buffer, DEFAULT_BUFFER_SIZE, 0, (struct sockaddr *)&_socket.getOtherAddress(), *_socket.getOtherAddressLength())) < 0) {
							DEFAULT_NT_LOGGER("Server Send error unix: " + std::to_string(sendVal));
							programValue = 1;
						}
						#endif
						break;

					case Type::CLIENT:
						#ifdef NT_UDP_PLATFORM_WINDOWS
						if (sendto(_socket.getSocket(), buffer, DEFAULT_BUFFER_SIZE, 0, (struct sockaddr *)&_socket.getOtherAddress(), *_socket.getOtherAddressLength()) == SOCKET_ERROR) {
							DEFAULT_NT_LOGGER("Client Send error windows: " + std::to_string(WSAGetLastError()));
							programValue = 1;
						}
						#elif defined(NT_UDP_PLATFORM_UNIX)
						if ((sendVal = sendto(_socket.getSocket(), buffer, DEFAULT_BUFFER_SIZE, 0, (struct sockaddr *)&_socket.getOtherAddress(), *_socket.getOtherAddressLength())) < 0) {
							DEFAULT_NT_LOGGER("Client Send error unix: " + std::to_string(sendVal));
							programValue = 1;
						}
						#endif
						break;
				}
				_state = State::IDLE;
			} else {
				programValue = 1;
			}
			return programValue;
		}

		/**
		 * Raw receive
		 * receive a raw buffer with a max buffersize
		 */
		int raw_recv(char *buffer) {
			int programValue = 0;
			int recvVal = 0;
			memset(buffer, 0, DEFAULT_BUFFER_SIZE);
			if (_connStat == ConnectionStatus::CONNECTED) {
				_state = State::RUNNING;
				switch (_type) {
					case Type::SERVER:
						#ifdef NT_UDP_PLATFORM_WINDOWS
						if (recvfrom(_socket.getSocket(), buffer, DEFAULT_BUFFER_SIZE, 0, (struct sockaddr *)&_socket.getOtherAddress(), _socket.getOtherAddressLength()) == SOCKET_ERROR) {
							DEFAULT_NT_LOGGER("Server Recv error windows: " + std::to_string(WSAGetLastError()));
							programValue = 1;
						}

						#elif defined(NT_UDP_PLATFORM_UNIX)
						if ((recvVal = recvfrom(_socket.getSocket(), buffer, DEFAULT_BUFFER_SIZE, 0, (struct sockaddr *)&_socket.getOtherAddress(), _socket.getOtherAddressLength())) < 0) {
							DEFAULT_NT_LOGGER("Server Recv error unix: " + std::to_string(recvVal));
							programValue = 1;
						}
						#endif
						break;

					case Type::CLIENT:
						#ifdef NT_UDP_PLATFORM_WINDOWS
						if (recvfrom(_socket.getSocket(), buffer, DEFAULT_BUFFER_SIZE, 0, (struct sockaddr *)&_socket.getOtherAddress(), _socket.getOtherAddressLength()) == SOCKET_ERROR) {
							DEFAULT_NT_LOGGER("Client Recv error windows: " + std::to_string(WSAGetLastError()));
							programValue = 1;
						}
						#elif defined(NT_UDP_PLATFORM_UNIX)
						if ((recvVal = recvfrom(_socket.getSocket(), buffer, DEFAULT_BUFFER_SIZE, 0, (struct sockaddr *)&_socket.getOtherAddress(), _socket.getOtherAddressLength())) < 0) {
							DEFAULT_NT_LOGGER("Client Recv error unix: " + std::to_string(recvVal));
							programValue = 1;
						}
						#endif
						break;
				}
				_state = State::IDLE;
			} else {
				programValue = 1;
			}

			return programValue;
		}

		/**
		 * Send over a serialized datapacket
		 */
		void dpSend(DataPacket &dp) {
			if (_connStat == ConnectionStatus::CONNECTED) {
				DataPacket dpBuff = dp;
				char buffer[DEFAULT_BUFFER_SIZE];
				serialize(dpBuff, buffer);
				raw_send(buffer);
			}
		}

		/**
		 * receive a deserialized datapacket
		 * Use previously received datapacket as paramater to get streamline data.
		 * (Stops random zeroed out datapacket from being returned if no datapacket is received)
		 */
		DataPacket dpRecv(DataPacket dpPrevious = {0}) {
			if (_connStat == ConnectionStatus::CONNECTED) {
				DataPacket dpBuffer;
				char *recvBuffer = (char *)malloc(DEFAULT_BUFFER_SIZE * sizeof(char));
				if (raw_recv(recvBuffer) == 0) {
					deserialize(dpBuffer, recvBuffer);
					free(recvBuffer);
					return dpBuffer;
				}
			}

			return dpPrevious;
		}

	 private:
		Type _type;
		ConnectionType _connType{ ConnectionType::ANY };

		// Socket
		Socket _socket;

		// connections and states
		ConnectionStatus _connStat{ ConnectionStatus::DISCONNECTED };
		State _state{ State::UN_INITIALIZED };
	};
}

#endif // NETWORK_H