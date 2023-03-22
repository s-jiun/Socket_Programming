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

	inet_pton(AF_INET, server_name, &server_address.sin_addr);  // Convert IP addr. to binary  �ּ� �йи� 
	// inet_aton and inet_addr => IPv4 Only
	// server_address.sin_addr = Binary IP address

	// htons: port in network order format
	server_address.sin_port = htons(server_port); 
	
	/*htonl()�Լ��� long intger(�Ϲ������� 4byte)�����͸� ��Ʈ��ũ byte order�� �����Ѵ�.
	htons()�Լ��� short intger(�Ϲ������� 2byte)�����͸� ��Ʈ��ũ byte order�� �����Ѵ�.
	ntohl()�Լ��� long intger �����͸� ȣ��Ʈ byte order�� �����Ѵ�.
	ntohs()�Լ��� short intger �����͸� ȣ��Ʈ byte order�� �����Ѵ�.
	*/
	
	/* little endian and big endian
	CPU���� ������ ���� ��� �ٸ�. 
	0A0B0C0D -> 0D 0C 0B 0A or 0A 0B 0C 0D
	
	Network: Big endian! ����.. 
	Intel CPU -> Little endian!
	*/

	// open a stream socket
	int sock;
	if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0) { // IPv4 + TCP .. ���������йи� 
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
