#include "iec61850_server.h"
#include "hal_thread.h"
#include "static_model.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>

static int running = 0;

void sigint_handler(int sig) {
    running = 0;
}

void leer_capacidad_desde_mysql(MYSQL *conn, float *capacidad1, float *capacidad2, float *capacidad3) {
    MYSQL_RES *res;
    MYSQL_ROW row;

    if (mysql_query(conn, "SELECT capacidad_linea1, capacidad_linea2, capacidad_linea3 FROM capacidad ORDER BY id DESC LIMIT 1")) {
        fprintf(stderr, "Error en la consulta: %s\n", mysql_error(conn));
        *capacidad1 = *capacidad2 = *capacidad3 = 0.0;
        return;
    }

    res = mysql_store_result(conn);

    if (res == NULL) {
        fprintf(stderr, "Fallo al obtener el resultado: %s\n", mysql_error(conn));
        *capacidad1 = *capacidad2 = *capacidad3 = 0.0;
        return;
    }

    if ((row = mysql_fetch_row(res)) != NULL) {
        *capacidad1 = atof(row[0]);
        *capacidad2 = atof(row[1]);
        *capacidad3 = atof(row[2]);
    } else {
        printf("No hay registros.\n");
        *capacidad1 = *capacidad2 = *capacidad3 = 0.0;
    }

    mysql_free_result(res);
}

int main() {
    IedServer server = IedServer_create(&iedModel);
    float capacidad1 = 0.0;
    float capacidad2 = 0.0;
    float capacidad3 = 0.0;

    if (server == NULL) {
        printf("Error: No se pudo crear el servidor IEC 61850.\n");
        return -1;
    }

    IedServer_start(server, 102);

    if (!IedServer_isRunning(server)) {
        printf("No se pudo iniciar el servidor\n");
        return -1;
    }

    MYSQL *conn = mysql_init(NULL);
    if (!mysql_real_connect(conn, "mysql", "opcua_user", "opcua_pass", "opcua_db", 3306, NULL, 0)) {
        fprintf(stderr, "Error de conexión: %s\n", mysql_error(conn));
        mysql_close(conn);
        return 1;
    }

    signal(SIGINT, sigint_handler);
    running = 1;

    while (running) {
        leer_capacidad_desde_mysql(conn, &capacidad1, &capacidad2, &capacidad3);
        // IedServer_updateFloatAttributeValue(server, IEDMODEL_LD0_LLN00_CapacidadLinea_mag_f, capacidad1);
        
        // Actualizar los tres atributos en el servidor IEC 61850
        IedServer_updateFloatAttributeValue(server, IEDMODEL_LD0_LLN00_CapacidadLinea1_mag_f, capacidad1);
        IedServer_updateFloatAttributeValue(server, IEDMODEL_LD0_LLN00_CapacidadLinea2_mag_f, capacidad2);
        IedServer_updateFloatAttributeValue(server, IEDMODEL_LD0_LLN00_CapacidadLinea3_mag_f, capacidad3);

        // printf("Valor actualizado: %f\n", capacidad1);
        printf("Capacidad 1: %f | Capacidad 2: %f | Capacidad 3: %f\n", capacidad1, capacidad2, capacidad3);
        Thread_sleep(1000);
    }

    mysql_close(conn);
    IedServer_stop(server);
    IedServer_destroy(server);

    return 0;
}
