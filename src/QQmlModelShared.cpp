#include <QString>

#include "QQmlModelShared.h"
#include "QQmlObjectListModel.h"
#include "QQmlVariantListModel.h"

QQMLMODEL_USING_NAMESPACE;

uint32_t QQmlModelVersion::getMajor()
{
	return QQMLMODEL_VERSION_MAJOR;
}

uint32_t QQmlModelVersion::getMinor()
{
	return QQMLMODEL_VERSION_MINOR;
}

uint32_t QQmlModelVersion::getPatch()
{
	return QQMLMODEL_VERSION_PATCH;
}

uint32_t QQmlModelVersion::getTag()
{
	return QQMLMODEL_VERSION_TAG_HEX;
}

QString QQmlModelVersion::getVersion()
{
	return QString::number(getMajor()) + "." +
		QString::number(getMinor()) + "." +
		QString::number(getTag()) + "." +
		QString::number(getTag(),16);
}

//void Qqm::registerQtQmlTricksSmartDataModel(QQmlEngine* engine)
//{
//    Q_UNUSED (engine)
//
//    const char* uri = "QtQmlTricks.SmartDataModels"; // @uri QtQmlTricks.SmartDataModels
//    const int maj = 2;
//    const int min = 0;
//    const char* msg = "!!!";
//
//    qmlRegisterUncreatableType<QQmlObjectListModelBase>(uri, maj, min, "ObjectListModel", msg);
//    qmlRegisterUncreatableType<QQmlVariantListModel>(uri, maj, min, "VariantListModel", msg);
//}
