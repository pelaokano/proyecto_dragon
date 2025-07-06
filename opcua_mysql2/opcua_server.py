from opcua import ua, Server
import mysql.connector
import time
import os

# Configuración de conexión MySQL desde variables de entorno o valores por defecto
MYSQL_HOST = os.getenv("MYSQL_HOST", "mysql")
MYSQL_USER = os.getenv("MYSQL_USER", "opcua_user")
MYSQL_PASSWORD = os.getenv("MYSQL_PASSWORD", "opcua_pass")
MYSQL_DB = os.getenv("MYSQL_DB", "opcua_db")

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
    server = Server()
    server.set_endpoint("opc.tcp://0.0.0.0:4840/freeopcua/server/")
    uri = "http://examples.freeopcua.github.io"
    idx = server.register_namespace(uri)
    objects = server.get_objects_node()
    
    # capacidad_var = objects.add_variable(idx, "capacidad", 0.0)
    # capacidad_var.set_writable()

    capacidad1_var = objects.add_variable(idx, "capacidad_linea1", 0.0)
    capacidad1_var.set_writable()
    capacidad2_var = objects.add_variable(idx, "capacidad_linea2", 0.0)
    capacidad2_var.set_writable()
    capacidad3_var = objects.add_variable(idx, "capacidad_linea3", 0.0)
    capacidad3_var.set_writable()

    server.start()
    print("Servidor OPC UA iniciado")

    try:
        while True:
            capacidad1, capacidad2, capacidad3 = get_capacidad()
            # capacidad_var.set_value(valor)
            capacidad1_var.set_value(capacidad1)
            capacidad2_var.set_value(capacidad2)
            capacidad3_var.set_value(capacidad3)            
            # print(f"Variable 'capacidad' actualizada a: {valor}")
            time.sleep(2)
    finally:
        server.stop()