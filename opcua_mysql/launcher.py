from multiprocessing import Process
import opcua_server  # debe tener una función run()
import mqtt_publisher  # debe tener una función run()
import server_modbus
import ampacity_calculation2

def main():
    p1 = Process(target=opcua_server.run)
    p2 = Process(target=mqtt_publisher.run)
    p3 = Process(target=server_modbus.run_server)
    p4 = Process(target=ampacity_calculation2.run)

    p1.start()
    p2.start()
    p3.start()
    p4.start()

    p1.join()
    p2.join()
    p3.join()
    p4.join()

if __name__ == "__main__":
    main()
