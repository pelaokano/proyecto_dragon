import time
import os
import mysql.connector
import paho.mqtt.client as mqtt

# Parámetros desde variables de entorno o valores por defecto
MYSQL_HOST = os.getenv("MYSQL_HOST", "mysql")
MYSQL_USER = os.getenv("MYSQL_USER", "opcua_user")
MYSQL_PASSWORD = os.getenv("MYSQL_PASSWORD", "opcua_pass")
MYSQL_DB = os.getenv("MYSQL_DB", "opcua_db")

MQTT_HOST = os.getenv("MQTT_HOST", "mqtt_broker")
MQTT_PORT = int(os.getenv("MQTT_PORT", 1883))

def get_capacidad():
    try:
        conn = mysql.connector.connect(
            host=MYSQL_HOST,
            user=MYSQL_USER,
            password=MYSQL_PASSWORD,
            database=MYSQL_DB
        )
        cursor = conn.cursor()
        
        cursor.execute(
            "SELECT capacidad_linea1, capacidad_linea2, capacidad_linea3 "
            "FROM capacidad ORDER BY id DESC LIMIT 1"
        )

        result = cursor.fetchone()
        cursor.close()
        conn.close()

        if result:
            return float(result[0]), float(result[1]), float(result[2])

    except Exception as e:
        print(f"Error al consultar MySQL: {e}")
    return 0.0, 0.0, 0.0

def run():
    client = mqtt.Client()
    client.connect(MQTT_HOST, MQTT_PORT)

    while True:
        capacidad1, capacidad2, capacidad3 = get_capacidad()
        # print(f"Publicando capacidad: {valor} en MQTT")
        client.publish("opcua/capacidad1", str(capacidad1))
        client.publish("opcua/capacidad2", str(capacidad2))
        client.publish("opcua/capacidad3", str(capacidad3))
        time.sleep(2)
