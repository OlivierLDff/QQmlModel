import qbs;

Project {
    name: "Qt QML Models";

    Product {
        name: "libqtqmltricks-qtqmlmodels";
        type: "staticlibrary";
        targetName: "QtQmlModels";

        Export {
            cpp.includePaths: "./src";

            Depends { name: "cpp"; }
            Depends {
                name: "Qt";
                submodules: ["core", "qml"];
            }
        }
        Depends { name: "cpp"; }
        Depends {
            name: "Qt";
                submodules: ["core", "qml"];
        }
        Group {
            name: "C++ template sources";
            fileTags: ["txt"]
            files: [
                "src/QQmlObjectListModel.cpp",
                "src/QQmlVariantListModel.cpp"
            ]
        }
        Group {
            name: "C++ headers";
            files: [
                "src/QQmlObjectListModel.h",
                "src/QQmlVariantListModel.h",
                "src/QtQmlTricksPlugin_SmartDataModels.h",
            ]
        }
        Group {
            qbs.install: (product.type === "dynamiclibrary");
            fileTagsFilter: product.type;
        }
    }
}
