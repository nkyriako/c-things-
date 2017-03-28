//arg[1] must be a port number


//while the connection is still there, 
	//string x  = accept new line 
	//use x as input for main
	/* A simple server in the internet domain using TCP
	The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <boost/algorithm/string.hpp>
#include "KB.h"
#include "RB.h"
#include "Rule.h"
#include "Fact.h" 
#include "Query.h"


void error(const char *msg)
{
	perror(msg);
	exit(1);
}

vector<string> split(string str, char delimiter) {
  vector<string> internal1;
  stringstream ss(str); // Turn the string into a stream.
  string tok;
  
  while(getline(ss, tok, delimiter)) {
    internal1.push_back(tok);
  }
  
  return internal1;
}

int main(int argc, char *argv[])
{

    
    KB *kb = new KB();
    RB *rb = new RB();
    
    bool takingInput = true;

	int sockfd, newsockfd, portno;
	socklen_t clilen;
	char buffer[1024];
	struct sockaddr_in serv_addr, cli_addr;
	int n;
	if (argc < 2) {
		fprintf(stderr, "ERROR, no port provided\n");
		exit(1);
	}
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		error("ERROR opening socket");
	bzero((char *)&serv_addr, sizeof(serv_addr));
	portno = atoi(argv[1]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	if (bind(sockfd, (struct sockaddr *) &serv_addr,
		sizeof(serv_addr)) < 0)
		error("ERROR on binding");
	listen(sockfd, 5);
	clilen = sizeof(cli_addr);
	newsockfd = accept(sockfd,
		(struct sockaddr *) &cli_addr,
		&clilen);
	if (newsockfd < 0)
		error("ERROR on accept");
	while(true) {
	std::string str ;
   	 std::string line;
   	 std::string word;
    
	bzero(buffer, 1024);
	n = read(newsockfd, buffer, 1023);
	if (n < 0) error("ERROR reading from socket");
	printf("Here is the message: %s\n", buffer);

  //      std::getline(std::cin, line);
           for (int i = 0; i < strlen(buffer); i++) {
		line+=buffer[i];
		}
        std::istringstream iss(line);
        
        //Look at the first word 
        if(iss >> word){

		std::cout << "line: " << line <<std::endl;
		std::cout << "word: "<<word << std::endl;
               removeSpace(word);
            //pass substring of 'line' without the beginning word
   
            if (word=="RULE"){
                    //new rule
                    rb->add(new Rule(removeFirst(line))); //new
			write(newsockfd, "Rule added!", strlen("Rule added!"));
            } else if (word=="FACT"){
                    // new fact 
			std::cout << "wat" <<std::endl;
                    kb->add(new Fact(removeFirst(line))); //new
			write(newsockfd, "Fact added!", strlen("Fact added!"));
            } else if (word=="INFERENCE"){
                    //make a query
		
                    vector<Fact*> facts = inquire(removeFirst(line), rb, kb);
		    for(int i = 0; i< facts.size(); i++)
		    {
			//str = str + facts.at(i)-
			str = str + facts.at(i)->param.at(0) + ",";
			str = str + facts.at(i)->param.at(1) + '\n';
		    }
		write(newsockfd, str.c_str(), strlen(str.c_str()));
		   
		   
            } else if (word=="DROP") {
                    //drop fact or rule 
                    DROP(removeFirst(line), kb, rb);
            } else if (word == "LOAD") {
                   // LOAD(removeFirst(line), kb, rb);
			//std::cout << "Not loading currently";
		removeFirst(line);
		std::vector<std::string> x = split(line, '\n');
		x.at(0) = x.at(0).substr(1,x.at(0).size()-1);
		for(int i = 0; i < x.size() ; i++)
		{
			std::stringstream iss(x.at(i)); 
			if (iss>>word) {
           
            		if (word=="RULE") {
                		//not sure if this works
                	rb->add( new Rule(removeFirst(line))); //New?
            		} else if (word == "FACT") {
                	kb->add(new Fact(removeFirst(line)));
            		}
        		}
		}


            } else if(word == "DUMP") {
                   // DUMP(removeFirst(line), kb, rb);
			std::cout << "Not dumping currently";
            } else if (word =="EXIT") {
		//write(newsockfd, "EXIT", strlen("EXIT"));
                //takingInput=false;
		break;
            } else {
             std::cout <<"Not a valid command." 
             <<"Valid commands are LOAD, DROP, INFERENCE, FACT, RULE, DUMP ";
            }
            //else the argument is invalid 
        } else {
            std::cout <<"Not a valid command." 
             <<"Valid commands are LOAD, DROP, INFERENCE, FACT, RULE, DUMP " ;
        }
        std::cout << "\n";

	// input str(buffer)
	// output to "i got your message" place 
	//n = write(newsockfd, "I got your message", 18);
	//if (n < 0) error("ERROR writing to socket");

	}//endwhile 

	close(newsockfd);
	close(sockfd);
    	std::cout << "Exiting Program..." << std::endl;
	delete rb;
	delete kb;
	return 0;
}
