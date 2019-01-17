#include <QString>

#include "QQmlModelShared.h"
#include "QQmlObjectListModel.h"
#include "QQmlVariantListModel.h"

QQMLMODEL_USING_NAMESPACE;

uint32_t QQmlModelVersion::GetMajor()
{
	return QQMLMODEL_VERSION_MAJOR;
}

uint32_t QQmlModelVersion::GetMinor()
{
	return QQMLMODEL_VERSION_MINOR;
}

uint32_t QQmlModelVersion::GetPatch()
{
	return QQMLMODEL_VERSION_PATCH;
}

uint32_t QQmlModelVersion::GetTag()
{
	return QQMLMODEL_VERSION_TAG_HEX;
}

QString QQmlModelVersion::GetVersion()
{
	return QString::number(GetMajor()) + "." +
		QString::number(GetMinor()) + "." +
		QString::number(GetTag()) + "." +
		QString::number(GetTag(),16);
}

void Qqm::registerQtQmlTricksSmartDataModel(QQmlEngine* engine)
{
	Q_UNUSED (engine)

	const char* uri = "QtQmlTricks.SmartDataModels"; // @uri QtQmlTricks.SmartDataModels
	const int maj = 2;
	const int min = 0;
	const char* msg = "!!!";

	qmlRegisterUncreatableType<QQmlObjectListModelBase>(uri, maj, min, "ObjectListModel", msg);
	qmlRegisterUncreatableType<QQmlVariantListModel>(uri, maj, min, "VariantListModel", msg);
}
