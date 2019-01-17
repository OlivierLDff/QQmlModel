#ifndef QQMLMODELSHARED_H
#define QQMLMODELSHARED_H

#include <QQmlEngine>
#include <QtQml>

#ifdef WIN32
	#ifdef QQMLMODEL_SHARED	// Shared build
		#define QQMLMODEL_API_ __declspec(dllexport)
	#elif QQMLMODEL_STATIC 	// No decoration when building staticlly
		#define QQMLMODEL_API_
	#else 				// Link to lib 
		#define QQMLMODEL_API_ __declspec(dllimport)
	#endif
#else
	#define QQMLMODEL_API_
#endif

#ifdef QQMLMODEL_USE_NAMESPACE
#ifndef QQMLMODEL_NAMESPACE_NAME
#define QQMLMODEL_NAMESPACE_NAME Qqm
#endif
#define QQMLMODEL_NAMESPACE_START namespace QQMLMODEL_NAMESPACE_NAME {
#define QQMLMODEL_NAMESPACE_END }
#define QQMLMODEL_USING_NAMESPACE using namespace QQMLMODEL_NAMESPACE_NAME;
#else
#undef QQMLMODEL_NAMESPACE_NAME
#define QQMLMODEL_NAMESPACE_NAME
#define QQMLMODEL_NAMESPACE_START
#define QQMLMODEL_NAMESPACE_END
#define QQMLMODEL_USING_NAMESPACE
#endif

QQMLMODEL_NAMESPACE_START

class QQMLMODEL_API_ QQmlModelVersion
{
public:
	/** Library Major Version */
	static uint32_t GetMajor();
	/** Library Minor Version */
	static uint32_t GetMinor();
	/** Library Patch Version */
	static uint32_t GetPatch();
	/** Library Tag Version */
	static uint32_t GetTag();
	/** Get version in form major.minor.patch.tag */
	static QString GetVersion();
};

	static void registerQtQmlTricksSmartDataModel(QQmlEngine* engine);

QQMLMODEL_NAMESPACE_END

#endif
