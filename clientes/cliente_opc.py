from opcua import Client
import time
# Cambia 'localhost' por la IP o hostname de tu servidor Docker si no es local
url = "opc.tcp://localhost:4840/freeopcua/server/"

client = Client(url)

try:
    client.connect()
    print("Conectado al servidor OPC UA")

    # con esto se puede inspeccionar que nodos estan activos

    # root = client.get_root_node()
    # print("Nodo raíz:", root)

    # # Accede al nodo Objects (índice 0)
    # objects_node = root.get_child(["0:Objects"])
    # print("Nodo Objects:", objects_node)

    # print("\nExplorando nodos hijos de Objects:")
    # for child in objects_node.get_children():
    #     browse_name = child.get_browse_name()
    #     print(f"- {browse_name} (NodeId: {child.nodeid})")

    root = client.get_root_node()

    # Accedemos directamente con los nombres correctos
    capacidad_node1 = root.get_child(["0:Objects", "2:capacidad_linea1"])
    capacidad_node2 = root.get_child(["0:Objects", "2:capacidad_linea2"])
    capacidad_node3 = root.get_child(["0:Objects", "2:capacidad_linea3"])

    # Bucle principal
    while True:
        valor1 = capacidad_node1.get_value()
        valor2 = capacidad_node2.get_value()
        valor3 = capacidad_node3.get_value()

        print(f"Capacidad línea 1: {valor1}")
        print(f"Capacidad línea 2: {valor2}")
        print(f"Capacidad línea 3: {valor3}")
        print("-" * 40)

        time.sleep(3)

finally:
    client.disconnect()
    print("Desconectado del servidor")
