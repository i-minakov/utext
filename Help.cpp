#include "Help.h"

QString Help::getFileName(QString path) {
    QStringList splitList = path.split("/");

    for (int i = splitList.size() - 1; i >= 0; i--)
        if (splitList[i].size() != 0)
            return splitList[i];
    return nullptr;
}
