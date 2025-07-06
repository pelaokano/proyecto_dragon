import time
import os
import mysql.connector
from pyModbusTCP.server import ModbusServer
from pyModbusTCP.utils import encode_ieee, long_list_to_word

# Configuración de conexión MySQL desde variables de entorno o valores por defecto
MYSQL_HOST = os.getenv("MYSQL_HOST", "mysql")
MYSQL_USER = os.getenv("MYSQL_USER", "opcua_user")
MYSQL_PASSWORD = os.getenv("MYSQL_PASSWORD", "opcua_pass")
MYSQL_DB = os.getenv("MYSQL_DB", "opcua_db")

# Dirección donde se escribirá en los registros Modbus
# ADDRESS_WRITE_SERVER = 200
ADDRESS_WRITE_BASE = 200  # Dirección para capacidad_linea1

# Configuración del servidor Modbus
SERVER_HOST = "0.0.0.0"
SERVER_PORT = 502  # evitar el 502 si no corres como root

def get_capacidad():
    try:
        conn = mysql.connector.connect(
            host=MYSQL_HOST,
            user=MYSQL_USER,
            password=MYSQL_PASSWORD,
            database=MYSQL_DB
        )
        cursor = conn.cursor()
        # cursor.execute("SELECT capacidad FROM capacidad ORDER BY id DESC LIMIT 1")
        
        cursor.execute(
            "SELECT capacidad_linea1, capacidad_linea2, capacidad_linea3 "
            "FROM capacidad ORDER BY id DESC LIMIT 1"
        )

        result = cursor.fetchone()
        cursor.close()
        conn.close()

        if result:
            # return float(result[0])
            return float(result[0]), float(result[1]), float(result[2])
    
    except Exception as e:
        print(f"Error al consultar MySQL: {e}")
    return 0.0, 0.0, 0.0

def run_server():
    server = ModbusServer(SERVER_HOST, SERVER_PORT, no_block=True)

    try:
        print("Iniciando servidor Modbus...")
        server.start()
        while True:
            # capacidad = get_capacidad()
            capacidad1, capacidad2, capacidad3 = get_capacidad()
            # encoded = encode_ieee(capacidad)
            encoded1 = encode_ieee(capacidad1)
            encoded2 = encode_ieee(capacidad2)
            encoded3 = encode_ieee(capacidad3)

            # word_list = long_list_to_word([encoded])
            
            words1 = long_list_to_word([encoded1])
            words2 = long_list_to_word([encoded2])
            words3 = long_list_to_word([encoded3])

            ok1 = server.data_bank.set_holding_registers(ADDRESS_WRITE_BASE, words1)
            ok2 = server.data_bank.set_holding_registers(ADDRESS_WRITE_BASE + 2, words2)
            ok3 = server.data_bank.set_holding_registers(ADDRESS_WRITE_BASE + 4, words3)
                    
            # ok = server.data_bank.set_holding_registers(ADDRESS_WRITE_SERVER, word_list)
            # if ok:
                # print(f"[✓] Valor de capacidad ({capacidad}) escrito en dirección {ADDRESS_WRITE_SERVER}")

            if ok1 and ok2 and ok3:
                print(f"[✓] Capacidades escritas en Modbus: {capacidad1}, {capacidad2}, {capacidad3}")
            else:
                print(f"[✗] Falló escritura Modbus")
            time.sleep(2)
    
    except Exception as e:
        print("Apagando servidor Modbus...")
        print(e)
    
    finally:
        server.stop()
        print("Servidor Modbus detenido.")

if __name__ == "__main__":
    run_server()
