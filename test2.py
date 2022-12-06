from paho.mqtt import client as mqtt_client
import random
import json

broker = '192.168.100.7'
port = 1883
topic = "ANGX"
topics = ["EMG1", "EMG2", "ANGX", "ANGY", "ANGZ", "ANGX2", "ANGY2", "ANGZ2"]

client_id = f'python-mqtt-{random.randint(0, 1000)}'
# username = 'emqx'
# password = 'public'


def connect_mqtt():
    def on_connect(client, userdata, flags, rc):
        if rc == 0:
            print("Connected to MQTT Broker!")
        else:
            print("Failed to connect, return code %d\n", rc)
    # Set Connecting Client ID
    client = mqtt_client.Client(client_id)
    # client.username_pw_set(username, password)
    client.on_connect = on_connect
    client.connect(broker, port)
    return client


def subscribe(client: mqtt_client):
    def on_message(client, userdata, message):
        d = json.loads(message.payload.decode("utf-8"))
        with open("realtime.txt", "a+") as file:
            file.write("%f\n" %d)
    for topic in topics:
        client.subscribe(topic)
        client.on_message = on_message

def run():
    client = connect_mqtt()

    subscribe(client)
    client.loop_forever()


if __name__ == '__main__':
    run()

