from mqtt_subscribe import subscribe
from utils import connect_mqtt

def run():
    client = connect_mqtt()
    subscribe(client)
    client.loop_forever()

if __name__ == '__main__':
    run()