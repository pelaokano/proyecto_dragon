# Base Ubuntu 22.04
FROM ubuntu:22.04

ENV WORKDIR=/root/proyecto_c
ENV LIBIEC61850_REPO=https://github.com/mz-automation/libiec61850.git
ENV MBEDTLS_VERSION=2.28.0

# Instalar dependencias básicas + mysql client y librerías para desarrollo mysql
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    wget \
    netcat-openbsd \
    libmbedtls-dev \
    default-libmysqlclient-dev \
    mysql-client \
    libmysqlclient-dev \
    pkg-config \
    && rm -rf /var/lib/apt/lists/*

WORKDIR $WORKDIR

# Clonar libiec61850
RUN git clone $LIBIEC61850_REPO

# Descargar y preparar Mbed TLS 2.28.0
RUN cd $WORKDIR/libiec61850/third_party && \
    wget -O mbedtls-$MBEDTLS_VERSION.tar.gz https://github.com/Mbed-TLS/mbedtls/archive/refs/tags/v$MBEDTLS_VERSION.tar.gz && \
    tar xzf mbedtls-$MBEDTLS_VERSION.tar.gz && \
    mv mbedtls-$MBEDTLS_VERSION mbedtls-2.28

# Compilar libiec61850 con soporte Mbed TLS
RUN cd $WORKDIR/libiec61850 && \
    make clean && \
    make WITH_MBEDTLS=1 && \
    make INSTALL_PREFIX=/usr/local install && \
    ldconfig

WORKDIR $WORKDIR

# Copiar tu código al contenedor
COPY simple_server.c static_model.c static_model.h $WORKDIR/
COPY wait-for-it.sh /usr/local/bin/wait-for-it.sh
RUN chmod +x /usr/local/bin/wait-for-it.sh

# Compilar tu aplicación con mysql_config
RUN gcc simple_server.c static_model.c \
    -I/usr/local/include/libiec61850 \
    -I/root/proyecto_c/libiec61850/hal/inc \
    -L/usr/local/lib -liec61850 -lpthread \
    $(mysql_config --cflags --libs) \
    -lmbedtls -lmbedx509 -lmbedcrypto \
    -o simple_server

EXPOSE 2102

# CMD ["./simple_server"]
CMD ["wait-for-it.sh", "mysql", "3306", "--", "./simple_server"]


