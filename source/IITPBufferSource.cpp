#include "Poco/URIStreamOpener.h"
#include "Poco/URI.h"

#include "IITPBufferSource.h"
#include "IITPStreamFactory.h"
#include "IITPClientSession.h"
#include <memory>

#include <RemoteLogger_Mb.h>
#include <Buffer_Mb.h>
#include <DataPlayer_Mb.h>


IITPBufferSourceApp::IITPBufferSourceApp(): _helpRequested(false)
{
}

IITPBufferSourceApp::~IITPBufferSourceApp()
{
}

void IITPBufferSourceApp::initialize(Poco::Util::Application &self)
{
    loadConfiguration(); // load default configuration files, if present
    ServerApplication::initialize(self);
}

void IITPBufferSourceApp::uninitialize()
{
    ServerApplication::uninitialize();
}

void IITPBufferSourceApp::defineOptions(Poco::Util::OptionSet &options)
{
    ServerApplication::defineOptions(options);

    options.addOption(
                Option("help", "h", "display help information on command line arguments")
                .required(false)
                .repeatable(false));
}

void IITPBufferSourceApp::handleOption(const std::string &name, const std::string &value)
{
    ServerApplication::handleOption(name, value);

    if (name == "help")
        _helpRequested = true;
}

void IITPBufferSourceApp::displayHelp()
{
    HelpFormatter helpFormatter(options());
    helpFormatter.setCommand(commandName());
    helpFormatter.setUsage("OPTIONS");
    helpFormatter.setHeader("Buffer source tester.");
    helpFormatter.format(std::cout);
}

using Innovative::Buffer;
ostream& operator<<(ostream& os, const Buffer& b)
{
    os.write(reinterpret_cast<const char*>(b.ConstHeader()), b.HeaderSizeInBytes());
    os.write(reinterpret_cast<const char*>(b.ConstData()), b.SizeInBytes());

    os.flush();

    return os;
}

int IITPBufferSourceApp::main(const std::vector<std::string> &/*args*/)
{
    if (_helpRequested)
    {
        displayHelp();
    }
    else
    {
        logger().information("Hello...");
        Innovative::IITPStreamFactory::registerFactory();

        Poco::URI uri("iitp://localhost:9980");
        uri.setPath("/test/data.bin");
        try
        {
            Innovative::RemoteLogger rlog("iitp://localhost:9980");

            Innovative::DataPlayer   Player;
            Player.FileName("C:/Users/hakan/DataFile/Data0.bin");

            bool ok = Player.Start();
            if (!ok ) {
                logger().information("File not opened");
                return 0;
            }

            logger().information("Pwd : " + rlog.getWorkingDirectory());

            rlog.setWorkingDirectory("c:\\users\\hakan\\DataFile\\");
            logger().information("Pwd : " + rlog.getWorkingDirectory());

            rlog.start();
            Innovative::VeloBuffer VeloBuf;
            int d = 0;
            while (Player.Percent() < 100) {
                Player.PlayWithHeader(VeloBuf);
                d += (VeloBuf.FullSizeInInts() * sizeof(int));
                rlog.log(VeloBuf);
            }

            logger().information(Poco::format("Sent %d ints", d));


            Player.Stop();

            rlog.stop();
        }
        catch (Poco::Exception& exc)
        {
            std::cerr << exc.displayText() << std::endl;
            return Application::EXIT_IOERR;;
        }

        logger().information("Done...");
        getchar();
    }
    return Application::EXIT_OK;
}



