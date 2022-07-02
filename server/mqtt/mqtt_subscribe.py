# python3.6
import random
import utils
from decide import decide
from mqtt_publish import publish

from paho.mqtt import client as mqtt_client

topic = "97411252/scan"

def subscribe(client: mqtt_client):
    def on_message(client, userdata, msg):
        print(f"Received `{msg.payload}`")
        res = decide(msg.payload)        
        publish(client, res)

    client.subscribe(topic)
    client.on_message = on_message


def run():
    client = utils.connect_mqtt()
    subscribe(client)
    client.loop_forever()


if __name__ == '__main__':
    run()
