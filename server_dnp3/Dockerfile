FROM ubuntu:20.04

ENV DEBIAN_FRONTEND=noninteractive
ENV LD_LIBRARY_PATH=/usr/local/lib

# Instala herramientas necesarias
RUN apt-get update && apt-get install -y \
    git \
    g++ \
    cmake \
    make \
    libssl-dev \
    libmysqlclient-dev \
    wget \
    netcat-openbsd \
    unzip \
    pkg-config \
    && apt-get clean && rm -rf /var/lib/apt/lists/*

# Clona y compila OpenDNP3
WORKDIR /opt
RUN git clone https://github.com/dnp3/opendnp3.git

WORKDIR /opt/opendnp3
RUN mkdir build && cd build && \
    cmake -DBUILD_EXAMPLES=OFF -DBUILD_TESTS=OFF .. && \
    make -j$(nproc) && \
    make install

# Copia tu archivo fuente outstation2.cpp al contenedor
# (Asumiendo que outstation2.cpp está en la misma carpeta donde haces el build)
# Copiar wait-for-it.sh
COPY outstation2.cpp /opt/opendnp3/outstation2.cpp
COPY wait-for-it.sh /usr/local/bin/wait-for-it.sh
RUN chmod +x /usr/local/bin/wait-for-it.sh

WORKDIR /opt/opendnp3

# Compila tu archivo usando las librerías instaladas
RUN g++ outstation2.cpp -std=c++17 -I/usr/local/include -L/usr/local/lib -lopendnp3 -lmysqlclient -lpthread -o outstation2

# Usar wait-for-it
CMD ["wait-for-it.sh", "mysql", "3306", "--", "./outstation2"]