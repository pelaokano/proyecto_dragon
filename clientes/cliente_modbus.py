from pyModbusTCP.client import ModbusClient
from pyModbusTCP.utils import word_list_to_long, decode_ieee
import time

# IP o hostname del servidor Modbus (puede ser localhost si el contenedor expone el puerto)
MODBUS_HOST = "localhost"
MODBUS_PORT = 502
ADDRESS_READ = 200
NUM_REGISTERS = 6  # Un float IEEE 754 = 2 registros Modbus de 16 bits

client = ModbusClient(host=MODBUS_HOST, port=MODBUS_PORT, auto_open=True)

def leer_float_modbus():
    if client.open():
        regs = client.read_holding_registers(ADDRESS_READ, NUM_REGISTERS)
        if regs and len(regs) == NUM_REGISTERS:
        #if regs:
            longs = word_list_to_long(regs)  # Convierte cada par de registros en un entero largo (32 bits)
            valores = [decode_ieee(l) for l in longs]  # Decodifica cada entero largo a float IEEE 754
            print(f"Valores leídos desde Modbus: {valores[0]}, {valores[1]}, {valores[2]}")
            #value_long = word_list_to_long(regs)
            #valor_float = decode_ieee(value_long[0])
            #print(f"Valor leído desde Modbus: {valor_float}")
        else:
            print("No se pudieron leer registros")
    else:
        print("No se pudo abrir conexión Modbus")

if __name__ == "__main__":
    while True:
        leer_float_modbus()
        time.sleep(2)
