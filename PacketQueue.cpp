//
// Created by adam on 28/11/2019.
//

#include <mutex>
#include "PacketQueue.h"

void PacketQueue::Enqueue(std::unique_ptr<pcpp::Packet> packet) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_packets.push(std::move(packet));
    m_cv.notify_one();
}

std::unique_ptr<pcpp::Packet> PacketQueue::Dequeue() {
    std::unique_lock<std::mutex> cvLock(m_mutex);

    while(m_packets.empty()){
        m_cv.wait(cvLock);
    }

    auto outPacket = std::move(m_packets.front());
    m_packets.pop();

    return outPacket;
}