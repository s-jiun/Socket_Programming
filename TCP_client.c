#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main() {
	const char* server_name = "localhost"; // 127.0.0.1 
	const int server_port = 8875;

	struct sockaddr_in server_address; // Create socket structure
	memset(&server_address, 0, sizeof(server_address)); // Initialize memory space with zeros
	server_address.sin_family = AF_INET; // IPv4

	inet_pton(AF_INET, server_name, &server_address.sin_addr);  // Convert IP addr. to binary  주소 패밀리 
	// inet_aton and inet_addr => IPv4 Only
	// server_address.sin_addr = Binary IP address

	// htons: port in network order format
	server_address.sin_port = htons(server_port); 
	
	/*htonl()함수는 long intger(일반적으로 4byte)데이터를 네트워크 byte order로 변경한다.
	htons()함수는 short intger(일반적으로 2byte)데이터를 네트워크 byte order로 변경한다.
	ntohl()함수는 long intger 데이터를 호스트 byte order로 변경한다.
	ntohs()함수는 short intger 데이터를 호스트 byte order로 변경한다.
	*/
	
	/* little endian and big endian
	CPU마다 데이터 저장 방식 다름. 
	0A0B0C0D -> 0D 0C 0B 0A or 0A 0B 0C 0D
	
	Network: Big endian! 통일.. 
	Intel CPU -> Little endian!
	*/

	// open a stream socket
	int sock;
	if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0) { // IPv4 + TCP .. 프로토콜패밀리 
		printf("could not create socket\n");
		return 1;
	}

	// TCP is connection oriented, a reliable connection
	// **must** be established before any data is exchanged
	if (connect(sock, (struct sockaddr*)&server_address,
	            sizeof(server_address)) < 0) {
		printf("could not connect to server\n");
		return 1;
	}

	// send

	// data that will be sent to the server
	const char* data_to_send = "Hello!";
	send(sock, data_to_send, strlen(data_to_send), 0);

	// receive

	int n = 0;
	int len = 0, maxlen = 100;
	char buffer[maxlen];
	char* pbuffer = buffer;

	// will remain open until the server terminates the connection
	while ((n = recv(sock, pbuffer, maxlen, 0)) > 0) {
		pbuffer += n;
		maxlen -= n;
		len += n;

		buffer[len] = '\0';
		printf("%s\n", buffer);
	}

	// close the socket
	close(sock);
	return 0;
}
