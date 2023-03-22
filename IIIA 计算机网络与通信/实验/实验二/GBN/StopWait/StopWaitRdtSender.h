#ifndef STOP_WAIT_RDT_SENDER_H
#define STOP_WAIT_RDT_SENDER_H
#include "RdtSender.h"

#define len 8
class GBNSender :public RdtSender
{
private:
    int base;//���ʹ��ڵ�base
    int nextseqnum;//���ʹ��ڵ�nextseqnum
    bool waitingState;//�Ƿ��ڵȴ�ACK״̬
    Packet win[len];//���ʹ��ڣ����ڴ�СΪ4
    int num_packet_win;//��¼�����еı��ĸ���
public:
    bool getWaitingState();
    bool send(const Message& message);//����Ӧ�ò�������Message����NetworkServiceSimulator����
    void receive(const Packet& ackPkt);//����ACK������
    void timeoutHandler(int seqNum);
    void Init();
    void printSlideWindow();

public:
    GBNSender();
    virtual ~GBNSender();
};
#endif //STOPWAIT_GBNSENDER_H


