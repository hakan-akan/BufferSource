#ifndef IITP_BUFFERSOURCE_H
#define IITP_BUFFERSOURCE_H

#include "Poco/Util/ServerApplication.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"
#include <iostream>
//
#include "IITPRequestHandlerFactory.h"
#include "IITPRequestHandler.h"


using Poco::Util::ServerApplication;
using Poco::Util::Application;
using Poco::Util::Option;
using Poco::Util::OptionSet;
using Poco::Util::HelpFormatter;


using Innovative::IITPRequestHandlerFactory;
using Innovative::IITPRequestHandler;




class FormRequestHandler: public IITPRequestHandler
{
public:
    FormRequestHandler()
    {
    }

    void handleRequest()
    {
        Application& app = Application::instance();
        app.logger().information("Received a request... ");

   }
};


class FormRequestHandlerFactory: public IITPRequestHandlerFactory
{
public:
    FormRequestHandlerFactory()
    {
    }

    IITPRequestHandler* createRequestHandler()
    {
        return new FormRequestHandler;
    }
};



class IITPBufferSourceApp: public Poco::Util::ServerApplication
{
public:
    IITPBufferSourceApp();

    ~IITPBufferSourceApp();

protected:
    void initialize(Application& self);

    void uninitialize();

    void defineOptions(OptionSet& options);

    void handleOption(const std::string& name, const std::string& value);

    void displayHelp();

    int main(const std::vector<std::string>& args);

private:
    bool _helpRequested;
};

#endif
