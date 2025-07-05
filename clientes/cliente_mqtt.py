import paho.mqtt.client as mqtt

MQTT_HOST = "localhost"  # o la IP del host si no estás en la misma máquina
MQTT_PORT = 1883
# TOPIC = "opcua/capacidad"
TOPICS = [("opcua/capacidad1", 0), ("opcua/capacidad2", 0), ("opcua/capacidad3", 0)]

def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Conectado al broker MQTT")
        # client.subscribe(TOPIC)
        for topic, qos in TOPICS:
            client.subscribe(topic, qos)
    else:
        print("Fallo al conectar. Código de retorno:", rc)

def on_message(client, userdata, msg):
    print(f"Mensaje recibido en {msg.topic}: {msg.payload.decode()}")

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.connect(MQTT_HOST, MQTT_PORT)
client.loop_forever()
