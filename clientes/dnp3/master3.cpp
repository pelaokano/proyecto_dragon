#include <opendnp3/ConsoleLogger.h>
#include <opendnp3/DNP3Manager.h>
#include <opendnp3/channel/PrintingChannelListener.h>
#include <opendnp3/logging/LogLevels.h>
#include <opendnp3/master/DefaultMasterApplication.h>
#include <opendnp3/master/ISOEHandler.h>

#include <chrono>
#include <thread>
#include <memory>
#include <iostream>

using namespace opendnp3;
using namespace std;

/* Handler que solo procesa analógicos */
class SimpleAnalogHandler : public ISOEHandler
{
    void BeginFragment(const ResponseInfo&) override {}
    void EndFragment(const ResponseInfo&) override {}

    void Process(const HeaderInfo&, const ICollection<Indexed<Analog>>& values) override
    {
        struct Visitor final : public IVisitor<Indexed<Analog>>
        {
            void OnValue(const Indexed<Analog>& item) override
            {
                cout << "Analog[" << item.index << "] = " << item.value.value << endl;
            }
        } printer;

        values.Foreach(printer);
    }

    // Ignorar otros tipos
    void Process(const HeaderInfo&, const ICollection<Indexed<Binary>>&) override {}
    void Process(const HeaderInfo&, const ICollection<Indexed<DoubleBitBinary>>&) override {}
    void Process(const HeaderInfo&, const ICollection<Indexed<Counter>>&) override {}
    void Process(const HeaderInfo&, const ICollection<Indexed<FrozenCounter>>&) override {}
    void Process(const HeaderInfo&, const ICollection<Indexed<BinaryOutputStatus>>&) override {}
    void Process(const HeaderInfo&, const ICollection<Indexed<AnalogOutputStatus>>&) override {}
    void Process(const HeaderInfo&, const ICollection<Indexed<OctetString>>&) override {}
    void Process(const HeaderInfo&, const ICollection<Indexed<TimeAndInterval>>&) override {}
    void Process(const HeaderInfo&, const ICollection<Indexed<BinaryCommandEvent>>&) override {}
    void Process(const HeaderInfo&, const ICollection<Indexed<AnalogCommandEvent>>&) override {}
    void Process(const HeaderInfo&, const ICollection<DNPTime>&) override {}
};

/* Función para solicitar lectura analógica (30/5, índices 0-2) */
void solicitarAnalogicos(const shared_ptr<IMaster>& master,
                         const shared_ptr<ISOEHandler>& handler)
{
    master->ScanRange(GroupVariationID(30, 5),  // Grupo 30, Var 5 (float)
                      0, 2,                     // índices 0 a 2
                      handler);                 // handler para la respuesta
}

int main()
{
    const auto logLevels = levels::NORMAL | levels::ALL_COMMS;

    DNP3Manager manager(1, ConsoleLogger::Create());

    auto channel = manager.AddTCPClient(
        "client",
        logLevels,
        ChannelRetry::Default(),                 // reconexión automática
        { IPEndpoint("127.0.0.1", 20000) },      // dirección del outstation
        "0.0.0.0",                               // bind local
        PrintingChannelListener::Create());

    MasterStackConfig cfg;
    cfg.link.LocalAddr = 1;
    cfg.link.RemoteAddr = 10;
    cfg.master.responseTimeout = TimeDuration::Seconds(2);
    cfg.master.startupIntegrityClassMask = ClassField::None();  // no poll automático

    auto handler = make_shared<SimpleAnalogHandler>();

    auto master = channel->AddMaster(
        "master", handler, DefaultMasterApplication::Create(), cfg);

    master->Enable();

    // Bucle: consulta cada 5 segundos
    while (true)
    {
        solicitarAnalogicos(master, handler);
        this_thread::sleep_for(chrono::seconds(5));
    }

    return 0;
}
