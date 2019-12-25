#!/usr/bin/python3

import matplotlib.pyplot as plt
import matplotlib.animation as animation

fig = plt.figure()
ax = fig.add_subplot(1, 1, 1)
xs = []
ys = []
base_ts = None


import hawktracer.client

client = hawktracer.client.Client()
client.start('127.0.0.1:8765')


def read_data(x_data, y_data):
    global base_ts
    while not client.eos():
        event = client.poll_event()
        if not event:
            return

        if event[0] == 'ResourceUsageEvent':
            timestamp = event[1]['timestamp']
            allocation_count = event[1]['alloc_count']
            mem_usage = event[1]['memory_usage']/1024/1024 #

            if not base_ts:
                base_ts = timestamp

            x_data.append((timestamp-base_ts)/(1000*1000*1000))
            y_data.append(mem_usage)
            print(allocation_count)


def animate(i, xs, ys):
    read_data(xs, ys)

    ax.clear()
    ax.plot(xs, ys)
    ax.set_ylim(0, 250)
    ax.set_xlim(0, 25)
    plt.title('Memory usage over time')
    plt.ylabel('Memory usage (MB)')
    plt.xlabel('Timestamp (seconds)')

ani = animation.FuncAnimation(fig, animate, fargs=(xs, ys), interval=20)
plt.show()
