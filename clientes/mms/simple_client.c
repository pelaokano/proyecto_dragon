#include <stdio.h>
#include "iec61850_client.h"
#include "hal_thread.h"

static int running = 1;

void sigint_handler(int sig) {
    running = 0;
}

int main() {
    IedClientError error;
    IedConnection conn = IedConnection_create();

    IedConnection_connect(conn, &error, "127.0.0.1", 102);

    if (error != IED_ERROR_OK) {
        printf("Error al conectar: %i\n", error);
        return -1;
    }

    printf("Conectado al servidor IEC 61850.\nPresiona Ctrl+C para salir.\n");

    while (running) {

        // MmsValue* val1 = IedConnection_readObject(conn, &error, "IED1LD0_LLN00_CapacidadLinea1_mag_f", IEC61850_FC_MX);
        MmsValue* val1 = IedConnection_readObject(conn, &error, "IED1LD0/LLN00.CapacidadLinea1.mag.f", IEC61850_FC_MX);
        float cap1 = 0.0;
        if (error == IED_ERROR_OK && val1 != NULL) {
            cap1 = MmsValue_toFloat(val1);
            MmsValue_delete(val1);
        } else {
            printf("Error leyendo Capacidad 1: %i\n", error);
        }

        // MmsValue* val2 = IedConnection_readObject(conn, &error, "IED1LD0_LLN00_CapacidadLinea2_mag_f", IEC61850_FC_MX);
        MmsValue* val2 = IedConnection_readObject(conn, &error, "IED1LD0/LLN00.CapacidadLinea2.mag.f", IEC61850_FC_MX);
        
        float cap2 = 0.0;
        if (error == IED_ERROR_OK && val2 != NULL) {
            cap2 = MmsValue_toFloat(val2);
            MmsValue_delete(val2);
        } else {
            printf("Error leyendo Capacidad 2: %i\n", error);
        }

        // MmsValue* val3 = IedConnection_readObject(conn, &error, "IED1LD0_LLN00_CapacidadLinea3_mag_f", IEC61850_FC_MX);
        MmsValue* val3 = IedConnection_readObject(conn, &error, "IED1LD0/LLN00.CapacidadLinea3.mag.f", IEC61850_FC_MX);
        float cap3 = 0.0;
        if (error == IED_ERROR_OK && val3 != NULL) {
            cap3 = MmsValue_toFloat(val3);
            MmsValue_delete(val3);
        } else {
            printf("Error leyendo Capacidad 3: %i\n", error);
        }

        printf("Capacidad 1: %.2f | Capacidad 2: %.2f | Capacidad 3: %.2f\n", cap1, cap2, cap3);

        Thread_sleep(1000);
    }

    printf("Cerrando conexión...\n");
    IedConnection_close(conn);
    IedConnection_destroy(conn);

    return 0;
}
//cl simple_client.c /I. /I "C:\Users\AdrianAlarconBecerra\Documents\proyectos_c\proyecto_61850\libiec61850\src\iec61850\inc" /I "C:\Users\AdrianAlarconBecerra\Documents\proyectos_c\proyecto_61850\libiec61850\src\stack\include" /I "C:\Users\AdrianAlarconBecerra\Documents\proyectos_c\proyecto_61850\libiec61850\src\iec61850\common" /I "C:\Users\AdrianAlarconBecerra\Documents\proyectos_c\proyecto_61850\libiec61850\src\mms\inc" /I "C:\Users\AdrianAlarconBecerra\Documents\proyectos_c\proyecto_61850\libiec61850\src\common\inc" /I "C:\Users\AdrianAlarconBecerra\Documents\proyectos_c\proyecto_61850\libiec61850\hal\inc" /I "C:\Users\AdrianAlarconBecerra\Documents\proyectos_c\proyecto_61850\libiec61850\src\logging" /link /MACHINE:X64 /LIBPATH:"C:\Users\AdrianAlarconBecerra\Documents\proyectos_c\proyecto_61850\libiec61850\build\src\Release" iec61850.lib ws2_32.lib
