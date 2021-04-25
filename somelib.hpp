#pragma once

#include <QObject>
#include <QVariant>
#include <QTimer>
#include <QElapsedTimer>
#include <QDebug>


class SomeClass : public QObject {
    Q_OBJECT

    Q_PROPERTY(int var
        READ var
        WRITE setVar
        NOTIFY hello
    )

  signals:
    void hello();

  public slots:
    void setToOne();

  private:
    int m_var = 0;
    int var();
    void setVar(int value);
};
