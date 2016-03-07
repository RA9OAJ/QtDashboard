#include <QCoreApplication>
#include <QProcess>
#include <iostream>
#include "../QtDashboard/mycoreapplication.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setApplicationName("QDNetService");
    QCoreApplication::setApplicationVersion("0.0.1-alpha");
    QCoreApplication::setOrganizationName("Sarvaritdinov Ravil <ra9oaj@gmail.com>");

    MyCoreApplication a(argc, argv);
    a.setOnlyOneProcess(true,3);

    if(a.isFirstProcess())
    {
        if(!a.getSharedValue(true).contains("PARENT_PID"))
        {
            qint64 fork_pid = 0;
            QProcess::startDetached(a.applicationFilePath(),QStringList(),QString(),&fork_pid);

            if(fork_pid)
            {
                a.setSharedValue(QString("PID_%1").arg(QDir::homePath()),fork_pid,true);
                a.setSharedValue(QString("PARENT_PID"),a.applicationPid(),true);

                return 0;
            }
            else
            {
                std::cout<<"Service not started. Error: child process not started!"<<std::endl;
                std::cerr<<"Service not started. Error: child process not started!"<<std::endl;
                return 1;
            }
        }
    }
    else
    {
        //Service code
        QTimer::singleShot(5000,qApp,SLOT(quit()));
    }

    return a.exec();
}

