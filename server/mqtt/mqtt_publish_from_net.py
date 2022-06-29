# python 3.6
import utils
import time
from paho.mqtt import client as mqtt_client

topic = "97411252/test"

def publish(client):
    msg_count = 0
    while True:
        time.sleep(1)
        msg = f"messages: {msg_count}"
        result = client.publish(topic, msg)
        # result: [0, 1]
        status = result[0]
        if status == 0:
            print(f"Send `{msg}` to topic `{topic}`")
        else:
            print(f"Failed to send message to topic {topic}")
        msg_count += 1


def run():
    client = utils.connect_mqtt()
    client.loop_start()
    publish(client)


if __name__ == '__main__':
    run()
