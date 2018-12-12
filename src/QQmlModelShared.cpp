#include <QString>

#include "QQmlModelShared.h"
#include "QQmlObjectListModel.h"
#include "QQmlVariantListModel.h"

QQML_MODEL_USING_NAMESPACE;

uint32_t QQmlModelVersion::GetMajor()
{
	return QQML_MODEL_VERSION_MAJOR;
}

uint32_t QQmlModelVersion::GetMinor()
{
	return QQML_MODEL_VERSION_MINOR;
}

uint32_t QQmlModelVersion::GetPatch()
{
	return QQML_MODEL_VERSION_PATCH;
}

uint32_t QQmlModelVersion::GetTag()
{
	return QQML_MODEL_VERSION_TAG_HEX;
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
