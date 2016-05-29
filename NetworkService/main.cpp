#include <QCoreApplication>
#include <QProcess>

#include "../QtDashboard/mycoreapplication.h"
#include "servicecore.h"
#include "servicelog.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setApplicationName("QDNetService");
    QCoreApplication::setApplicationVersion("0.0.1-alpha");
    QCoreApplication::setOrganizationName("Sarvaritdinov Ravil <ra9oaj@gmail.com>");

    ServiceCore a(argc,argv);
    if(!a.isChildProcess())
    {
        return EXIT_SUCCESS;
    }
    else
    {
        a.childProcessStartSuccess();
        QTimer::singleShot(600*1000,qApp,SLOT(quit()));
    }


    return a.exec();
}

