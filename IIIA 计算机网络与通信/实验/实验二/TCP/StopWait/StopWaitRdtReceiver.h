#include "RdtReceiver.h"
#ifndef TCP_TCPRECEIVER_H
#define TCP_TCPRECEIVER_H
class TCPReceiver :public RdtReceiver
{
private:
    int seq; //�ڴ��յ�����һ���������
    Packet lastAckPkt;            //�ϴη��͵�ȷ�ϱ���
public:
    TCPReceiver();
    virtual ~TCPReceiver();

public:
    void receive(const Packet& packet); //���ܱ��ģ���NetworkService����
};
#endif
