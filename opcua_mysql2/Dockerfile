FROM python:3.10-slim

# Instala las dependencias necesarias
RUN apt-get update && apt-get install -y netcat-openbsd && \
    pip install opcua mysql-connector-python paho-mqtt pyModbusTCP numpy

# Copia los scripts y el wait-for-it
WORKDIR /app
COPY opcua_server.py /app/opcua_server.py
COPY mqtt_publisher.py /app/mqtt_publisher.py
COPY server_modbus.py /app/server_modbus.py
COPY ampacity_calculation2.py /app/ampacity_calculation2.py
COPY launcher.py /app/launcher.py
COPY wait-for-it.sh /usr/local/bin/wait-for-it.sh
RUN chmod +x /usr/local/bin/wait-for-it.sh
# Da permisos de ejecución al script de espera
# RUN chmod +x /usr/local/bin/wait-for-it.sh

# Comando por defecto: espera a MySQL y luego ejecuta launcher.py
CMD ["wait-for-it.sh", "mysql", "3306", "--", "python", "launcher.py"]
