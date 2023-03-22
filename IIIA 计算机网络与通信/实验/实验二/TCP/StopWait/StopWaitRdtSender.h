#include "RdtSender.h"
#ifndef TCP_TCPSENDER_H
#define TCP_TCPSENDER_H
#define len 8
class TCPSender :public RdtSender
{
private:
    int base;//���ʹ��ڵ�base
    int nextseqnum;//���ʹ��ڵ�nextseqnum
    bool waitingState;//�Ƿ��ڵȴ�ACK״̬
    Packet win[len];//���ʹ��ڣ����ڴ�СΪ4
    int num_pac_win;//��¼�����еı��ĸ���
    int count;//��¼����ACK��Ŀ
    int current_rcv_ack;//�����յ���ACK���
    int last_rcv_ack;//��һ���յ���ACK���
public:
    bool getWaitingState();
    bool send(const Message& message);//����Ӧ�ò�������Message����NetworkServiceSimulator����
    void receive(const Packet& ackPkt);//����ACK������
    void timeoutHandler(int seqNum);
    void PrintSildeWindows();
public:
    TCPSender();
    virtual ~TCPSender();
};
#endif //TCP_TCPSENDER_H
