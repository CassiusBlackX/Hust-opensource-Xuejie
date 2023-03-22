#include "stdafx.h"
#include "Global.h"
#include "StopWaitRdtReceiver.h"

GBNReceiver::GBNReceiver() :seq(1)
{
    lastAckPkt.acknum = 0;
    lastAckPkt.checksum = 0;
    lastAckPkt.seqnum = -1;
    for (int i = 0; i < Configuration::PAYLOAD_SIZE; i++)
        lastAckPkt.payload[i] = '.';
    lastAckPkt.checksum = pUtils->calculateCheckSum(lastAckPkt);
}
GBNReceiver::~GBNReceiver()
{
}
void GBNReceiver::receive(const struct Packet& packet)
{
    int checkSum = pUtils->calculateCheckSum(packet);//�������

    if (checkSum == packet.checksum && this->seq == packet.seqnum)//�յ�����ȷ��ŵı���
    {
        pUtils->printPacket("���շ���ȷ�յ����ͷ��ı���", packet);
        Message msg;
        memcpy(msg.data, packet.payload, sizeof(packet.payload));//�Ͻ��ļ���Ӧ�ò�
        pns->delivertoAppLayer(RECEIVER, msg);//�Ͻ�

        lastAckPkt.acknum = packet.seqnum;//׼������ack
        lastAckPkt.checksum = pUtils->calculateCheckSum(lastAckPkt);//����ackpackage����

        pUtils->printPacket("���շ�����ȷ�ϱ���", lastAckPkt);
        pns->sendToNetworkLayer(SENDER, lastAckPkt);//����ack�����ͷ�

        this->seq++;
    }

    else//������ʹ��󣬻�������
    {
        if (packet.acknum != seq)//�������
            pUtils->printPacket("ERROR�����շ�δ�յ���ȷ���ģ�������Ŵ���", packet);
        else
            pUtils->printPacket("ERROR�����շ�δ�յ���ȷ���ģ�����ʹ���", packet);

        pUtils->printPacket("���ܷ����·����ϴε�ȷ�ϱ���", lastAckPkt);//������һ�ε�ȷ�ϱ���
        pns->sendToNetworkLayer(SENDER, lastAckPkt);//������һ�ε�ȷ�ϱ���

    }
}