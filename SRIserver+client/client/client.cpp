#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string>
#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sstream>

void error(const char *msg)
{
	perror(msg);
	exit(0);
}

int main(int argc, char *argv[])
{
	int sockfd, portno, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;

	char buffer[256];
	//char* buffer = new char[1024];
	if (argc < 3) {
		fprintf(stderr, "usage %s hostname port\n", argv[0]);
		exit(0);
	}
	portno = atoi(argv[2]);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		error("ERROR opening socket");
	server = gethostbyname(argv[1]);
	if (server == NULL) {
		fprintf(stderr, "ERROR, no such host\n");
		exit(0);
	}
	bzero((char *)&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr,
		(char *)&serv_addr.sin_addr.s_addr,
		server->h_length);
	serv_addr.sin_port = htons(portno);
	if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		error("ERROR connecting");
	std::string line;
	std::string word;
	bool takingInput = true;
	while (takingInput) {
		printf("Enter Commands here: ");
		bzero(buffer, 256);
	//	fgets(buffer, 255, stdin);
		std::getline(std::cin, line);
		std::istringstream iss(line);

		if (iss >> word) {
			//buffer = line
			for (int i = 0; i < line.length(); i++) {
				buffer[i] = line[i];
			}
			if (word == "RULE") {
				//new rule
				//rb->add(new Rule(removeFirst(line))); //new
				write(sockfd, buffer, strlen(buffer));
			}
			else if (word == "FACT") {
				// new fact 
				//kb->add(new Fact(removeFirst(line))); //new
				write(sockfd, buffer, strlen(buffer));
			}
			else if (word == "INFERENCE") {
				//make a query
				//INFERENCE(removeFirst(line), rb, kb);
				write(sockfd, buffer, strlen(buffer));
				bzero(buffer, 256);
				n = read(sockfd, buffer, 255);
				printf("%s\n", buffer);
			}
			else if (word == "DROP") {
				//drop fact or rule 
				//DROP(removeFirst(line), kb, rb);
				write(sockfd, buffer, strlen(buffer));
			}
			else if (word == "LOAD") {
				// LOAD(removeFirst(line), kb, rb);
				std::cout << "Not loading currently";
			}
			else if (word == "DUMP") {
				// DUMP(removeFirst(line), kb, rb);
				std::cout << "Not dumping currently";
			}
			else if (word == "EXIT") {
				break;
				//takingInput = false;
			}
			else {
				std::cout << "Not a valid command."
					<< "Valid commands are LOAD, DROP, INFERENCE, FACT, RULE, DUMP ";
			}
			//else the argument is invalid 
		}
		else {
			std::cout << "Not a valid command!"
				<< "Valid commands are LOAD, DROP, INFERENCE, FACT, RULE, DUMP ";
		}
		std::cout << "\n";
		//
		//if (n < 0)
		//	error("ERROR writing to socket");
		//bzero(buffer, 256);
		//n = read(sockfd, buffer, 255);
		//if (n < 0)
		//	error("ERROR reading from socket");
		//printf("%s\n", buffer);
	}

	close(sockfd);
	//delete[] buffer;
	return 0;
}
