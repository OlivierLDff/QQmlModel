#ifndef QQMLMODELSHARED_H
#define QQMLMODELSHARED_H

#ifdef WIN32
	#ifdef QQML_MODEL_SHARED	// Shared build
		#define QQML_MODEL_API_ __declspec(dllexport)
	#elif QQML_MODEL_STATIC 	// No decoration when building staticlly
		#define QQML_MODEL_API_
	#else 				// Link to lib 
		#define QQML_MODEL_API_ __declspec(dllimport)
	#endif
#else
	#define QQML_MODEL_API_
#endif

#ifdef QQML_MODEL_USE_NAMESPACE
#ifndef QQML_MODEL_NAMESPACE_NAME
#define QQML_MODEL_NAMESPACE_NAME Qqm
#endif
#define QQML_MODEL_NAMESPACE_START namespace QQML_MODEL_NAMESPACE_NAME {
#define QQML_MODEL_NAMESPACE_END }
#define QQML_MODEL_USING_NAMESPACE using namespace QQML_MODEL_NAMESPACE_NAME;
#else
#undef QQML_MODEL_NAMESPACE_NAME
#define QQML_MODEL_NAMESPACE_NAME
#define QQML_MODEL_NAMESPACE_START
#define QQML_MODEL_NAMESPACE_END
#define QQML_MODEL_USING_NAMESPACE
#endif

QQML_MODEL_NAMESPACE_START

class QQML_MODEL_API_ QQmlModelVersion
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

QQML_MODEL_NAMESPACE_END

#endif