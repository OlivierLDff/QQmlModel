#ifndef QQMLMODELSHARED_H
#define QQMLMODELSHARED_H

template<typename T> QList<T> qListFromVariant (const QVariantList & list) {
    QList<T> ret;
    ret.reserve (list.size ());
    for (QVariantList::const_iterator it = list.constBegin (); it != list.constEnd (); it++) {
        const QVariant & var = static_cast<QVariant>(* it);
        ret.append (var.value<T> ());
    }
    return ret;
}

template<typename T> QVariantList qListToVariant (const QList<T> & list) {
    QVariantList ret;
    ret.reserve (list.size ());
    for (typename QList<T>::const_iterator it = list.constBegin (); it != list.constEnd (); it++) {
        const T & val = static_cast<T>(* it);
        ret.append (QVariant::fromValue (val));
    }
    return ret;
}

#endif