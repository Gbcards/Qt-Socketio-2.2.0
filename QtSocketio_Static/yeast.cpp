#include "yeast.h"



QString alphabet = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz-_";

int length = 64;
QStringList map = {};
int seed = 0;
int i = 0;
QString prev;

Yeast::Yeast()
{
    /*
  for (; i < length; i++)
  {
      map[alphabet[i]] = i;
  }
  */
}

QString Yeast::encode(int num)
{
    QString encoded = "";

    do {
        encoded = alphabet[num % length] + encoded;
        num = qFloor(num / length);
    } while (num > 0);

    return  encoded;
}

int Yeast::decode(QString str)
{
    int decoded = 0;

    for (i = 0; i < str.length(); i++) {
        //decoded = decoded * length + map[str[i]];
    }

    return decoded;
}

QString Yeast::getYeast()
{
    QString now = encode(QDateTime::currentMSecsSinceEpoch());

    if (now != prev)
    {
        prev = now;
        seed = 0;
        return prev;
    }
    return now +'.'+ encode(seed++);
}

