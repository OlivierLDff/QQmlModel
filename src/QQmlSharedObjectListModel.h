#ifndef QQMLSHAREDOBJECTLISTMODEL_H
#define QQMLSHAREDOBJECTLISTMODEL_H

#include <QAbstractListModel>
#include <QByteArray>
#include <QChar>
#include <QDebug>
#include <QHash>
#include <QList>
#include <QMetaMethod>
#include <QMetaObject>
#include <QMetaProperty>
#include <QObject>
#include <QString>
#include <QStringBuilder>
#include <QVariant>
#include <QVector>
#include <QSharedPointer>

#include "QQmlModelShared.h"

QQMLMODEL_NAMESPACE_START

// custom foreach for QList, which uses no copy and check pointer non-null
#define SHARED_OBJECT_FOREACH_PTR_IN_QLIST(_type_, _var_, _list_) \
    for (typename QList<QSharedPointer<_type_>>::const_iterator it = _list_.constBegin (); it != _list_.constEnd (); ++it) \
        if (QSharedPointer<_type_> _var_ = (* it))

class QQmlSharedObjectListModelBase : public QAbstractListModel // abstract Qt base class
{
    Q_OBJECT

    // count is a property easy to access that give the size of the model
    Q_PROPERTY (int count READ count NOTIFY countChanged)
    // length can also be used as conveniance
    Q_PROPERTY (int length READ count NOTIFY countChanged)

public:
    explicit QQmlSharedObjectListModelBase (QObject * parent = Q_NULLPTR) : QAbstractListModel (parent) { }

public slots: // virtual methods API for QML
    /** Returns the number of items in the list.
     * in js model.size() */
    virtual int size (void) const = 0;
    /** This function is provided for STL compatibility.
     * It is equivalent to isEmpty() and returns true if the list is empty.
     * model.count or model.length in js will work */
    virtual int count (void) const = 0;
    /** Returns true if the list contains no items; otherwise returns false. */
    virtual bool isEmpty (void) const = 0;
    /** Returns true if the list contains an occurrence of value; otherwise returns false.
     * This function requires the value type to have an implementation of operator==(). */
    virtual bool contains (QSharedPointer<QObject> item) const = 0;
    /** Returns the index position of the first occurrence of value in the list,
     * searching forward from index position from. Returns -1 if no item matched.
     * \param item ptr the item you want the index of */
    virtual int indexOf (QSharedPointer<QObject> item) const = 0;
    /** Get the role id of name, -1 if role not found */
    virtual int roleForName (const QByteArray & name) const = 0;
    /** Removes all items from the list. It won't delete any object */
    virtual void clear (void) = 0;
    /** Inserts value at the end of the list.
     * This is the same as list.insert(size(), value).
     * If this list is not shared, this operation is typically
     * very fast (amortized constant time),
     * because QList preallocates extra space on both sides of
     * its internal buffer to allow for fast growth at both ends of the list.
     */
    virtual void append (QSharedPointer<QObject> item) = 0;
    /** Inserts value at the beginning of the list.
     * This is the same as list.insert(0, value).
     * If this list is not shared, this operation is typically very fast
     * (amortized constant time), because QList preallocates extra space
     * on both sides of its internal buffer to allow for fast growth at
     * both ends of the list.
     */
    virtual void prepend (QSharedPointer<QObject> item) = 0;
    /** Inserts value at index position i in the list.
     * If i <= 0, the value is prepended to the list.
     * If i >= size(), the value is appended to the list. */
    virtual void insert (int idx, QSharedPointer<QObject> item) = 0;
    /** Moves the item at index position from to index position to.
     * This is the same as insert(to, takeAt(from)).
     * This function assumes that both from and to are at least 0 but less than size().
     * To avoid failure, test that both from and to are at least 0 and less than size().
     */
    virtual void move (int idx, int pos) = 0;
    virtual void moveDown(const int row) = 0;
    virtual void moveUp(const int row) = 0;
    virtual void remove (QSharedPointer<QObject> item) = 0;
    /** Removes the item at index position i.
     *i must be a valid index position in the list (i.e., 0 <= i < size()). */
    virtual void remove (int idx) = 0;
    virtual QSharedPointer<QObject> get (int idx) const = 0;
    virtual QSharedPointer<QObject> get (const QString & uid) const = 0;
    virtual QSharedPointer<QObject> getFirst (void) const = 0;
    virtual QSharedPointer<QObject> getLast (void) const = 0;
    virtual QVariantList toVarArray (void) const = 0;

protected slots: // internal callback
    virtual void onItemPropertyChanged (void) = 0;

signals: // notifier
    /** Emitted when count changed (ie removed or inserted item) */
    void countChanged (void);
signals:
    /** Emitted when an item is about to be inserted */
    void itemAboutToBeInserted(QSharedPointer<QObject> item, int row);
    /** Emitted after an item is in the list */
    void itemInserted(QSharedPointer<QObject> item, int row);
    /** Emitted when an item is about to be moved */
    void itemAboutToBeMoved(QSharedPointer<QObject> item, int src, int dest);
    /** Emitted after an item have been moved */
    void itemMoved(QSharedPointer<QObject> item, int src, int dest);
    /** Emitted when an item is about to be removed */
    void itemAboutToBeRemoved(QSharedPointer<QObject> item, int row);
    /** Emitted when an item is about to be removed */
    void itemRemoved(QSharedPointer<QObject> item, int row);
};

template<class ItemType> class QQmlSharedObjectListModel : public QQmlSharedObjectListModelBase
{
public:
    explicit QQmlSharedObjectListModel (QObject *          parent      = Q_NULLPTR,
                                  const QList<QByteArray> & exposedRoles = QList<QByteArray>(),
                                  const QByteArray & displayRole = QByteArray (),
                                  const QByteArray & uidRole     = QByteArray ())
        : QQmlSharedObjectListModelBase (parent)
        , m_count (0)
        , m_uidRoleName (uidRole)
        , m_dispRoleName (displayRole)
        , m_metaObj (ItemType::staticMetaObject)
    {
        // Keep a track of black list rolename that are not compatible with Qml, they should never be used
        static QSet<QByteArray> roleNamesBlacklist;
        if (roleNamesBlacklist.isEmpty ()) {
            roleNamesBlacklist << QByteArrayLiteral ("id")
                               << QByteArrayLiteral ("index")
                               << QByteArrayLiteral ("class")
                               << QByteArrayLiteral ("model")
                               << QByteArrayLiteral ("modelData");
        }

        // Set handler that handle every property changed
        static const char * HANDLER = "onItemPropertyChanged()";
        m_handler = metaObject ()->method (metaObject ()->indexOfMethod (HANDLER));

        // Force a display role the the role map
        if (!displayRole.isEmpty ()) {
            m_roles.insert (Qt::DisplayRole, QByteArrayLiteral ("display"));
        }
        // Return a pointer to the qtObject as the base Role. This point is essential
        m_roles.insert (baseRole (), QByteArrayLiteral ("qtObject"));

        // Number of attribute declare with the Q_PROPERTY flags
        const int len = m_metaObj.propertyCount ();
        // For every property in the ItemType
        for (int propertyIdx = 0, role = (baseRole () +1); propertyIdx < len; propertyIdx++, role++) {
            QMetaProperty metaProp = m_metaObj.property (propertyIdx);
            const QByteArray propName = QByteArray (metaProp.name ());
            // Only expose the property as a role if:
            // - It isn't blacklisted (id, index, class, model, modelData)
            // - When exposedRoles is empty we expose every property
            // - When exposedRoles isn't empty we only expose the property asked by the user
            if (!roleNamesBlacklist.contains (propName) &&
                (exposedRoles.size() == 0 || exposedRoles.contains(propName)))
            {
                m_roles.insert (role, propName);
                // If there is a notify signal associated with the Q_PROPERTY we keep a track of it for fast lookup
                if (metaProp.hasNotifySignal ())
                {
                    m_signalIdxToRole.insert (metaProp.notifySignalIndex (), role);
                }
            }
            else if(roleNamesBlacklist.contains(propName))
            {
                static const QByteArray CLASS_NAME = (QByteArrayLiteral ("QQmlSharedObjectListModel<") % m_metaObj.className () % '>');
                qWarning () << "Can't have" << propName << "as a role name in" << qPrintable (CLASS_NAME) << ", because it's a blacklisted keywork in QML!. "
                "Please don't use any of the following words when declaring your Q_PROPERTY: (id, index, class, model, modelData)";
            }
        }
    }
    bool setData (const QModelIndex & index, const QVariant & value, int role) Q_DECL_FINAL {
        bool ret = false;
        QSharedPointer<QObject> item = at (index.row ());
        const QByteArray rolename = (role != Qt::DisplayRole ? m_roles.value (role, emptyBA ()) : m_dispRoleName);
        if (item != Q_NULLPTR && role != baseRole () && !rolename.isEmpty ()) {
            ret = item->setProperty (rolename, value);
        }
        return ret;
    }
    QVariant data (const QModelIndex & index, int role) const Q_DECL_FINAL {
        QVariant ret;
        QSharedPointer<QObject> item = at (index.row ());
        const QByteArray rolename = (role != Qt::DisplayRole ? m_roles.value (role, emptyBA ()) : m_dispRoleName);
        if (item != Q_NULLPTR && !rolename.isEmpty ()) {
            ret.setValue (role != baseRole () ? item->property (rolename) : QVariant::fromValue (item.staticCast<QObject>()));
        }
        return ret;
    }
    QHash<int, QByteArray> roleNames (void) const Q_DECL_FINAL {
        return m_roles;
    }
    typedef typename QList<QSharedPointer<ItemType>>::const_iterator const_iterator;
    const_iterator begin (void) const {
        return m_items.begin ();
    }
    const_iterator end (void) const {
        return m_items.end ();
    }
    const_iterator constBegin (void) const {
        return m_items.constBegin ();
    }
    const_iterator constEnd (void) const {
        return m_items.constEnd ();
    }

public: // C++ API
    QSharedPointer<ItemType> at (int idx) const {
        QSharedPointer<ItemType> ret = Q_NULLPTR;
        if (idx >= 0 && idx < m_items.size ()) {
            ret = m_items.value (idx);
        }
        return ret;
    }
    QSharedPointer<ItemType> getByUid (const QString & uid) const {
        return (!m_indexByUid.isEmpty () ? m_indexByUid.value (uid, Q_NULLPTR) : Q_NULLPTR);
    }
    int roleForName (const QByteArray & name) const Q_DECL_FINAL {
        return m_roles.key (name, -1);
    }
    int count (void) const Q_DECL_FINAL {
        return m_count;
    }
    int size (void) const Q_DECL_FINAL {
        return m_count;
    }
    bool isEmpty (void) const Q_DECL_FINAL {
        return m_items.isEmpty ();
    }
    bool contains (QSharedPointer<ItemType> item) const {
        return m_items.contains (item);
    }
    int indexOf (QSharedPointer<ItemType> item) const {
        return m_items.indexOf (item);
    }
    void clear (void) Q_DECL_FINAL {
        if (!m_items.isEmpty ()) {
            QList<QSharedPointer<ItemType>> tempList;
            for (int i = 0; i < m_items.count(); ++i)
                itemAboutToBeRemoved(m_items.at(i), i);
            beginRemoveRows (noParent (), 0, m_items.count () -1);
            SHARED_OBJECT_FOREACH_PTR_IN_QLIST (ItemType, item, m_items) {
                dereferenceItem (item);
                tempList.append(item);
            }
            m_items.clear ();
            updateCounter ();
            endRemoveRows ();
            for (int i = 0; i < tempList.count(); ++i)
                itemRemoved(tempList.at(i), i);
        }
    }
    void append (QSharedPointer<ItemType> item) {
        if (item != Q_NULLPTR) {
            const int pos = m_items.count ();
            itemAboutToBeInserted(item, pos);
            beginInsertRows (noParent (), pos, pos);
            m_items.append (item);
            referenceItem (item);
            updateCounter ();
            endInsertRows ();
            itemInserted(item, pos);
        }
    }
    void prepend (QSharedPointer<ItemType> item) {
        if (item != Q_NULLPTR) {
            itemAboutToBeInserted(item, 0);
            beginInsertRows (noParent (), 0, 0);
            m_items.prepend (item);
            referenceItem (item);
            updateCounter ();
            endInsertRows ();
            itemInserted(item, 0);
        }
    }
    void insert (int idx, QSharedPointer<ItemType> item) {
        if (item != Q_NULLPTR) {
            itemAboutToBeInserted(item, idx);
            beginInsertRows (noParent (), idx, idx);
            m_items.insert (idx, item);
            referenceItem (item);
            updateCounter ();
            endInsertRows ();
            itemInserted(item, idx);
        }
    }
    void append (const QList<QSharedPointer<ItemType>> & itemList) {
        if (!itemList.isEmpty ()) {
            const int pos = m_items.count ();
            for(int i = 0; i < itemList.count(); ++i)
                itemAboutToBeInserted(itemList.at(i), i + pos);

            beginInsertRows (noParent (), pos, pos + itemList.count () -1);
            m_items.reserve (m_items.count () + itemList.count ());
            m_items.append (itemList);
            SHARED_OBJECT_FOREACH_PTR_IN_QLIST (ItemType, item, itemList) {
                referenceItem (item);
            }
            updateCounter ();
            endInsertRows ();
            for (int i = 0; i < itemList.count(); ++i)
                itemInserted(itemList.at(i), i + pos);
        }
    }
    void prepend (const QList<QSharedPointer<ItemType>> & itemList) {
        if (!itemList.isEmpty ()) {
            for (int i = 0; i < itemList.count(); ++i)
                itemAboutToBeInserted(itemList.at(i), i);
            beginInsertRows (noParent (), 0, itemList.count () -1);
            m_items.reserve (m_items.count () + itemList.count ());
            int offset = 0;
            SHARED_OBJECT_FOREACH_PTR_IN_QLIST (ItemType, item, itemList) {
                m_items.insert (offset, item);
                referenceItem (item);
                offset++;
            }
            updateCounter ();
            endInsertRows ();
            for (int i = 0; i < itemList.count(); ++i)
                itemInserted(itemList.at(i), i);
        }
    }
    void insert (int idx, const QList<QSharedPointer<ItemType>> & itemList) {
        if (!itemList.isEmpty ()) {
            for (int i = 0; i < itemList.count(); ++i)
                itemAboutToBeInserted(itemList.at(i), i + idx);
            beginInsertRows (noParent (), idx, idx + itemList.count () -1);
            m_items.reserve (m_items.count () + itemList.count ());
            int offset = 0;
            SHARED_OBJECT_FOREACH_PTR_IN_QLIST (ItemType, item, itemList) {
                m_items.insert (idx + offset, item);
                referenceItem (item);
                offset++;
            }
            updateCounter ();
            endInsertRows ();
            for (int i = 0; i < itemList.count(); ++i)
                itemInserted(itemList.at(i), i + idx);
        }
    }
    void move (int idx, int pos) Q_DECL_FINAL {
        if (idx != pos && idx >=0 && pos>=0 && idx < m_items.size() && pos < m_items.size()) {
            itemAboutToBeMoved(m_items.at(idx), idx, pos);
            beginMoveRows (noParent (), idx, idx, noParent (), (idx < pos ? pos +1 : pos));
            m_items.move (idx, pos);
            endMoveRows ();
            itemMoved(m_items.at(idx), idx, pos);
        }
    }
    void remove (QSharedPointer<ItemType> item) {
        if (item != Q_NULLPTR) {
            const int idx = m_items.indexOf (item);
            remove (idx);
        }
    }
    void remove (int idx) Q_DECL_FINAL {
        if (idx >= 0 && idx < m_items.size ()) {
            itemAboutToBeRemoved(m_items.at(idx), idx);
            beginRemoveRows (noParent (), idx, idx);
            QSharedPointer<ItemType> item = m_items.takeAt (idx);
            dereferenceItem (item);
            updateCounter ();
            endRemoveRows ();
            itemRemoved(item, idx);
        }
    }
    QSharedPointer<ItemType> first (void) const {
        return m_items.first ();
    }
    QSharedPointer<ItemType> last (void) const {
        return m_items.last ();
    }
    const QList<QSharedPointer<ItemType>> & toList (void) const {
        return m_items;
    }

public: // QML slots implementation
    void append (QSharedPointer<QObject> item) Q_DECL_FINAL {
        append (qSharedPointerObjectCast<ItemType>(item));
    }
    void prepend (QSharedPointer<QObject> item) Q_DECL_FINAL {
        prepend (qSharedPointerObjectCast<ItemType>(item));
    }
    void insert (int idx, QSharedPointer<QObject> item) Q_DECL_FINAL {
        insert (idx, qSharedPointerObjectCast<ItemType>(item));
    }
    void remove (QSharedPointer<QObject> item) Q_DECL_FINAL {
        remove (qSharedPointerObjectCast<ItemType>(item));
    }
    bool contains (QSharedPointer<QObject> item) const Q_DECL_FINAL {
        return contains (qSharedPointerObjectCast<ItemType>(item));
    }
    int indexOf (QSharedPointer<QObject> item) const Q_DECL_FINAL {
        return indexOf (qSharedPointerObjectCast<ItemType>(item));
    }
    int indexOf (const QString & uid) const {
        return indexOf (get (uid));
    }
    QSharedPointer<QObject> get (int idx) const Q_DECL_FINAL {
        return qSharedPointerCast<QObject>(at(idx));
    }
    QSharedPointer<QObject> get (const QString & uid) const Q_DECL_FINAL {
        return qSharedPointerCast<QObject>(getByUid(uid));
    }
    QSharedPointer<QObject> getFirst (void) const Q_DECL_FINAL {
        return qSharedPointerCast<QObject>(first());
    }
    QSharedPointer<QObject> getLast (void) const Q_DECL_FINAL {
        return qSharedPointerCast<QObject>(last());
    }

protected: // internal stuff
    static const QString & emptyStr (void) {
        static const QString ret = QStringLiteral ("");
        return ret;
    }
    static const QByteArray & emptyBA (void) {
        static const QByteArray ret = QByteArrayLiteral ("");
        return ret;
    }
    static const QModelIndex & noParent (void) {
        static const QModelIndex ret = QModelIndex ();
        return ret;
    }
    static const int & baseRole (void) {
        static const int ret = Qt::UserRole;
        return ret;
    }
    int rowCount (const QModelIndex & parent = QModelIndex ()) const Q_DECL_FINAL {
        return (!parent.isValid () ? m_items.count () : 0);
    }
    void referenceItem (QSharedPointer<ItemType> item) {
        if (item != Q_NULLPTR) {
            if (!item->parent ()) {
                item->setParent (this);
            }
            for (QHash<int, int>::const_iterator it = m_signalIdxToRole.constBegin (); it != m_signalIdxToRole.constEnd (); ++it) {
                connect(item.get(), item->metaObject()->method(it.key()), this, m_handler, Qt::UniqueConnection);
            }
            if (!m_uidRoleName.isEmpty ()) {
                const QString key = m_indexByUid.key (item, emptyStr ());
                if (!key.isEmpty ()) {
                    m_indexByUid.remove (key);
                }
                const QString value = item->property (m_uidRoleName).toString ();
                if (!value.isEmpty ()) {
                    m_indexByUid.insert (value, item);
                }
            }
        }
    }
    void dereferenceItem (QSharedPointer<ItemType> item) {
        if (item != Q_NULLPTR) {
            disconnect (this, Q_NULLPTR, item.get(), Q_NULLPTR);
            disconnect (item.get(), Q_NULLPTR, this, Q_NULLPTR);
            if (!m_uidRoleName.isEmpty ()) {
                const QString key = m_indexByUid.key (item, emptyStr ());
                if (!key.isEmpty ()) {
                    m_indexByUid.remove (key);
                }
            }
            if (item->parent () == this) { // FIXME : maybe that's not the best way to test ownership ?
                item->deleteLater ();
            }
        }
    }
    void onItemPropertyChanged (void) Q_DECL_FINAL {
        ItemType * _item = qobject_cast<ItemType *> (sender ());
        QSharedPointer<ItemType> item;
        for(const auto it: *this)
        {
            if(it && it.get() == _item)
                item = it;
        }
        const int row = m_items.indexOf (item);
        const int sig = senderSignalIndex ();
        const int role = m_signalIdxToRole.value (sig, -1);
        if (row >= 0 && role >= 0) {
            const QModelIndex index = QAbstractListModel::index (row, 0, noParent ());
            QVector<int> rolesList;
            rolesList.append (role);
            if (m_roles.value (role) == m_dispRoleName) {
                rolesList.append (Qt::DisplayRole);
            }
            emit dataChanged (index, index, rolesList);
        }
        if (!m_uidRoleName.isEmpty ()) {
            const QByteArray roleName = m_roles.value (role, emptyBA ());
            if (!roleName.isEmpty () && roleName == m_uidRoleName) {
                const QString key = m_indexByUid.key (item, emptyStr ());
                if (!key.isEmpty ()) {
                    m_indexByUid.remove (key);
                }
                const QString value = item->property (m_uidRoleName).toString ();
                if (!value.isEmpty ()) {
                    m_indexByUid.insert (value, item);
                }
            }
        }
    }
    inline void updateCounter (void) {
        if (m_count != m_items.count ()) {
            m_count = m_items.count ();
            emit countChanged ();
        }
    }

private:
    void itemAboutToBeInserted(QSharedPointer<ItemType> item, int row) { onItemAboutToBeInserted(item, row); _onItemAboutToBeInserted(item, row); }
    void itemInserted(QSharedPointer<ItemType> item, int row) { onItemInserted(item, row); _onItemInserted(item, row); }
    void itemAboutToBeMoved(QSharedPointer<ItemType> item, int src, int dest) { onItemAboutToBeMoved(item, src, dest); _onItemAboutToBeMoved(item, src, dest); }
    void itemMoved(QSharedPointer<ItemType> item, int src, int dest) { onItemMoved(item, src, dest); _onItemMoved(item, src, dest); }
    void itemAboutToBeRemoved(QSharedPointer<ItemType> item, int row) { onItemAboutToBeRemoved(item, row); _onItemAboutToBeRemoved(item, row); }
    void itemRemoved(QSharedPointer<ItemType> item, int row) { onItemRemoved(item, row); _onItemRemoved(item, row); }

protected:
    virtual void onItemAboutToBeInserted(QSharedPointer<ItemType> item, int row) { }
    virtual void onItemInserted(QSharedPointer<ItemType> item, int row) { }
    virtual void onItemAboutToBeMoved(QSharedPointer<ItemType> item, int src, int dest) { }
    virtual void onItemMoved(QSharedPointer<ItemType> item, int src, int dest) { }
    virtual void onItemAboutToBeRemoved(QSharedPointer<ItemType> item, int row) { }
    virtual void onItemRemoved(QSharedPointer<ItemType> item, int row) { }

private:
    void _onItemAboutToBeInserted(QSharedPointer<ItemType> item, int row) { emit QQmlSharedObjectListModelBase::itemAboutToBeInserted(item, row); }
    void _onItemInserted(QSharedPointer<ItemType> item, int row) { emit QQmlSharedObjectListModelBase::itemInserted(item, row); }
    void _onItemAboutToBeMoved(QSharedPointer<ItemType> item, int src, int dest) { emit QQmlSharedObjectListModelBase::itemAboutToBeMoved(item, src, dest); }
    void _onItemMoved(QSharedPointer<ItemType> item, int src, int dest) { emit QQmlSharedObjectListModelBase::itemMoved(item, src, dest); }
    void _onItemAboutToBeRemoved(QSharedPointer<ItemType> item, int row) { emit QQmlSharedObjectListModelBase::itemAboutToBeRemoved(item, row); }
    void _onItemRemoved(QSharedPointer<ItemType> item, int row) { emit QQmlSharedObjectListModelBase::itemRemoved(item, row); }

    /** Move row to row-1 */
    void moveUp(const int row) override final
    {
        if (row > 0 && row < count())
            move(row, row - 1);
    }

    /** Move row to row+1 */
    void moveDown(const int row) override final
    {
        if (count() && // There is a least one entry
            row >= 0 && // We can be from the first
            row < (count() - 1) // To the last one minus 1
            )
        {
            return moveUp(row + 1);
        }
    }

private: // data members
    int                        m_count;
    QByteArray                 m_uidRoleName;
    QByteArray                 m_dispRoleName;
    QMetaObject                m_metaObj;
    QMetaMethod                m_handler;
    QHash<int, QByteArray>     m_roles;
    QHash<int, int>            m_signalIdxToRole;
    QList<QSharedPointer<ItemType>>          m_items;
    QHash<QString, QSharedPointer<ItemType>> m_indexByUid;
};

#define QQMLMODEL_SHARED_OBJ_PROPERTY(type, name, Name) \
    QQMLMODEL_SHARED_OBJ_PROPERTY_SUB(QQMLMODEL_NAMESPACE_NAME::QQmlSharedObjectListModel<type>, name, Name)

#define QQMLMODEL_SHARED_OBJ_PROPERTY_SUB(type, name, Name) \
    protected: Q_PROPERTY (QQMLMODEL_NAMESPACE_NAME::QQmlSharedObjectListModelBase name READ Get##Name CONSTANT) \
    private: type * _##name = new type(this); \
    public: type * Get##Name (void) const { return _##name; } \
    private:

#define QQMLMODEL_SHARED_OBJ_PROPERTY_SUB_NO_T(type, name, Name) \
    protected: Q_PROPERTY (type * name READ Get##Name CONSTANT) \
    private: type * _##name = new type(this); \
    public: type * Get##Name (void) const { return _##name; } \
    private:

/**
 * \internal
 */
class _Test_TestSharedObj : public QObject
{
    Q_OBJECT
public:
    _Test_TestSharedObj(QObject* parent = nullptr) : QObject(parent) {}
};

/**
 * \internal
 */
class _Test_QmlModelSharedObjList : public QQmlSharedObjectListModel<_Test_TestSharedObj>
{
    Q_OBJECT
    //QQMLMODEL_SHARED_OBJ_PROPERTY(_Test_TestObj, myObject, MyObject);
};

QQMLMODEL_NAMESPACE_END

#endif // QQMLOBJECTLISTMODEL_H
