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

#endif