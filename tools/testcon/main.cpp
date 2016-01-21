/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the tools applications of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL21$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "mainwindow.h"

#include <QApplication>
#include <QAxFactory>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QDebug>
#include <QDesktopWidget>

QAXFACTORY_DEFAULT(MainWindow,
                   QLatin1String("{5f5ce700-48a8-47b1-9b06-3b7f79e41d7c}"),
                   QLatin1String("{3fc86f5f-8b15-4428-8f6b-482bae91f1ae}"),
                   QLatin1String("{02a268cd-24b4-4fd9-88ff-b01b683ef39d}"),
                   QLatin1String("{4a43e44d-9d1d-47e5-a1e5-58fe6f7be0a4}"),
                   QLatin1String("{16ee5998-77d2-412f-ad91-8596e29f123f}"))

QT_USE_NAMESPACE

static void redirectDebugOutput(QtMsgType, const QMessageLogContext &, const QString &msg)
{
    if (MainWindow *mainWindow = MainWindow::instance())
        mainWindow->appendLogText(msg);
}

int main( int argc, char **argv )
{
    QApplication app( argc, argv );
    QCoreApplication::setApplicationName(QLatin1String("TestCon"));
    QCoreApplication::setOrganizationName(QLatin1String("QtProject"));
    QCoreApplication::setApplicationVersion(QLatin1String(QT_VERSION_STR));
    QCommandLineParser parser;
    parser.setApplicationDescription(QLatin1String("ActiveX Control Test Container"));
    parser.addHelpOption();
    parser.addVersionOption();
    QCommandLineOption scriptOption(QLatin1String("script"),
                                    QLatin1String("A script to load."),
                                    QLatin1String("script"));
    parser.addOption(scriptOption);
    QCommandLineOption noMessageHandlerOption(QLatin1String("no-messagehandler"),
                                              QLatin1String("Suppress installation of the message handler."));
    parser.addOption(noMessageHandlerOption);
    parser.addPositionalArgument(QLatin1String("clsid/file"),
                                 QLatin1String("The clsid/file to show."));
    parser.process(app);

    if (!parser.isSet(noMessageHandlerOption))
        qInstallMessageHandler(redirectDebugOutput);

    MainWindow mw;
    foreach (const QString &a, parser.positionalArguments()) {
        if (a.startsWith(QLatin1Char('{')) && a.endsWith(QLatin1Char('}')))
            mw.addControlFromClsid(a);
        else
            mw.addControlFromFile(a);
    }
    if (parser.isSet(scriptOption))
        mw.loadScript(parser.value(scriptOption));

    const QRect availableGeometry = QApplication::desktop()->availableGeometry(&mw);
    mw.resize(availableGeometry.size() * 2 / 3);
    mw.show();

    return app.exec();;
}
