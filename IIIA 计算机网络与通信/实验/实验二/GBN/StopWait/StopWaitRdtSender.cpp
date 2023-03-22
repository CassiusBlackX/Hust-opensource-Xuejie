#include "stdafx.h"
#include "Global.h"
#include "StopWaitRdtSender.h"



GBNSender::GBNSender() :base(1), nextseqnum(1), waitingState(false), num_packet_win(0)
{
}
GBNSender::~GBNSender()
{
}
bool GBNSender::getWaitingState()
{
    return waitingState;
}
bool GBNSender::send(const struct Message& message)
{
    if (nextseqnum < base + len)//�����Լ������ͱ���
    {
        this->waitingState = false;//��Ǵ�ʱ����δ��
        this->win[num_packet_win].acknum = -1;//ack��Ϊ-1
        this->win[num_packet_win].seqnum = this->nextseqnum;//ָ����һ������
        this->win[num_packet_win].checksum = 0;//������0
        memcpy(this->win[num_packet_win].payload, message.data, sizeof(message.data));//��������

        this->win[num_packet_win].checksum = pUtils->calculateCheckSum(this->win[num_packet_win]);//�������
        pUtils->printPacket("���ͷ����ͱ���", this->win[num_packet_win]);
        if (base == nextseqnum)//��Ϊ������Ԫ����򿪼�ʱ��
            pns->startTimer(SENDER, Configuration::TIME_OUT, this->win[num_packet_win].seqnum);

        pns->sendToNetworkLayer(RECEIVER, this->win[num_packet_win]);//���ͱ����������
        this->num_packet_win++;//������packet��Ŀ+1

        if (num_packet_win > len)//�жϴ����Ƿ���
            this->waitingState = true;//�������ˣ�

        this->nextseqnum++;//������һ����
        return true;
    }
    else
    {
        this->waitingState = true;
        return false;
    }
}
void GBNSender::receive(const struct Packet& ack)//����ȷ��ack
{
    if (this->num_packet_win > 0)//���ڱ���������0
    {
        int checkSum = pUtils->calculateCheckSum(ack);//�������

        if (checkSum == ack.checksum && ack.acknum >= this->base)//�ۼ�ȷ��
        {
            int num = ack.acknum - this->base + 1;//��¼���յ���ack�������base=1�� ack1��ʧ�����յ���ack2��Ҳ������ack1�յ���

            base = ack.acknum + 1;//��������base
            pUtils->printPacket("���ͷ��յ���ȷȷ��", ack);

            if (this->base == this->nextseqnum)
                pns->stopTimer(SENDER, this->win[0].seqnum);//������ֹͣ
            else//��Ȼ��������ʱ��
            {
                pns->stopTimer(SENDER, this->win[0].seqnum);
                pns->startTimer(SENDER, Configuration::TIME_OUT, this->win[num].seqnum);
            }

            for (int i = 0; i < num_packet_win - num; i++)
            {
                win[i] = win[i + num];//�������ڵ�packet��ǰ�ƶ�numλ
                printf("The current windows's %d number is %d\n", i, win[i].seqnum);
            }


            this->num_packet_win = this->num_packet_win - num;//������ǰ�ƶ�num���������ڵ�packet��Ŀ��ȥnum
        }
    }
}
void GBNSender::timeoutHandler(int seqNum)
{
    pUtils->printPacket("���ͷ���ʱ��ʱ�䵽���ط���һ�η��͵ı���", this->win[0]);
    pns->stopTimer(SENDER, this->win[0].seqnum);
    pns->startTimer(SENDER, Configuration::TIME_OUT, this->win[0].seqnum);
    for (int i = 0; i < num_packet_win; i++)
    {
        pns->sendToNetworkLayer(RECEIVER, this->win[i]);
    }

}


