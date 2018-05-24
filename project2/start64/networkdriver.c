#include <pthread.h>

/*User defined headers*/
#include "BoundedBuffer.h"
#include "destination.h"
#include "diagnostics.h"
#include "fakeapplications.h"
#include "freepacketdescriptorstore__full.h"
#include "freepacketdescriptorstore.h"
#include "networkdevice__full.h"
#include "networkdevice.h"
#include "networkdriver.h"
#include "packetdescriptor.h"
#include "pid.h"
#include "queue.h"

/*size used for max incomming buffer*/
#define MAX_IN 5
#define UNUSED __attribute__((unused))
#define MAX_SEND 3
/*send and recv functions for pthreads*/
static void* driver_send(void* arg);
static void* driver_recv(void* arg);    

static BoundedBuffer* out_buff;
static BoundedBuffer* in_buff[MAX_PID+1];
static BoundedBuffer* free_buff;

FreePacketDescriptorStore* FPDS;
NetworkDevice* netd;

void init_network_driver(NetworkDevice *nd, void *mem_start,unsigned long mem_length,FreePacketDescriptorStore **fpds_ptr){
	//*create network device*/
	netd = nd;
	/*create free packet descriptor store, and set value to pointer*/
	FPDS= FreePacketDescriptorStore_create(mem_start, mem_length);
	*fpds_ptr =  FPDS;
	/*create recv buffer per pid*/
	int i;
	for(i = 0; i < MAX_PID+1 ; i++)
		in_buff[i] = BoundedBuffer_create(MAX_IN);

	/*outgoing CHECK SIZE*/
	out_buff = BoundedBuffer_create(MAX_PID);
	free_buff = BoundedBuffer_create(MAX_PID+1);

	/*thread for send/recv*/
	pthread_t send,recv;
	/*change to test the creation of pthreads*/
	if (pthread_create(&send, NULL, &driver_send, NULL)){
		DIAGNOSTICS("[DRIVER> ERROR: creating send thread failed\n");
	}
	if(pthread_create(&recv, NULL, &driver_recv, NULL)){
		DIAGNOSTICS("[DRIVER> ERROR: creating recv thread failed\n");
	}

}
/***
*Send Functions
*
***/

/* if you are able to queue up packet descriptor immediately, do so and return 1 */
/* otherwise, return 0 */
int nonblocking_send_packet(PacketDescriptor *pd) {
	return out_buff->nonblockingWrite(out_buff, pd);
}

/* queue up packet descriptor for sending */
/* do not return until it has been successfully queued */
void blocking_send_packet(PacketDescriptor *pd){
	out_buff->blockingWrite(out_buff,pd);
}


int send_non_blocking_helper(PacketDescriptor* pd){
	if(!(free_buff->nonblockingWrite(free_buff,pd))){
		if(!(FPDS->nonblockingPut(FPDS,pd))){
			return 0;
		}
	}
	return 1;
}


/*
For sending packets to the
network, you can pass PacketDescriptor s to the NetworkDevice for dispatch. The complication
is that the NetworkDevice can only process one packet at a time
*/

/*send fuction used by sender thread*/
static void* driver_send(UNUSED void* arg){

	PacketDescriptor* pd;
	int i;
	int status = 0;

	for(;;){
		/*blocking get pd from buffer*/
		out_buff->blockingRead(out_buff, (void**)&pd);
		/*attempt to send until it gets it or reach max tries*/
		for(i=0; i < MAX_SEND; i++){
					/*send from network device*/
			if((status = netd->sendPacket(netd,pd))){
				break;
			}
		}
		/*print success/failure*/
		if(status){
			DIAGNOSTICS("[DRIVER> Info: Sent a packet after %d tries\n", i);
		} else { 
			DIAGNOSTICS("[DRIVER> Info: Failure sending, attempted %d times\n", MAX_SEND);
		}

		/*following logic provided in lab*/
		if(!(free_buff->nonblockingWrite(free_buff,pd)))
			if(!(FPDS->nonblockingPut(FPDS,pd)))
				return;
		
	}
	return NULL;
}


/***
*Recv Functions
*
***/

int recv_non_blocking_helper(PacketDescriptor** pd){
	if(!(free_buff->nonblockingRead(free_buff, (void**)pd))){
		if(!(FPDS->nonblockingGet(FPDS,pd))){
			return 0;
		}
	}
	return 1;
}



void* driver_recv(UNUSED void* arg){

	PacketDescriptor* pd;
	PacketDescriptor* temp;
	PID pid;

	/*Blocking FPDS_GET*/
	FPDS->blockingGet(FPDS, &pd);
	/*INIT PD*/
	initPD(pd);
	/*register pd*/
	netd->registerPD(netd, pd);
	for(;;){
		/*maybe move init and register in for loop*/
		/*await_packe t()from network device*/
		netd->awaitIncomingPacket(netd);
		/*keep backup*/
		temp = pd;

		/*if nonblockgetpd(pool,fd,temp)*/
		if(recv_non_blocking_helper(&pd)){
			initPD(pd);
			netd->registerPD(netd,pd);
			pid = getPID(temp);
			/*if recv[pid]->NonBlockingWrite ==0*/
			if(!(in_buff[pid]->nonblockingWrite(in_buff[pid],temp))){
				send_non_blocking_helper(temp);
				/*NEED TO WRITE FUNCTION*/
			}
		} else {
			/*pd = temp*/
			pd = temp;
			/*initpd*/
			initPD(pd);
			/*register(pd,device) on network device?*/
			netd->registerPD(netd,pd);
		}	
	}
	return NULL;
}



/* if there is currently a waiting packet for `pid’, return that packet */
/* to the calling application and return 1 for the value of the function */
/* otherwise, return 0 for the value of the function */
int nonblocking_get_packet(PacketDescriptor **pd, PID pid) {
	
	/*read from the incomming bb, for specified pid*/
	return in_buff[pid]->nonblockingRead(in_buff[pid], (void**)pd);
}


/* wait until there is a packet for `pid’ */
/* return that packet descriptor to the calling application */
void blocking_get_packet(PacketDescriptor **pd, PID pid) {
			/*take buffer of current item*/   
  in_buff[pid]->blockingRead(in_buff[pid], (void**)pd);
}