#ifndef MJPEG_WRITER_H
#define MJPEG_WRITER_H

#define PORT        unsigned short
#define SOCKET    int
#define HOSTENT  struct hostent
#define SOCKADDR    struct sockaddr
#define SOCKADDR_IN  struct sockaddr_in
#define ADDRPOINTER  unsigned int*
#define INVALID_SOCKET -1
#define SOCKET_ERROR   -1
#define TIMEOUT_M       200000
#define NUM_CONNECTIONS 10

#include "common_headers.h"
#include "UDP_TransferNT.h"
#include <sys/signal.h>
#include <pthread.h>

struct clientFrame {
		uchar* outbuf;
		int outlen;
		int client;
};

struct clientPayload {
		void* context;
		clientFrame cf;
};

class MJPEGWriter{
		SOCKET sock;
		fd_set master;
		int timeout;
		int quality; // jpeg compression [1..100]
		std::vector<int> clients;
		pthread_t thread_listen, thread_write;
		pthread_mutex_t mutex_client = PTHREAD_MUTEX_INITIALIZER;
		pthread_mutex_t mutex_cout = PTHREAD_MUTEX_INITIALIZER;
		pthread_mutex_t mutex_writer = PTHREAD_MUTEX_INITIALIZER;
		cv::Mat lastFrame;
		int port;

		int _write(int sock, char *s, int len)
		{
				if (len < 1) { len = strlen(s); }
				{
					try
					{
						int retval = ::send(sock, s, len, 0x4000);
						return retval;
					}
					catch (int e)
					{
						CJ_CORE_PRINT_ERROR("An exception occurred. Exception Nr. " + std::to_string(e));
					}
				}
				return -1;
		}
		
		int _read(int socket, char* buffer)
		{
				int result;
				result = recv(socket, buffer, 4096, MSG_PEEK);
				if (result < 0 )
				{
						CJ_CORE_PRINT_ERROR("An exception occurred. Exception Nr. " + std::to_string(result));
						return result;
				}
				std::string s = buffer;
				buffer = (char*) s.substr(0, (int) result).c_str();
				return result;
		}

		static void* listen_Helper(void* context)
		{
				((MJPEGWriter *)context)->Listener();
				return NULL;
		}

		static void* writer_Helper(void* context)
		{
				((MJPEGWriter *)context)->Writer();
				return NULL;
		}

		static void* clientWrite_Helper(void* payload)
		{
				void* ctx = ((clientPayload *)payload)->context;
				struct clientFrame cf = ((clientPayload *)payload)->cf;
				((MJPEGWriter *)ctx)->ClientWrite(cf);
				return NULL;
		}

public:

		MJPEGWriter(int port = 0)
				: sock(INVALID_SOCKET)
				, timeout(TIMEOUT_M)
				, quality(90)
	, port(port)
		{
				signal(SIGPIPE, SIG_IGN);
				FD_ZERO(&master);
				// if (port)
				//     open(port);
		}

		~MJPEGWriter()
		{
				release();
		}

		bool release()
		{
				if (sock != INVALID_SOCKET)
						shutdown(sock, 2);
				sock = (INVALID_SOCKET);
				return false;
		}

		bool open()
		{
				sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

				SOCKADDR_IN address;
				address.sin_addr.s_addr = INADDR_ANY;
				address.sin_family = AF_INET;
				address.sin_port = htons(port);
				if (::bind(sock, (SOCKADDR*)&address, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
				{
						CJ_CORE_PRINT_ERROR("Error, could not bind to socket: " + std::to_string(sock));
						return release();
				}
				if (listen(sock, NUM_CONNECTIONS) == SOCKET_ERROR)
				{
						CJ_CORE_PRINT_ERROR("Error, could not listen on sock: " + std::to_string(sock));
						return release();
				}
				FD_SET(sock, &master);
				return true;
		}

		bool isOpened()
		{
				return sock != INVALID_SOCKET;
		}

		void start(){
				pthread_mutex_lock(&mutex_writer);
				pthread_create(&thread_listen, NULL, this->listen_Helper, this);
				pthread_create(&thread_write, NULL, this->writer_Helper, this);
		}

		void stop(){
			this->release();
				pthread_join(thread_listen, NULL);
				pthread_join(thread_write, NULL);
		}

		void write(cv::Mat frame){
			pthread_mutex_lock(&mutex_writer);
			if(!frame.empty()){
				lastFrame.release();
				lastFrame = frame.clone();
			}
			pthread_mutex_unlock(&mutex_writer);
		}

private:
		void Listener();
		void Writer();
		void ClientWrite(clientFrame &cf);
};

#endif