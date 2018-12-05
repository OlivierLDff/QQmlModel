#include <QString>

#include "QQmlModelShared.h"

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