Qt QML Models
=============

*This fork aimed is to add CMake build functionality to this library*

Additional data models aimed to bring more power to QML applications by using useful C++ models in back-end.

* `QQmlObjectListModel` : a much nicer way to expose C++ list to QML than the quick & dirty `QList<QObject*>` property . Supports all the strong model features of `QAbstractListModel` while showing the simple and well know API of QList.

* `QQmlVariantListModel` : a dead-simple way to create a dynamic C++ list of any type and expose it to QML, way better than using a `QVariantList` property.

## CMake

The CMake can build the library either as a static or a shared library. It can also generate a doxygen website.

### Variables

#### Input

- **QQML_MODEL_TARGET** : Name of the library target. *Default : "QQmlModel"*
- **QQML_MODEL_PROJECT** : Name of the project. *Default : "QQmlModel"*
- **QQML_MODEL_BUILD_SHARED** : Build shared library [ON OFF]. *Default: OFF.*
- **QQML_MODEL_BUILD_STATIC** : Build static library [ON OFF]. *Default: ON.*
- **QQML_MODEL_USE_NAMESPACE** : If the library compile with a namespace. *Default: OFF.*
- **QQML_MODEL_NAMESPACE** : Namespace for the library. Only relevant if `QQML_MODEL_USE_NAMESPACE` is ON. *Default: "Qqm".*
- **QQML_MODEL_BUILD_DOC** : Build the QQmlModel Doc [ON OFF]. *Default: OFF.*
- **QQML_MODEL_DOXYGEN_BT_REPOSITORY** : Repository of DoxygenBt. *Default : "https://github.com/OlivierLDff/DoxygenBootstrappedCMake.git"*
- **QQML_MODEL_DOXYGEN_BT_TAG** : Git Tag of DoxygenBt. *Default : "v1.3.0"*

#### Output

- **QQML_MODEL_TARGET** : Output target to link to. *Default: "QQmlModel"*

### Building

#### In source

This library generate by default a static build. If you want a shared library set `-DQQML_MODEL_BUILD_SHARED=ON  `.

```bash
git clone https://github.com/OlivierLDff/QQmlModel.git
cd QQmlModel && mkdir build && cd build
cmake -DQT_DIR=path/to/qt/toolchain ..
```

In order to build with the documentation:

```bash
cmake -DQQML_MODEL_BUILD_DOC=ON -DQT_DIR=path/to/qt/toolchain ..
```

Then build the project depending on your CMake Generator. For example with Makefile generator.

```bash
make -j8
```

#### From external CMake project

The main goal of this CMake project is to big included into another CMake project.

```cmake
SET( QQML_MODEL_TARGET QQmlModel )
SET( QQML_MODEL_PROJECT QQmlModel )
SET( QQML_MODEL_BUILD_SHARED OFF )
SET( QQML_MODEL_BUILD_STATIC ON )
SET( QQML_MODEL_BUILD_DOC OFF )
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/path/to/QQmlModel ${CMAKE_CURRENT_BINARY_DIR}/QQmlModel_Build)
```

It is also possible to download the repository with the scripts inside `cmake/`. Simply call `BuildQQmlModel.cmake`.

You will need:

* `DownloadProject.cmake`
* `DownloadProject.CMakeLists.cmake.in`
* `BuildQQmlModel.cmake`

```cmake
SET( QQML_MODEL_TARGET QQmlModel )
SET( QQML_MODEL_PROJECT QQmlModel )
SET( QQML_MODEL_BUILD_SHARED OFF )
SET( QQML_MODEL_BUILD_STATIC ON )
SET( QQML_MODEL_BUILD_DOC OFF )
SET( QQML_MODEL_BUILD_DOC OFF )
SET( QQML_MODEL_USE_NAMESPACE ON )
SET( QQML_MODEL_REPOSITORY "https://github.com/OlivierLDff/QQmlModel.git" )
SET( QQML_MODEL_TAG v1.0.0 )
INCLUDE(path/to/BuildQQmlModel.cmake)
```

## Revisions

* [Thomas Boutroue](mailto:thomas.boutroue@gmail.com) :
  * Initial work.
* [Olivier Le Doeuff](olivier.ldff@gmail.com) : 
  * CMake support for the library.
  * Doxygen doc generation via CMake target `QQmlModelDoc`.
  * Move code into `src/` folder & Update QMake script to use the new folder.
  * Update `.gitignore` to allow in source building.
  * Move common declaration between Object and Gadget to `QQmlModelShared.h`
  * Namespace support.
  
  
> NOTE : If you want to donate to Thomas Boutroue, use this link : [![Flattr this](http://api.flattr.com/button/flattr-badge-large.png)](https://flattr.com/submit/auto?user_id=thebootroo&url=http://gitlab.unique-conception.org/qt-qml-tricks/qt-qml-models)
