#ifndef PAINTWEBSOCKET_H
#define PAINTWEBSOCKET_H

#include <QMainWindow>

namespace Ui {
class PaintWebSocket;
}

class PaintWebSocket : public QMainWindow
{
    Q_OBJECT

public:
    explicit PaintWebSocket(QWidget *parent = nullptr);
    ~PaintWebSocket();

private slots:

    void on_btnNegro_clicked();

    void on_btnRojo_clicked();

    void on_btnVerde_clicked();

    void on_btnAzul_clicked();

    void on_btnAmarillo_clicked();

private:
    Ui::PaintWebSocket *ui;
};

#endif // PAINTWEBSOCKET_H
