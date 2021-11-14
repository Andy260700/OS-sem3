package com.arkadeepde;

import java.util.LinkedList;
import java.util.Queue;

public class ProducerConsumerChannel <T> {
    private Queue<T> channel;
    final private int capacity;
    private Producer producer;
    private Consumer consumer;

    ProducerConsumerChannel(int capacity){
        this.capacity = capacity;
        channel = new LinkedList<T>();
        producer = new Producer();
        consumer = new Consumer();
    }

    class Producer {
        public void produce(T value) throws InterruptedException
        {
            synchronized (channel) {
                //producer will wait when the channel is full
                while (channel.size() == capacity)
                    channel.wait();

                channel.add(value);

                //Notify Consumer
                channel.notifyAll();
            }
        }
    }

    class Consumer{
        public T consume() throws InterruptedException
        {
            synchronized (channel) {
                T value;
                //consumer thread waits when channel is empty
                while (channel.size() == 0)
                    channel.wait();

                value = channel.remove();

                //Notify Producer
                channel.notifyAll();

                return value;
            }
        }
    }


    public Consumer getConsumer() {
        return consumer;
    }

    public Producer getProducer() {
        return producer;
    }
}
