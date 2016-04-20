#include "servicecore.h"

ServiceCore::ServiceCore(int &argc, char **argv) :
    QCoreApplication(argc,argv)
{
    QString buf_name = (qApp->applicationName().isEmpty() ? qApp->applicationDirPath() : qApp->applicationName());
    if(!_buffer.connectToBuffer(buf_name))
    {

    }
}

ServiceCore::~ServiceCore()
{

}

bool ServiceCore::isParentProcess() const
{

}

bool ServiceCore::isChildProcess() const
{

}

void ServiceCore::scheduler()
{
    QTimer::singleShot(50,this,SLOT(scheduler()));
}

