#ifndef STOP_WAIT_RDT_RECEIVER_H
#define STOP_WAIT_RDT_RECEIVER_H


#include "RdtReceiver.h"
class GBNReceiver :public RdtReceiver
{
private:
    int seq; //�ڴ��յ�����һ���������
    Packet lastAckPkt;            //�ϴη��͵�ȷ�ϱ���
public:
    GBNReceiver();
    virtual ~GBNReceiver();

public:
    void receive(const Packet& packet); //���ܱ��ģ���NetworkService����
};
#endif //STOPWAIT_GBNRECEIVER_H

