#ifndef QQMLOBJECTLISTMODEL_H
#define QQMLOBJECTLISTMODEL_H

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

#include "QQmlModelShared.h"

QQMLMODEL_NAMESPACE_START

template<typename T> QList<T> qListFromVariant (const QVariantList & list) {
    QList<T> ret;
    ret.reserve (list.size ());
    for (QVariantList::const_iterator it = list.constBegin (); it != list.constEnd (); ++it) {
        const QVariant & var = static_cast<QVariant>(* it);
        ret.append (var.value<T> ());
    }
    return ret;
}

template<typename T> QVariantList qListToVariant (const QList<T> & list) {
    QVariantList ret;
    ret.reserve (list.size ());
    for (typename QList<T>::const_iterator it = list.constBegin (); it != list.constEnd (); ++it) {
        const T & val = static_cast<T>(* it);
        ret.append (QVariant::fromValue (val));
    }
    return ret;
}

// custom foreach for QList, which uses no copy and check pointer non-null
#define FOREACH_PTR_IN_QLIST(_type_, _var_, _list_) \
    for (typename QList<_type_ *>::const_iterator it = _list_.constBegin (); it != _list_.constEnd (); ++it) \
        if (_type_ * _var_ = (* it))

class QQmlObjectListModelBase : public QAbstractListModel { // abstract Qt base class
    Q_OBJECT
    Q_PROPERTY (int count READ count NOTIFY countChanged)

public:
    explicit QQmlObjectListModelBase (QObject * parent = Q_NULLPTR) : QAbstractListModel (parent) { }

public slots: // virtual methods API for QML
	/** Returns the number of items in the list. */
	Q_INVOKABLE virtual int size (void) const = 0;
	/** This function is provided for STL compatibility. 
	 * It is equivalent to isEmpty() and returns true if the list is empty. */
	Q_INVOKABLE virtual int count (void) const = 0;
	/** Returns true if the list contains no items; otherwise returns false. */
	Q_INVOKABLE virtual bool isEmpty (void) const = 0;
	/** Returns true if the list contains an occurrence of value; otherwise returns false.
	 * This function requires the value type to have an implementation of operator==(). */
	Q_INVOKABLE virtual bool contains (QObject * item) const = 0;
	/** Returns the index position of the first occurrence of value in the list, 
	 * searching forward from index position from. Returns -1 if no item matched.
	 * \param item ptr the item you want the index of */
	Q_INVOKABLE virtual int indexOf (QObject * item) const = 0;
	/** Get the role id of name, -1 if role not found */
	Q_INVOKABLE virtual int roleForName (const QByteArray & name) const = 0;
	/** Removes all items from the list. It won't delete any object */
	Q_INVOKABLE virtual void clear (void) = 0;
	/** Inserts value at the end of the list.
	 * This is the same as list.insert(size(), value). 
	 * If this list is not shared, this operation is typically 
	 * very fast (amortized constant time), 
	 * because QList preallocates extra space on both sides of 
	 * its internal buffer to allow for fast growth at both ends of the list.
	 */
	Q_INVOKABLE virtual void append (QObject * item) = 0;
	/** Inserts value at the beginning of the list. 
	 * This is the same as list.insert(0, value).
	 * If this list is not shared, this operation is typically very fast
	 * (amortized constant time), because QList preallocates extra space
	 * on both sides of its internal buffer to allow for fast growth at 
	 * both ends of the list.
	 */
    Q_INVOKABLE virtual void prepend (QObject * item) = 0;
	/** Inserts value at index position i in the list. 
	 * If i <= 0, the value is prepended to the list. 
	 * If i >= size(), the value is appended to the list. */
	Q_INVOKABLE virtual void insert (int idx, QObject * item) = 0;
	/** Moves the item at index position from to index position to. 
	 * This is the same as insert(to, takeAt(from)).
	 * This function assumes that both from and to are at least 0 but less than size(). 
	 * To avoid failure, test that both from and to are at least 0 and less than size().
	 */
	Q_INVOKABLE virtual void move (int idx, int pos) = 0;
	Q_INVOKABLE virtual void remove (QObject * item) = 0;
	/** Removes the item at index position i. 
	 *i must be a valid index position in the list (i.e., 0 <= i < size()). */
	Q_INVOKABLE virtual void remove (int idx) = 0;
	Q_INVOKABLE virtual QObject * get (int idx) const = 0;
	Q_INVOKABLE virtual QObject * get (const QString & uid) const = 0;
	Q_INVOKABLE virtual QObject * getFirst (void) const = 0;
	Q_INVOKABLE virtual QObject * getLast (void) const = 0;
    virtual QVariantList toVarArray (void) const = 0;
public:
	Q_INVOKABLE virtual void Append() = 0;
	Q_INVOKABLE virtual void Remove(const int row) = 0;
	Q_INVOKABLE virtual void Move(const int src, const int dest) = 0;
	Q_INVOKABLE virtual void Insert(const int src) = 0;
	Q_INVOKABLE virtual void MoveUp(const int row) = 0;
	Q_INVOKABLE virtual void MoveDown(const int row) = 0;
	Q_INVOKABLE virtual void Clear() = 0;
	Q_INVOKABLE virtual QObject * At(const int row) = 0;
	Q_INVOKABLE virtual int Count() const = 0;
	Q_INVOKABLE virtual int Size() const = 0;

protected slots: // internal callback
    virtual void onItemPropertyChanged (void) = 0;

signals: // notifier
	/** Emitted when count changed (ie removed or inserted item) */
    void countChanged (void);
signals:
	/** Emitted when an item is about to be inserted */
	void itemAboutToBeInserted(QObject* item, int row);
	/** Emitted after an item is in the list */
	void itemInserted(QObject* item, int row);
	/** Emitted when an item is about to be moved */
	void itemAboutToBeMoved(QObject* item, int src, int dest);
	/** Emitted after an item have been moved */
	void itemMoved(QObject* item, int src, int dest);
	/** Emitted when an item is about to be removed */
	void itemAboutToBeRemoved(QObject* item, int row);
	/** Emitted when an item is about to be removed */
	void itemRemoved(QObject* item, int row);
};

template<class ItemType> class QQmlObjectListModel : public QQmlObjectListModelBase 
{
public:
    explicit QQmlObjectListModel (QObject *          parent      = Q_NULLPTR,
								  const QList<QByteArray> & exposedRoles = QList<QByteArray>(),
                                  const QByteArray & displayRole = QByteArray (),
                                  const QByteArray & uidRole     = QByteArray ())
        : QQmlObjectListModelBase (parent)
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
                static const QByteArray CLASS_NAME = (QByteArrayLiteral ("QQmlObjectListModel<") % m_metaObj.className () % '>');
                qWarning () << "Can't have" << propName << "as a role name in" << qPrintable (CLASS_NAME) << ", because it's a blacklisted keywork in QML!. "
            	"Please don't use any of the following words when declaring your Q_PROPERTY: (id, index, class, model, modelData)";
            }
        }
    }
    bool setData (const QModelIndex & index, const QVariant & value, int role) Q_DECL_FINAL {
        bool ret = false;
        ItemType * item = at (index.row ());
        const QByteArray rolename = (role != Qt::DisplayRole ? m_roles.value (role, emptyBA ()) : m_dispRoleName);
        if (item != Q_NULLPTR && role != baseRole () && !rolename.isEmpty ()) {
            ret = item->setProperty (rolename, value);
        }
        return ret;
    }
    QVariant data (const QModelIndex & index, int role) const Q_DECL_FINAL {
        QVariant ret;
        ItemType * item = at (index.row ());
        const QByteArray rolename = (role != Qt::DisplayRole ? m_roles.value (role, emptyBA ()) : m_dispRoleName);
        if (item != Q_NULLPTR && !rolename.isEmpty ()) {
            ret.setValue (role != baseRole () ? item->property (rolename) : QVariant::fromValue (static_cast<QObject *> (item)));
        }
        return ret;
    }
    QHash<int, QByteArray> roleNames (void) const Q_DECL_FINAL {
        return m_roles;
    }
    typedef typename QList<ItemType *>::const_iterator const_iterator;
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
    ItemType * at (int idx) const {
        ItemType * ret = Q_NULLPTR;
        if (idx >= 0 && idx < m_items.size ()) {
            ret = m_items.value (idx);
        }
        return ret;
    }
    ItemType * getByUid (const QString & uid) const {
        return (!m_indexByUid.isEmpty () ? m_indexByUid.value (uid, Q_NULLPTR) : Q_NULLPTR);
    }
    int roleForName (const QByteArray & name) const Q_DECL_FINAL {
        return m_roles.key (name, -1);
    }
	Q_INVOKABLE int count (void) const Q_DECL_FINAL {
        return m_count;
    }
	Q_INVOKABLE int size (void) const Q_DECL_FINAL {
        return m_count;
    }
	Q_INVOKABLE bool isEmpty (void) const Q_DECL_FINAL {
        return m_items.isEmpty ();
    }
	Q_INVOKABLE bool contains (ItemType * item) const {
        return m_items.contains (item);
    }
	Q_INVOKABLE int indexOf (ItemType * item) const {
        return m_items.indexOf (item);
    }
	Q_INVOKABLE void clear (void) Q_DECL_FINAL {
        if (!m_items.isEmpty ()) {
			QList<ItemType*> tempList;
			for (int i = 0; i < m_items.count(); ++i)
				itemAboutToBeRemoved(m_items.at(i), i);
            beginRemoveRows (noParent (), 0, m_items.count () -1);
            FOREACH_PTR_IN_QLIST (ItemType, item, m_items) {
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
	Q_INVOKABLE void append (ItemType * item) {
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
	Q_INVOKABLE void prepend (ItemType * item) {
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
	Q_INVOKABLE void insert (int idx, ItemType * item) {
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
	Q_INVOKABLE void append (const QList<ItemType *> & itemList) {
        if (!itemList.isEmpty ()) {
            const int pos = m_items.count ();
			for(int i = 0; i < itemList.count(); ++i)
				itemAboutToBeInserted(itemList.at(i), i + pos);

            beginInsertRows (noParent (), pos, pos + itemList.count () -1);
            m_items.reserve (m_items.count () + itemList.count ());
            m_items.append (itemList);
            FOREACH_PTR_IN_QLIST (ItemType, item, itemList) {
                referenceItem (item);
            }
            updateCounter ();
            endInsertRows ();
			for (int i = 0; i < itemList.count(); ++i)
				itemInserted(itemList.at(i), i + pos);
        }
    }
	Q_INVOKABLE void prepend (const QList<ItemType *> & itemList) {
        if (!itemList.isEmpty ()) {
			for (int i = 0; i < itemList.count(); ++i)
				itemAboutToBeInserted(itemList.at(i), i);
            beginInsertRows (noParent (), 0, itemList.count () -1);
            m_items.reserve (m_items.count () + itemList.count ());
            int offset = 0;
            FOREACH_PTR_IN_QLIST (ItemType, item, itemList) {
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
	Q_INVOKABLE void insert (int idx, const QList<ItemType *> & itemList) {
        if (!itemList.isEmpty ()) {
			for (int i = 0; i < itemList.count(); ++i)
				itemAboutToBeInserted(itemList.at(i), i + idx);
            beginInsertRows (noParent (), idx, idx + itemList.count () -1);
            m_items.reserve (m_items.count () + itemList.count ());
            int offset = 0;
            FOREACH_PTR_IN_QLIST (ItemType, item, itemList) {
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
	Q_INVOKABLE void move (int idx, int pos) Q_DECL_FINAL {
        if (idx != pos && idx >=0 && pos>=0 && idx < m_items.size() && pos < m_items.size()) {
			itemAboutToBeMoved(m_items.at(idx), idx, pos);
            beginMoveRows (noParent (), idx, idx, noParent (), (idx < pos ? pos +1 : pos));
            m_items.move (idx, pos);
            endMoveRows ();
			itemMoved(m_items.at(idx), idx, pos);
        }
    }
	Q_INVOKABLE void remove (ItemType * item) {
        if (item != Q_NULLPTR) {
            const int idx = m_items.indexOf (item);
            remove (idx);
        }
    }
	Q_INVOKABLE void remove (int idx) Q_DECL_FINAL {
        if (idx >= 0 && idx < m_items.size ()) {
			itemAboutToBeRemoved(m_items.at(idx), idx);
            beginRemoveRows (noParent (), idx, idx);
            ItemType * item = m_items.takeAt (idx);
            dereferenceItem (item);
            updateCounter ();
            endRemoveRows ();
			itemRemoved(item, idx);
        }
    }
    ItemType * first (void) const {
        return m_items.first ();
    }
    ItemType * last (void) const {
        return m_items.last ();
    }
    const QList<ItemType *> & toList (void) const {
        return m_items;
    }

public: // QML slots implementation
    void append (QObject * item) Q_DECL_FINAL {
        append (qobject_cast<ItemType *> (item));
    }
    void prepend (QObject * item) Q_DECL_FINAL {
        prepend (qobject_cast<ItemType *> (item));
    }
    void insert (int idx, QObject * item) Q_DECL_FINAL {
        insert (idx, qobject_cast<ItemType *> (item));
    }
    void remove (QObject * item) Q_DECL_FINAL {
        remove (qobject_cast<ItemType *> (item));
    }
    bool contains (QObject * item) const Q_DECL_FINAL {
        return contains (qobject_cast<ItemType *> (item));
    }
    int indexOf (QObject * item) const Q_DECL_FINAL {
        return indexOf (qobject_cast<ItemType *> (item));
    }
    int indexOf (const QString & uid) const {
        return indexOf (get (uid));
    }
    QObject * get (int idx) const Q_DECL_FINAL {
        return static_cast<QObject *> (at (idx));
    }
    QObject * get (const QString & uid) const Q_DECL_FINAL {
        return static_cast<QObject *> (getByUid (uid));
    }
    QObject * getFirst (void) const Q_DECL_FINAL {
        return static_cast<QObject *> (first ());
    }
    QObject * getLast (void) const Q_DECL_FINAL {
        return static_cast<QObject *> (last ());
    }
    QVariantList toVarArray (void) const Q_DECL_FINAL {
        return qListToVariant<ItemType *> (m_items);
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
    void referenceItem (ItemType * item) {
        if (item != Q_NULLPTR) {
            if (!item->parent ()) {
                item->setParent (this);
            }
            for (QHash<int, int>::const_iterator it = m_signalIdxToRole.constBegin (); it != m_signalIdxToRole.constEnd (); ++it) {
				connect(item, item->metaObject()->method(it.key()), this, m_handler, Qt::UniqueConnection);
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
    void dereferenceItem (ItemType * item) {
        if (item != Q_NULLPTR) {
            disconnect (this, Q_NULLPTR, item, Q_NULLPTR);
            disconnect (item, Q_NULLPTR, this, Q_NULLPTR);
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
        ItemType * item = qobject_cast<ItemType *> (sender ());
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
	void itemAboutToBeInserted(ItemType* item, int row) { onItemAboutToBeInserted(item, row); _onItemAboutToBeInserted(item, row); }
	void itemInserted(ItemType* item, int row) { onItemInserted(item, row); _onItemInserted(item, row);	}
	void itemAboutToBeMoved(ItemType* item, int src, int dest) { onItemAboutToBeMoved(item, src, dest); _onItemAboutToBeMoved(item, src, dest);	}
	void itemMoved(ItemType* item, int src, int dest) { onItemMoved(item, src, dest); _onItemMoved(item, src, dest); }
	void itemAboutToBeRemoved(ItemType* item, int row) { onItemAboutToBeRemoved(item, row); _onItemAboutToBeRemoved(item, row); }
	void itemRemoved(ItemType* item, int row) { onItemRemoved(item, row); _onItemRemoved(item, row); }

protected:
	virtual void onItemAboutToBeInserted(ItemType* item, int row) { }
	virtual void onItemInserted(ItemType* item, int row) { }
	virtual void onItemAboutToBeMoved(ItemType* item, int src, int dest) { }
	virtual void onItemMoved(ItemType* item, int src, int dest) { }
	virtual void onItemAboutToBeRemoved(ItemType* item, int row) { }
	virtual void onItemRemoved(ItemType* item, int row) { }

private:
	void _onItemAboutToBeInserted(ItemType* item, int row) { emit QQmlObjectListModelBase::itemAboutToBeInserted(item, row); }
	void _onItemInserted(ItemType* item, int row) { emit QQmlObjectListModelBase::itemInserted(item, row); }
	void _onItemAboutToBeMoved(ItemType* item, int src, int dest) { emit QQmlObjectListModelBase::itemAboutToBeMoved(item, src, dest); }
	void _onItemMoved(ItemType* item, int src, int dest) { emit QQmlObjectListModelBase::itemMoved(item, src, dest); }
	void _onItemAboutToBeRemoved(ItemType* item, int row) { emit QQmlObjectListModelBase::itemAboutToBeRemoved(item, row); }
	void _onItemRemoved(ItemType* item, int row) { emit QQmlObjectListModelBase::itemRemoved(item, row); }

public:
	Q_INVOKABLE void Append() override { append(new ItemType(this)); }
	Q_INVOKABLE void Append(ItemType* dict) { append(dict); }
	Q_INVOKABLE void Remove(ItemType* dict) { remove(dict); }
	Q_INVOKABLE void Remove(const int row) override	final { remove(row); }

	Q_INVOKABLE void Clear() override final { clear(); }
	Q_INVOKABLE void Insert(const int src) override final { insert(src, new ItemType(this)); }
	Q_INVOKABLE void Move(const int row, const int newRow) override { move(row, newRow); }
	Q_INVOKABLE int Count() const final { return count(); }
	Q_INVOKABLE int Size() const final { return size(); }

	/** Move row to row-1 */
	Q_INVOKABLE void MoveUp(const int row) override final
    {
		if (row > 0 && row < count())
			move(row, row - 1);
    }

	/** Move row to row+1 */
	Q_INVOKABLE void MoveDown(const int row) override final
    {
		if (count() && // There is a least one entry
			row >= 0 && // We can be from the first
			row < (count() - 1) // To the last one minus 1
			)
		{
			return MoveUp(row + 1);
		}
    }

	Q_INVOKABLE QObject* At(const int row) override final
	{
		return row >= 0 && row < m_items.size() ? m_items.at(row) : nullptr;
	}

private: // data members
    int                        m_count;
    QByteArray                 m_uidRoleName;
    QByteArray                 m_dispRoleName;
    QMetaObject                m_metaObj;
    QMetaMethod                m_handler;
    QHash<int, QByteArray>     m_roles;
    QHash<int, int>            m_signalIdxToRole;
    QList<ItemType *>          m_items;
    QHash<QString, ItemType *> m_indexByUid;
};

#define QQMLMODEL_OBJ_PROPERTY(type, name, Name) \
	QQMLMODEL_OBJ_PROPERTY_SUB(QQMLMODEL_NAMESPACE_NAME::QQmlObjectListModel<type>, name, Name)

#define QQMLMODEL_OBJ_PROPERTY_SUB(type, name, Name) \
    protected: Q_PROPERTY (QQMLMODEL_NAMESPACE_NAME::QQmlObjectListModelBase * name READ Get##Name CONSTANT) \
    private: type * _##name = new type(this); \
    public: type * Get##Name (void) const { return _##name; } \
    private:

#define QQMLMODEL_OBJ_PROPERTY_SUB_NO_T(type, name, Name) \
    protected: Q_PROPERTY (type * name READ Get##Name CONSTANT) \
    private: type * _##name = new type(this); \
    public: type * Get##Name (void) const { return _##name; } \
    private:

/**
 * \internal
 */
class _Test_TestObj : public QObject
{
	Q_OBJECT
public:
	_Test_TestObj(QObject* parent = nullptr) : QObject(parent) {}
};

/**
 * \internal
 */
class _Test_QmlModelObj : public QObject
{
	Q_OBJECT
	QQMLMODEL_OBJ_PROPERTY(_Test_TestObj, myObject, MyObject);
};

QQMLMODEL_NAMESPACE_END

#endif // QQMLOBJECTLISTMODEL_H
