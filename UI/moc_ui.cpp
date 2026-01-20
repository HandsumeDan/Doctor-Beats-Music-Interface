/****************************************************************************
** Meta object code from reading C++ file 'ui.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.13)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "ui.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ui.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.13. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_viewInstrumentsList_t {
    QByteArrayData data[1];
    char stringdata0[20];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_viewInstrumentsList_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_viewInstrumentsList_t qt_meta_stringdata_viewInstrumentsList = {
    {
QT_MOC_LITERAL(0, 0, 19) // "viewInstrumentsList"

    },
    "viewInstrumentsList"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_viewInstrumentsList[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void viewInstrumentsList::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    (void)_o;
    (void)_id;
    (void)_c;
    (void)_a;
}

QT_INIT_METAOBJECT const QMetaObject viewInstrumentsList::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_viewInstrumentsList.data,
    qt_meta_data_viewInstrumentsList,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *viewInstrumentsList::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *viewInstrumentsList::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_viewInstrumentsList.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int viewInstrumentsList::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_writeDemoAndBlankFiles_t {
    QByteArrayData data[9];
    char stringdata0[223];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_writeDemoAndBlankFiles_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_writeDemoAndBlankFiles_t qt_meta_stringdata_writeDemoAndBlankFiles = {
    {
QT_MOC_LITERAL(0, 0, 22), // "writeDemoAndBlankFiles"
QT_MOC_LITERAL(1, 23, 23), // "write_blank_drums_score"
QT_MOC_LITERAL(2, 47, 0), // ""
QT_MOC_LITERAL(3, 48, 22), // "write_demo_drums_score"
QT_MOC_LITERAL(4, 71, 23), // "write_blank_music_score"
QT_MOC_LITERAL(5, 95, 30), // "write_blank_simple_drums_score"
QT_MOC_LITERAL(6, 126, 29), // "write_demo_simple_drums_score"
QT_MOC_LITERAL(7, 156, 33), // "write_blank_misc_percussion_s..."
QT_MOC_LITERAL(8, 190, 32) // "write_demo_misc_percussion_score"

    },
    "writeDemoAndBlankFiles\0write_blank_drums_score\0"
    "\0write_demo_drums_score\0write_blank_music_score\0"
    "write_blank_simple_drums_score\0"
    "write_demo_simple_drums_score\0"
    "write_blank_misc_percussion_score\0"
    "write_demo_misc_percussion_score"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_writeDemoAndBlankFiles[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x0a /* Public */,
       3,    0,   50,    2, 0x0a /* Public */,
       4,    0,   51,    2, 0x0a /* Public */,
       5,    0,   52,    2, 0x0a /* Public */,
       6,    0,   53,    2, 0x0a /* Public */,
       7,    0,   54,    2, 0x0a /* Public */,
       8,    0,   55,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void writeDemoAndBlankFiles::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<writeDemoAndBlankFiles *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->write_blank_drums_score(); break;
        case 1: _t->write_demo_drums_score(); break;
        case 2: _t->write_blank_music_score(); break;
        case 3: _t->write_blank_simple_drums_score(); break;
        case 4: _t->write_demo_simple_drums_score(); break;
        case 5: _t->write_blank_misc_percussion_score(); break;
        case 6: _t->write_demo_misc_percussion_score(); break;
        default: ;
        }
    }
    (void)_a;
}

QT_INIT_METAOBJECT const QMetaObject writeDemoAndBlankFiles::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_writeDemoAndBlankFiles.data,
    qt_meta_data_writeDemoAndBlankFiles,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *writeDemoAndBlankFiles::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *writeDemoAndBlankFiles::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_writeDemoAndBlankFiles.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int writeDemoAndBlankFiles::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}
struct qt_meta_stringdata_UI_t {
    QByteArrayData data[31];
    char stringdata0[460];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_UI_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_UI_t qt_meta_stringdata_UI = {
    {
QT_MOC_LITERAL(0, 0, 2), // "UI"
QT_MOC_LITERAL(1, 3, 21), // "generateFilterCommand"
QT_MOC_LITERAL(2, 25, 6), // "string"
QT_MOC_LITERAL(3, 32, 0), // ""
QT_MOC_LITERAL(4, 33, 8), // "tabIndex"
QT_MOC_LITERAL(5, 42, 23), // "get_instrument_category"
QT_MOC_LITERAL(6, 66, 13), // "theInstrument"
QT_MOC_LITERAL(7, 80, 20), // "get_instrument_index"
QT_MOC_LITERAL(8, 101, 11), // "theCategory"
QT_MOC_LITERAL(9, 113, 25), // "launchviewInstrumentsList"
QT_MOC_LITERAL(10, 139, 28), // "launchwriteDemoAndBlankFiles"
QT_MOC_LITERAL(11, 168, 18), // "loadFilterComboBox"
QT_MOC_LITERAL(12, 187, 5), // "index"
QT_MOC_LITERAL(13, 193, 15), // "loadFilterInput"
QT_MOC_LITERAL(14, 209, 22), // "loadInstrumentComboBox"
QT_MOC_LITERAL(15, 232, 8), // "catIndex"
QT_MOC_LITERAL(16, 241, 19), // "loadInstrumentLabel"
QT_MOC_LITERAL(17, 261, 8), // "loadSong"
QT_MOC_LITERAL(18, 270, 11), // "addSoundTab"
QT_MOC_LITERAL(19, 282, 14), // "removeSoundTab"
QT_MOC_LITERAL(20, 297, 11), // "loadHistory"
QT_MOC_LITERAL(21, 309, 14), // "open_song_file"
QT_MOC_LITERAL(22, 324, 15), // "open_sound_file"
QT_MOC_LITERAL(23, 340, 17), // "readFilterCommand"
QT_MOC_LITERAL(24, 358, 13), // "filterCommand"
QT_MOC_LITERAL(25, 372, 20), // "read_song_input_file"
QT_MOC_LITERAL(26, 393, 6), // "inFile"
QT_MOC_LITERAL(27, 400, 12), // "record_audio"
QT_MOC_LITERAL(28, 413, 17), // "updateHistoryFile"
QT_MOC_LITERAL(29, 431, 6), // "songNm"
QT_MOC_LITERAL(30, 438, 21) // "write_song_input_file"

    },
    "UI\0generateFilterCommand\0string\0\0"
    "tabIndex\0get_instrument_category\0"
    "theInstrument\0get_instrument_index\0"
    "theCategory\0launchviewInstrumentsList\0"
    "launchwriteDemoAndBlankFiles\0"
    "loadFilterComboBox\0index\0loadFilterInput\0"
    "loadInstrumentComboBox\0catIndex\0"
    "loadInstrumentLabel\0loadSong\0addSoundTab\0"
    "removeSoundTab\0loadHistory\0open_song_file\0"
    "open_sound_file\0readFilterCommand\0"
    "filterCommand\0read_song_input_file\0"
    "inFile\0record_audio\0updateHistoryFile\0"
    "songNm\0write_song_input_file"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_UI[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      21,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,  119,    3, 0x0a /* Public */,
       5,    1,  122,    3, 0x0a /* Public */,
       7,    2,  125,    3, 0x0a /* Public */,
       9,    0,  130,    3, 0x0a /* Public */,
      10,    0,  131,    3, 0x0a /* Public */,
      11,    1,  132,    3, 0x0a /* Public */,
      13,    1,  135,    3, 0x0a /* Public */,
      14,    1,  138,    3, 0x0a /* Public */,
      14,    2,  141,    3, 0x0a /* Public */,
      16,    1,  146,    3, 0x0a /* Public */,
      17,    0,  149,    3, 0x0a /* Public */,
      18,    0,  150,    3, 0x0a /* Public */,
      19,    0,  151,    3, 0x0a /* Public */,
      20,    0,  152,    3, 0x0a /* Public */,
      21,    0,  153,    3, 0x0a /* Public */,
      22,    0,  154,    3, 0x0a /* Public */,
      23,    2,  155,    3, 0x0a /* Public */,
      25,    1,  160,    3, 0x0a /* Public */,
      27,    0,  163,    3, 0x0a /* Public */,
      28,    1,  164,    3, 0x0a /* Public */,
      30,    0,  167,    3, 0x0a /* Public */,

 // slots: parameters
    0x80000000 | 2, QMetaType::Int,    4,
    0x80000000 | 2, 0x80000000 | 2,    6,
    QMetaType::Int, 0x80000000 | 2, 0x80000000 | 2,    6,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   12,
    QMetaType::Void, QMetaType::Int,   12,
    QMetaType::Void, QMetaType::Int,   12,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   15,    4,
    QMetaType::Void, QMetaType::Int,   12,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 2, QMetaType::Int,   24,    4,
    QMetaType::Void, 0x80000000 | 2,   26,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 2,   29,
    QMetaType::Void,

       0        // eod
};

void UI::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<UI *>(_o);
        (void)_t;
        switch (_id) {
        case 0: { string _r = _t->generateFilterCommand((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< string*>(_a[0]) = std::move(_r); }  break;
        case 1: { string _r = _t->get_instrument_category((*reinterpret_cast< string(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< string*>(_a[0]) = std::move(_r); }  break;
        case 2: { int _r = _t->get_instrument_index((*reinterpret_cast< string(*)>(_a[1])),(*reinterpret_cast< string(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 3: _t->launchviewInstrumentsList(); break;
        case 4: _t->launchwriteDemoAndBlankFiles(); break;
        case 5: _t->loadFilterComboBox((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->loadFilterInput((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->loadInstrumentComboBox((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->loadInstrumentComboBox((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 9: _t->loadInstrumentLabel((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->loadSong(); break;
        case 11: _t->addSoundTab(); break;
        case 12: _t->removeSoundTab(); break;
        case 13: _t->loadHistory(); break;
        case 14: _t->open_song_file(); break;
        case 15: _t->open_sound_file(); break;
        case 16: _t->readFilterCommand((*reinterpret_cast< string(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 17: _t->read_song_input_file((*reinterpret_cast< string(*)>(_a[1]))); break;
        case 18: _t->record_audio(); break;
        case 19: _t->updateHistoryFile((*reinterpret_cast< string(*)>(_a[1]))); break;
        case 20: _t->write_song_input_file(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject UI::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_UI.data,
    qt_meta_data_UI,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *UI::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *UI::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_UI.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int UI::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 21)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 21;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 21)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 21;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
