#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>

#define PORT_NUMBER 60000
int main(int argc, char *argv[])
{
	//DECLARING VARIABLES
	int socket_s, socket_c, c;
	int read_size = 0;
	char buffer[1024];
	time_t current_time;
	struct tm* time_info;
	char timeString[9];
	char dateString[11];
	char zoneString[9];
	char dayString[11];
	struct sockaddr_in addr_s, addr_c;


	//CREATE SOCKET
	socket_s = socket(AF_INET,SOCK_STREAM,0);
	if(socket_s == -1){
		puts("Could Not Create Socket.");
		return 1;
	}
	//BIND SOCKET TO PORT 
	addr_s.sin_family = AF_INET;
	addr_s.sin_addr.s_addr = INADDR_ANY;
	addr_s.sin_port = htons(PORT_NUMBER);
	if(bind(socket_s,(struct sockaddr*)&addr_s, sizeof(addr_s)) < 0){
		puts("Bind Failed.");
		return 1;
	}
	puts ("Socket Binded.");
	//LISTENING CLIENT
	listen(socket_s,1);
	c = sizeof(struct sockaddr_in);
	//ACCEPTING CLIENT
	socket_c = accept(socket_s,(struct sockaddr*)&addr_c, (socklen_t*)&c);
	if (socket_c == -1){
		puts ("Error Accepting Client.");
		return 1;
	}
	else{
		puts("Client Accepted.");
		write(socket_c, "Welcome Client.\n",16);
	}
	//RECEIVES INPUTS FROM CLIENT IN A LOOP UNTIL CLOSE_SERVER EXECUTES
	while(read_size = recv(socket_c, buffer, 1024, 0)>0){
		buffer[strlen(buffer)] = '\0';
		//SEND TIME AND DATE TO CLIENT
		if(strstr(buffer,"GET_TIME_DATE") != NULL){
			time(&current_time);
			time_info = localtime(&current_time);
			strftime(timeString, sizeof(timeString), "%H:%M:%S", time_info);
			strftime(dateString, sizeof(dateString), "%d.%m.%Y", time_info);
			write(socket_c, timeString, strlen(timeString));
			write(socket_c,",",1);
			write(socket_c, dateString, strlen(dateString));
			write(socket_c,"\n",1);
			}
		//SEND TIME ZONE TO CLIENT
		else if(strstr(buffer,"GET_TIME_ZONE") != NULL){
			time(&current_time);
			time_info = localtime(&current_time);
			strftime(zoneString, sizeof(zoneString), "%Z", time_info);
			write(socket_c, zoneString, strlen(zoneString));
			write(socket_c,"\n",1);
			}
		//SEND TIME TO CLIENT
		else if(strstr(buffer,"GET_TIME") != NULL){
			time(&current_time);
			time_info = localtime(&current_time);
			strftime(timeString, sizeof(timeString), "%H:%M:%S", time_info);
			write(socket_c, timeString, strlen(timeString));
			write(socket_c,"\n",1);
			}
		//SEND DATE TO CLIENT
		else if(strstr(buffer,"GET_DATE") != NULL){
			time(&current_time);
			time_info = localtime(&current_time);
			strftime(dateString, sizeof(dateString), "%d.%m.%Y", time_info);
			write(socket_c, dateString, strlen(dateString));
			write(socket_c,"\n",1);
			}
		//SEND DAY OF THE WEEK TO CLIENT
		else if(strstr(buffer,"GET_DAY_OF_WEEK") != NULL){
			time(&current_time);
			time_info = localtime(&current_time);
			strftime(dayString, sizeof(dayString), "%A", time_info);
			write(socket_c, dayString, strlen(dayString));
			write(socket_c,"\n",1);
			}
		//CLOSE THE SERVER AND CLIENT
		else if(strstr(buffer,"CLOSE_SERVER") != NULL){
			close(socket_c);
			close(socket_s);
			}
		//INCORRECT INPUT CHECK
		else{
			write(socket_c,"INCORRECT REQUEST",18);
			write(socket_c,"\n",1);
			
		}
		memset(buffer, '\0', sizeof(buffer));
		memset(buffer, 0 ,1024);
	}
	
	return 0;
}
