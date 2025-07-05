#include <opendnp3/ConsoleLogger.h>
#include <opendnp3/DNP3Manager.h>
#include <opendnp3/channel/PrintingChannelListener.h>
#include <opendnp3/logging/LogLevels.h>
#include <opendnp3/outstation/DefaultOutstationApplication.h>
#include <opendnp3/outstation/IUpdateHandler.h>
#include <opendnp3/outstation/SimpleCommandHandler.h>
#include <opendnp3/outstation/UpdateBuilder.h>

#include <iostream>
#include <thread>
#include <random>
#include <chrono>
#include <mysql/mysql.h>  // Librería MySQL C API

using namespace std;
using namespace opendnp3;

DatabaseConfig ConfigureDatabase()
{
    DatabaseConfig config(3); // Hasta 10 puntos analógicos

    for (int i = 0; i < 3; ++i)
    {
        config.analog_input[i].clazz = PointClass::Class1;
        config.analog_input[i].svariation = StaticAnalogVariation::Group30Var5;
        config.analog_input[i].evariation = EventAnalogVariation::Group32Var7;
    }

    return config;
}

// Función para leer las capacidades desde MySQL
bool leer_capacidad_desde_mysql(MYSQL* conn, float& capacidad1, float& capacidad2, float& capacidad3)
{
    if (mysql_query(conn, "SELECT capacidad_linea1, capacidad_linea2, capacidad_linea3 FROM capacidad ORDER BY id DESC LIMIT 1"))
    {
        cerr << "Error en la consulta: " << mysql_error(conn) << endl;
        capacidad1 = capacidad2 = capacidad3 = 0.0f;
        return false;
    }

    MYSQL_RES* res = mysql_store_result(conn);
    if (res == nullptr)
    {
        cerr << "Fallo al obtener resultado: " << mysql_error(conn) << endl;
        capacidad1 = capacidad2 = capacidad3 = 0.0f;
        return false;
    }

    MYSQL_ROW row = mysql_fetch_row(res);
    if (row != nullptr)
    {
        capacidad1 = atof(row[0]);
        capacidad2 = atof(row[1]);
        capacidad3 = atof(row[2]);
        mysql_free_result(res);
        return true;
    }
    else
    {
        cout << "No hay registros." << endl;
        capacidad1 = capacidad2 = capacidad3 = 0.0f;
        mysql_free_result(res);
        return false;
    }
}

int main()
{

     // Inicializar conexión MySQL
    MYSQL* conn = mysql_init(nullptr);
    if (!conn)
    {
        cerr << "mysql_init() falló" << endl;
        return -1;
    }
    // "127.0.0.1", "opcua_user", "opcua_pass", "opcua_db", 3306,
    if (!mysql_real_connect(conn, "mysql", "opcua_user", "opcua_pass", "opcua_db", 3306, nullptr, 0))
    {
        cerr << "Error conectando a MySQL: " << mysql_error(conn) << endl;
        return -1;
    }

    // Niveles de log a consola
    const auto logLevels = levels::NORMAL | levels::ALL_COMMS;

    DNP3Manager manager(1, ConsoleLogger::Create());

    // Crear canal TCP
    auto channel = manager.AddTCPServer(
        "server", logLevels, ServerAcceptMode::CloseExisting,
        IPEndpoint("0.0.0.0", 20000), PrintingChannelListener::Create()
    );

    // Configurar el outstation
    OutstationStackConfig config(ConfigureDatabase());

    EventBufferConfig configEvents(
        0,  // Binary events
        0,  // DoubleBitBinary events
        3,  // Analog events
        0,  // Counter events
        0,  // FrozenCounter events
        0,  // BinaryOutputStatus events
        0   // AnalogOutputStatus events
        );

    config.outstation.eventBufferConfig = configEvents;
    // config.outstation.eventBufferConfig = EventBufferConfig::AllTypes(100);
    config.outstation.params.allowUnsolicited = true;
    config.link.LocalAddr = 10;
    config.link.RemoteAddr = 1;
    config.link.KeepAliveTimeout = TimeDuration::Max();

    auto outstation = channel->AddOutstation(
        "outstation", SuccessCommandHandler::Create(),
        DefaultOutstationApplication::Create(), config
    );

    outstation->Enable();

    // Generador de números aleatorios
    // std::random_device rd;
    // std::mt19937 gen(rd());
    // std::uniform_real_distribution<> dis(0.0, 100.0);

    // Bucle de actualización periódica
    while (true)
    {
        float cap1, cap2, cap3;
        if (!leer_capacidad_desde_mysql(conn, cap1, cap2, cap3))
        {
            cerr << "Error leyendo capacidades, se usarán valores 0.0" << endl;
        }

        UpdateBuilder builder;

        // for (int i = 0; i < 3; ++i)
        // {
        //     builder.Update(Analog(dis(gen)), i); // puntos 0, 1, 2
        // }
        builder.Update(Analog(cap1), 0);
        builder.Update(Analog(cap2), 1);
        builder.Update(Analog(cap3), 2);

        outstation->Apply(builder.Build());
        // std::cout << "Valores analógicos actualizados." << std::endl;
        cout << "Valores analógicos actualizados: " << cap1 << ", " << cap2 << ", " << cap3 << endl;
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
    mysql_close(conn);

    return 0;
}
