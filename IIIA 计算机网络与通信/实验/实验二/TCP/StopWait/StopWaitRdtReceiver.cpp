#include "stdafx.h"
#include "Global.h"
#include "StopWaitRdtReceiver.h"

#include "string.h"
#include "Global.h"


TCPReceiver::TCPReceiver() :seq(1)
{
    lastAckPkt.acknum = 0;
    lastAckPkt.checksum = 0;
    lastAckPkt.seqnum = -1;
    for (int i = 0; i < Configuration::PAYLOAD_SIZE; i++)
        lastAckPkt.payload[i] = '.';
    lastAckPkt.checksum = pUtils->calculateCheckSum(lastAckPkt);
}


TCPReceiver::~TCPReceiver() {}


void TCPReceiver::receive(const struct Packet& packet)//�ۼ�ȷ��
{
    int checkSum = pUtils->calculateCheckSum(packet);//��鷢�ͷ����͵ı����Ƿ����
    if (checkSum == packet.checksum && this->seq == packet.seqnum) //�����ȷ����Ϊ�ڴ����ܵı���
    {
        pUtils->printPacket("���շ���ȷ�յ�����", packet);
        Message msg;
        memcpy(msg.data, packet.payload, sizeof(packet.payload));
        pns->delivertoAppLayer(RECEIVER, msg);//���ϵݽ���Ӧ�ò�

        this->lastAckPkt.acknum = this->seq + 1;//���͵�ack�����ڴ������к�
        this->lastAckPkt.checksum = pUtils->calculateCheckSum(lastAckPkt);//���¼���У���
        pUtils->printPacket("���շ����ͷ�����ȷ�ϱ���", lastAckPkt);
        pns->sendToNetworkLayer(SENDER, lastAckPkt);//����ack�������
        this->seq++;//�ڴ�����һ���������+1
    }

    else//���ʧ����߳���
    {
        if (packet.acknum != seq)
            pUtils->printPacket("ERROR�����շ�δ��ȷ�յ����ģ��������кŴ���", packet);

        else
            pUtils->printPacket("ERROR�����շ�δ��ȷ�յ����ģ�����ʹ���", packet);

        pUtils->printPacket("ERROR�����շ���������ACK", this->lastAckPkt);//lastpacket�е�acknumΪseq��ֵ�����ڴ����ܵ������кš�
        pns->sendToNetworkLayer(SENDER, lastAckPkt);
    }
}