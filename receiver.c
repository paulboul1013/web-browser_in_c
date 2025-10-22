#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>  
#include <stdlib.h>     
#include <string.h>    
#include <unistd.h>    
#include <sys/types.h>
#include <pthread.h>
#include <fcntl.h>
#include <time.h>

#define ECHOMAX 1004 //longest strig to echo
#define BUF_SIZE 1000
#define FRAGS 10000


typedef struct {
    int fragment[10];
    char buf[10][BUF_SIZE];
    int empty[10];
} Buffer;

Buffer b;

int sock; //Socket
struct sockaddr_in echo_serv_addr; //Local address
struct sockaddr_in echo_client_addr; //Client  address
unsigned int cli_addr_len; //length of incomming message
char echoBuffer[ECHOMAX]; //Buffer for echo string
unsigned short echo_serv_port; //server port
int recv_msg_size; //size of received message



pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;

//External error handling function
void Die_With_Error(const char *errorMessage){
    perror(errorMessage);
    exit(1);
};

void init_socket(){
    
    echo_serv_port=2000;

    //create socket for sending/receiving datagrams
    if ((sock=socket(PF_INET,SOCK_DGRAM,IPPROTO_UDP))<0){
        Die_With_Error("socket() failed");
    }

    //Construct local address structure
    memset(&echo_serv_addr,0,sizeof(echo_serv_addr)); //Zero out structure
    echo_serv_addr.sin_family = AF_INET; //IPV4
    echo_serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);  //Any incoming interface
    echo_serv_addr.sin_port=htons(echo_serv_port); //Local port

    //Bind to the local address
    if (bind(sock,(struct sockaddr *)&echo_serv_addr,sizeof(echo_serv_addr))<0){
        Die_With_Error("bind() failed");
    }


}

void *th3(void *ptr);
void *th4(void *ptr);

void assemble_file_from_packets(){
    char *x;
    pthread_t thread3,thread4;
    int iret3,iret4;

    iret3=pthread_create(&thread3,NULL,th3,(void*)x);
    iret4=pthread_create(&thread4,NULL,th4,(void*)x);

    pthread_join(thread3,NULL);
    pthread_join(thread4,NULL);

    exit(0);
}


int main(int argc,char *argv[]){
    int i;
    for(i=0;i<10;i++){
        b.fragment[i] = -1;
        b.empty[i]=0;
    }

    init_socket();
    assemble_file_from_packets();
    close(sock);

    return 0;

}

//專門接收並存入緩衝區
void *th3(void *ptr){
    int cont1=1;
    int i,fragnum,x,try;
    char pkt[BUF_SIZE];
    int success=1;
    int num_of_pkts=0;


    while (cont1){
        
        try=0;

        if (success){
            cli_addr_len=sizeof(echo_client_addr);

            //Block until receive a message fomr a client
            if ((recv_msg_size=recvfrom(sock,echoBuffer,ECHOMAX,0,
                (struct sockaddr *)&echo_client_addr,&cli_addr_len))<0){
                Die_With_Error("recvfrom() failed");
            }

            fragnum=0;
            fragnum+=(echoBuffer[0]&&0xFF);
            fragnum+=(echoBuffer[1]&&0xFF)<<8;
            fragnum+=(echoBuffer[2]&&0xFF)<<16;
            fragnum+=(echoBuffer[3]&&0xFF)<<24;

            printf("%d\n",fragnum);
            for(i=0;i<1000;i++){
                pkt[i]=echoBuffer[i+4];
            }            

            num_of_pkts++;


        }

        pthread_mutex_lock(&mutex);

        x=0;
        while ((b.empty[x]!=0) && x<10) {
            x++;
        }

        if (b.empty[x]==0){
            success=1;
            b.empty[x]=1;
            b.fragment[x]=fragnum;
            for(i=0;i<1000;i++){
                b.buf[x][i]=pkt[i];
            }
        }
        else{
            success=0;
        }

        if ((success==0) && (try < 1)){
            try++;
            pthread_mutex_unlock(&mutex);
            usleep(5000); //5ms
        }
        else if ((success==0) && (try==1)){
            success=1;
        }

        pthread_mutex_unlock(&mutex);

        // if has received all packets
        if ((num_of_pkts==FRAGS-1) && (success==1)){
            cont1=0;
        }

    }
}

void *th4(void *ptr){
    return;
}