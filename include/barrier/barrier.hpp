#pragma once

#include "barrier/common.hpp"

// This namespace is holding header parts of barrier classes. Barrier class
// object is in fact object from other class inside memgraph. Main purpose of
// barrier classes is to hide real classes from outside viewers. This header
// should be only to be included from memgraph into generated/compiled query.
// And this header must ensure that it defines all classes and methods required
// for querys.
namespace barrier
{
// Every class which must be visible to outside the barrier should have there
// barrier class defined here.

// ************ Here should be forward declarations of Sized barrier classes
// ACCESSORS
class DbAccessor;
class VertexAccessor;
class EdgeAccessor;

// GENERIC ITERATORS
class VertexIterator;
class EdgeIterator;

// TYPED ITERATORS
class VertexAccessIterator;
class OutEdgesIterator;
class InEdgesIterator;

// PROPERTY
class VertexPropertyKey;
class EdgePropertyKey;
template <class T>
class VertexPropertyType;
template <class T>
class EdgePropertyType;

// BOLT
template <class Stream>
class BoltSerializer;

// ************ Here should be forward declarations of Unsized barrier classes
// COMMON
class Db;
class Label;
class EdgeType;

// GENERIC INDEXES
template <class K>
class VertexIndex;
template <class K>
class EdgeIndex;

// PROPERTY
class VertexPropertyFamily;
class EdgePropertyFamily;

// ************* Here should be defined usings
using label_ref_t = ReferenceWrapper<const Label>;

// ************ Here should be definitions of Sized barrier classes
// Original class should have Sized barrier class if it can't be Unsized.
// Sized barrier classes should:
// --Have same name as original class.
// --Inherit Sized class from common.hpp as private. Blueprint:
//      class class_name: Sized<size_of_t,aligment_of_t>
// --Sized template arguments must be hardcoded numbers equal to sizeof(T) and
//   alignof(T) where T is original class.
// --It should have undefined public constructor which is specialized in .cpp
//   Blueprint:
//   public:
//      template<class T> class_name(T &&d);
// --It should delete or specify following methods depending on the behavior of
//   the original class:
//   public:
//      class_name(const class_name &other);
//      class_name(class_name &&other);
//      ~class_name();
//      class_name &operator=(const class_name &other);
//      class_name &operator=(class_name &&other);
// --It should specify public methods which can be called on the original class.
//
// Blueprint:
// class class_name : private Sized<,>
// {
// public:
//     template <class T>
//     class_name(T &&d);
//
//     class_name(const class_name &other);
//     class_name(class_name &&other);
//     ~class_name();
//
//     class_name &operator=(const class_name &other);
//     class_name &operator=(class_name &&other);
//
// };

// TODO: Extract methods od RecordAccessor for VertexAccessor and EdgeAccessor
// into joined class.
class VertexAccessor : private Sized<24, 8>
{
public:
    template <class T>
    VertexAccessor(T &&d);

    VertexAccessor(VertexAccessor &other);
    VertexAccessor(const VertexAccessor &other);
    VertexAccessor(VertexAccessor &&other);
    VertexAccessor(VertexAccessor const &&other);
    ~VertexAccessor();

    VertexAccessor &operator=(const VertexAccessor &other);
    VertexAccessor &operator=(VertexAccessor &&other);

    size_t out_degree() const;

    size_t in_degree() const;

    size_t degree() const;

    bool add_label(const Label &label);

    bool remove_label(const Label &label);

    bool has_label(const Label &label) const;

    const std::vector<label_ref_t> &labels() const;

    OutEdgesIterator out() const;

    InEdgesIterator in() const;

    bool in_contains(VertexAccessor const &other) const;

    bool empty() const;

    bool fill() const;

    const Id &id() const;

    VertexAccessor update() const;

    bool remove() const;

    const Property &at(VertexPropertyFamily &key) const;

    const Property &at(VertexPropertyKey &key) const;

    template <class V>
    OptionPtr<V> at(VertexPropertyType<V> &key) const;

    template <class V, class... Args>
    void set(VertexPropertyType<V> &key, Args &&... args);

    void set(VertexPropertyKey &key, Property::sptr value);

    void clear(VertexPropertyKey &key);

    void clear(VertexPropertyFamily &key);

    template <class Handler>
    void accept(Handler &handler) const;

    explicit operator bool() const;

    friend bool operator==(const VertexAccessor &a, const VertexAccessor &b);

    friend bool operator!=(const VertexAccessor &a, const VertexAccessor &b);
};

class EdgeAccessor : private Sized<24, 8>
{
public:
    template <class T>
    EdgeAccessor(T &&d);

    EdgeAccessor(EdgeAccessor &other);
    EdgeAccessor(const EdgeAccessor &other);
    EdgeAccessor(EdgeAccessor &&other);
    EdgeAccessor(EdgeAccessor const &&other);
    ~EdgeAccessor();

    EdgeAccessor &operator=(const EdgeAccessor &other);
    EdgeAccessor &operator=(EdgeAccessor &&other);

    void edge_type(const EdgeType &edge_type);

    const EdgeType &edge_type() const;

    const VertexAccessor from() const;

    const VertexAccessor to() const;

    bool empty() const;

    bool fill() const;

    const Id &id() const;

    EdgeAccessor update() const;

    bool remove() const;

    const Property &at(EdgePropertyFamily &key) const;

    const Property &at(EdgePropertyKey &key) const;

    template <class V>
    OptionPtr<V> at(EdgePropertyType<V> &key) const;

    template <class V, class... Args>
    void set(EdgePropertyType<V> &key, Args &&... args);

    void set(EdgePropertyKey &key, Property::sptr value);

    void clear(EdgePropertyKey &key);

    void clear(EdgePropertyFamily &key);

    template <class Handler>
    void accept(Handler &handler) const;

    explicit operator bool() const;

    friend bool operator==(const EdgeAccessor &a, const EdgeAccessor &b);

    friend bool operator!=(const EdgeAccessor &a, const EdgeAccessor &b);
};

class DbAccessor : protected Sized<40, 8>
{
public:
    DbAccessor(Db &d);

    DbAccessor(const DbAccessor &other) = delete;
    DbAccessor(DbAccessor &&other) = delete;
    ~DbAccessor();

    DbAccessor &operator=(const DbAccessor &other) = delete;
    DbAccessor &operator=(DbAccessor &&other) = delete;

    VertexAccessIterator vertex_access();

    Option<const VertexAccessor> vertex_find(const Id &id);

    VertexAccessor vertex_insert();

    Option<const EdgeAccessor> edge_find(const Id &id);

    EdgeAccessor edge_insert(VertexAccessor const &from,
                             VertexAccessor const &to);

    const Label &label_find_or_create(const char *name);

    bool label_contains(const char *name);

    const EdgeType &type_find_or_create(const char *name);

    bool type_contains(const char *name);

    VertexPropertyFamily &vertex_property_family_get(const std::string &name);

    EdgePropertyFamily &edge_property_family_get(const std::string &name);

    VertexPropertyKey vertex_property_key(const std::string &name, Type type);

    EdgePropertyKey edge_property_key(const std::string &name, Type type);

    template <class T>
    VertexPropertyType<T> vertex_property_key(const std::string &name);

    template <class T>
    EdgePropertyType<T> edge_property_key(const std::string &name);

    bool commit();
    void abort();
};

class VertexIterator : private Sized<8, 8>
{
public:
    template <class T>
    VertexIterator(T &&d);

    VertexIterator(const VertexIterator &other) = delete;
    VertexIterator(VertexIterator &&other);
    ~VertexIterator();
    VertexIterator &operator=(const VertexIterator &other) = delete;
    VertexIterator &operator=(VertexIterator &&other) = delete;

    Option<const VertexAccessor> next();
};

// TODO: Find reasons of such great size ant try to decrease it.
class VertexAccessIterator : private Sized<552, 8>
{
public:
    template <class T>
    VertexAccessIterator(T &&d);

    VertexAccessIterator(const VertexAccessIterator &other) = delete;
    VertexAccessIterator(VertexAccessIterator &&other);
    ~VertexAccessIterator();
    VertexAccessIterator &operator=(const VertexAccessIterator &other) = delete;
    VertexAccessIterator &operator=(VertexAccessIterator &&other) = delete;

    Option<const VertexAccessor> next();
};

class OutEdgesIterator : private Sized<40, 8>
{
public:
    template <class T>
    OutEdgesIterator(T &&d);

    OutEdgesIterator(const OutEdgesIterator &other) = delete;
    OutEdgesIterator(OutEdgesIterator &&other);
    ~OutEdgesIterator();
    OutEdgesIterator &operator=(const OutEdgesIterator &other) = delete;
    OutEdgesIterator &operator=(OutEdgesIterator &&other) = delete;

    Option<const EdgeAccessor> next();
};

class InEdgesIterator : private Sized<56, 8>
{
public:
    template <class T>
    InEdgesIterator(T &&d);

    InEdgesIterator(const InEdgesIterator &other) = delete;
    InEdgesIterator(InEdgesIterator &&other);
    ~InEdgesIterator();
    InEdgesIterator &operator=(const InEdgesIterator &other) = delete;
    InEdgesIterator &operator=(InEdgesIterator &&other) = delete;

    Option<const EdgeAccessor> next();
};

class EdgeIterator : private Sized<8, 8>
{
public:
    template <class T>
    EdgeIterator(T &&d);

    EdgeIterator(const EdgeIterator &other) = delete;
    EdgeIterator(EdgeIterator &&other);
    ~EdgeIterator();
    EdgeIterator &operator=(const EdgeIterator &other) = delete;
    EdgeIterator &operator=(EdgeIterator &&other) = delete;

    Option<const EdgeAccessor> next();
};

class VertexPropertyKey : private Sized<8, 8>
{
public:
    template <class T>
    VertexPropertyKey(T &&d);

    VertexPropertyKey(const VertexPropertyKey &other) = default;
    VertexPropertyKey(VertexPropertyKey &&other) = default;
    ~VertexPropertyKey();

    VertexPropertyKey &operator=(const VertexPropertyKey &other) = default;
    VertexPropertyKey &operator=(VertexPropertyKey &&other) = default;
};

class EdgePropertyKey : private Sized<8, 8>
{
public:
    template <class T>
    EdgePropertyKey(T &&d);

    EdgePropertyKey(const EdgePropertyKey &other) = default;
    EdgePropertyKey(EdgePropertyKey &&other) = default;
    ~EdgePropertyKey();

    EdgePropertyKey &operator=(const EdgePropertyKey &other) = default;
    EdgePropertyKey &operator=(EdgePropertyKey &&other) = default;
};

template <class K>
class VertexPropertyType : private Sized<8, 8>
{
public:
    template <class T>
    VertexPropertyType(T &&d);

    VertexPropertyType(const VertexPropertyType &other) = default;
    VertexPropertyType(VertexPropertyType &&other) = default;
    ~VertexPropertyType(){};

    VertexPropertyType &operator=(const VertexPropertyType &other) = default;
    VertexPropertyType &operator=(VertexPropertyType &&other) = default;
};

template <class K>
class EdgePropertyType : private Sized<8, 8>
{
public:
    template <class T>
    EdgePropertyType(T &&d);

    EdgePropertyType(const EdgePropertyType &other) = default;
    EdgePropertyType(EdgePropertyType &&other) = default;
    ~EdgePropertyType(){};

    EdgePropertyType &operator=(const EdgePropertyType &other) = default;
    EdgePropertyType &operator=(EdgePropertyType &&other) = default;
};

template <class Stream>
class BoltSerializer : private Sized<8, 8>
{
public:
    template <class T>
    BoltSerializer(T &&d);

    BoltSerializer(const BoltSerializer &other) = default;
    BoltSerializer(BoltSerializer &&other) = default;
    ~BoltSerializer();

    BoltSerializer &operator=(const BoltSerializer &other) = default;
    BoltSerializer &operator=(BoltSerializer &&other) = default;

    void write(const VertexAccessor &vertex);

    void write(const EdgeAccessor &edge);

    void write(const Property &prop);

    void write_null();

    void write(const Bool &prop);

    void write(const Float &prop);

    void write(const Double &prop);

    void write(const Int32 &prop);

    void write(const Int64 &prop);

    void write(const std::string &value);

    void write(const String &prop);

    template <class T>
    void handle(const T &prop);
};

// ************ Here should be definitions of Unsized barrier classes
// Original class can be Unsized barrier class if they are only used outside the
// barrier through reference/pointer.
// Usized barrier classes should:
// --Have same name as original class or somethin more specific if it is
//   partialy specialized.
// --Inherit Unsized class from common.hpp as protected. Blueprint:
//      class class_name: protected Unsized
// --It should specify public methods which can be called on the original class.
//
// Blueprint:
// class class_name : protected Unsized
// {
// public:
//
// };

class VertexPropertyFamily : protected Unsized
{
public:
    OptionPtr<VertexIndex<std::nullptr_t>> index();
};

class EdgePropertyFamily : protected Unsized
{
public:
    OptionPtr<EdgeIndex<std::nullptr_t>> index();
};

class Label : protected Unsized
{
public:
    VertexIndex<std::nullptr_t> &index() const;
};

class EdgeType : protected Unsized
{
public:
    friend bool operator<(const EdgeType &lhs, const EdgeType &rhs);

    friend bool operator==(const EdgeType &lhs, const EdgeType &rhs);

    EdgeIndex<std::nullptr_t> &index() const;
};

template <class K>
class VertexIndex : protected Unsized
{
public:
    // TODO: This iterator sometimes has known type. It can be added to this
    // border to avoid dynamic dispatch
    VertexIterator for_range(DbAccessor &, Border<K> from = Border<K>(),
                             Border<K> to = Border<K>());

    bool unique();

    Order order();
};

template <class K>
class EdgeIndex : protected Unsized
{
public:
    // TODO: This iterator has known type. It can be added to this border to
    // avoid dynamic dispatch
    EdgeIterator for_range(DbAccessor &, Border<K> from = Border<K>(),
                           Border<K> to = Border<K>());

    bool unique();

    Order order();
};

class Db : protected Unsized
{
};
}
