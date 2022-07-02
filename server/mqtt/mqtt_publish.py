# python 3.6
import utils
import time
from paho.mqtt import client as mqtt_client

topic = "97411252/result"

def publish(client,msg):      
    time.sleep(.5)        
    result = client.publish(topic, msg)
    status = result[0]
    if status == 0:
        print(f"Send `{msg}` to topic `{topic}`")
    else:
        print(f"Failed to send message to topic {topic}")


def run():
    client = utils.connect_mqtt()
    client.loop_start()
    publish(client,"hello")


if __name__ == '__main__':
    run()
