#include "ZDLListEntry.hpp"

ZDLListEntry::ZDLListEntry(const QString &originator, const QString &type) {
    this->originator = originator;
    this->type = type;
}

ZDLListEntry::ZDLListEntry([[maybe_unused]] ZDLWidget originator, const QString &type) {
    /* Default until I change this class */
    this->originator = "net.vectec.zdlsharp.qzdl.zqwidget";
    this->type = type;
}

void ZDLListEntry::addData(const QString &key, const QVariant &newData) {
    data.insert(key, newData);
}

void ZDLListEntry::removeData(const QString &key) {
    data.remove(key);
}

QVariant ZDLListEntry::getData(const QString &key) {
    return data.value(key);
}


