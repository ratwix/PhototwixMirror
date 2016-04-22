#include <QCoreApplication>
#include <QGuiApplication>
#include <QKeyEvent>
#include <QQuickItem>
#include <assert.h>
#include <iostream>
#include "keyemitter.h"

KeyEmitter::KeyEmitter()
{

}

KeyEmitter::~KeyEmitter()
{

}

KeyEmitter::keyMesage KeyEmitter::stringToKey(QString s)
{
    KeyEmitter::keyMesage res;

    if (s[0] == 'a') {res.key = Qt::Key_A; res.modifiers = Qt::NoModifier; res.text = "a";}
    if (s[0] == 'b') {res.key = Qt::Key_B; res.modifiers = Qt::NoModifier; res.text = "b";}
    if (s[0] == 'c') {res.key = Qt::Key_C; res.modifiers = Qt::NoModifier; res.text = "c";}
    if (s[0] == 'd') {res.key = Qt::Key_D; res.modifiers = Qt::NoModifier; res.text = "d";}
    if (s[0] == 'e') {res.key = Qt::Key_E; res.modifiers = Qt::NoModifier; res.text = "e";}
    if (s[0] == 'f') {res.key = Qt::Key_F; res.modifiers = Qt::NoModifier; res.text = "f";}
    if (s[0] == 'g') {res.key = Qt::Key_G; res.modifiers = Qt::NoModifier; res.text = "g";}
    if (s[0] == 'h') {res.key = Qt::Key_H; res.modifiers = Qt::NoModifier; res.text = "h";}
    if (s[0] == 'i') {res.key = Qt::Key_I; res.modifiers = Qt::NoModifier; res.text = "i";}
    if (s[0] == 'j') {res.key = Qt::Key_J; res.modifiers = Qt::NoModifier; res.text = "j";}
    if (s[0] == 'k') {res.key = Qt::Key_K; res.modifiers = Qt::NoModifier; res.text = "k";}
    if (s[0] == 'l') {res.key = Qt::Key_L; res.modifiers = Qt::NoModifier; res.text = "l";}
    if (s[0] == 'm') {res.key = Qt::Key_M; res.modifiers = Qt::NoModifier; res.text = "m";}
    if (s[0] == 'n') {res.key = Qt::Key_N; res.modifiers = Qt::NoModifier; res.text = "n";}
    if (s[0] == 'o') {res.key = Qt::Key_O; res.modifiers = Qt::NoModifier; res.text = "o";}
    if (s[0] == 'p') {res.key = Qt::Key_P; res.modifiers = Qt::NoModifier; res.text = "p";}
    if (s[0] == 'q') {res.key = Qt::Key_Q; res.modifiers = Qt::NoModifier; res.text = "q";}
    if (s[0] == 'r') {res.key = Qt::Key_R; res.modifiers = Qt::NoModifier; res.text = "r";}
    if (s[0] == 's') {res.key = Qt::Key_S; res.modifiers = Qt::NoModifier; res.text = "s";}
    if (s[0] == 't') {res.key = Qt::Key_T; res.modifiers = Qt::NoModifier; res.text = "t";}
    if (s[0] == 'u') {res.key = Qt::Key_U; res.modifiers = Qt::NoModifier; res.text = "u";}
    if (s[0] == 'v') {res.key = Qt::Key_V; res.modifiers = Qt::NoModifier; res.text = "v";}
    if (s[0] == 'w') {res.key = Qt::Key_W; res.modifiers = Qt::NoModifier; res.text = "w";}
    if (s[0] == 'x') {res.key = Qt::Key_X; res.modifiers = Qt::NoModifier; res.text = "x";}
    if (s[0] == 'y') {res.key = Qt::Key_Y; res.modifiers = Qt::NoModifier; res.text = "y";}
    if (s[0] == 'z') {res.key = Qt::Key_Z; res.modifiers = Qt::NoModifier; res.text = "z";}

    if (s[0] == 'A') {res.key = Qt::Key_A; res.modifiers = Qt::ShiftModifier; res.text = "A";}
    if (s[0] == 'B') {res.key = Qt::Key_B; res.modifiers = Qt::ShiftModifier; res.text = "B";}
    if (s[0] == 'C') {res.key = Qt::Key_C; res.modifiers = Qt::ShiftModifier; res.text = "C";}
    if (s[0] == 'D') {res.key = Qt::Key_D; res.modifiers = Qt::ShiftModifier; res.text = "D";}
    if (s[0] == 'E') {res.key = Qt::Key_E; res.modifiers = Qt::ShiftModifier; res.text = "E";}
    if (s[0] == 'F') {res.key = Qt::Key_F; res.modifiers = Qt::ShiftModifier; res.text = "F";}
    if (s[0] == 'G') {res.key = Qt::Key_G; res.modifiers = Qt::ShiftModifier; res.text = "G";}
    if (s[0] == 'H') {res.key = Qt::Key_H; res.modifiers = Qt::ShiftModifier; res.text = "H";}
    if (s[0] == 'I') {res.key = Qt::Key_I; res.modifiers = Qt::ShiftModifier; res.text = "I";}
    if (s[0] == 'J') {res.key = Qt::Key_J; res.modifiers = Qt::ShiftModifier; res.text = "J";}
    if (s[0] == 'K') {res.key = Qt::Key_K; res.modifiers = Qt::ShiftModifier; res.text = "K";}
    if (s[0] == 'L') {res.key = Qt::Key_L; res.modifiers = Qt::ShiftModifier; res.text = "L";}
    if (s[0] == 'M') {res.key = Qt::Key_M; res.modifiers = Qt::ShiftModifier; res.text = "M";}
    if (s[0] == 'N') {res.key = Qt::Key_N; res.modifiers = Qt::ShiftModifier; res.text = "N";}
    if (s[0] == 'O') {res.key = Qt::Key_O; res.modifiers = Qt::ShiftModifier; res.text = "O";}
    if (s[0] == 'P') {res.key = Qt::Key_P; res.modifiers = Qt::ShiftModifier; res.text = "P";}
    if (s[0] == 'Q') {res.key = Qt::Key_Q; res.modifiers = Qt::ShiftModifier; res.text = "Q";}
    if (s[0] == 'R') {res.key = Qt::Key_R; res.modifiers = Qt::ShiftModifier; res.text = "R";}
    if (s[0] == 'S') {res.key = Qt::Key_S; res.modifiers = Qt::ShiftModifier; res.text = "S";}
    if (s[0] == 'T') {res.key = Qt::Key_T; res.modifiers = Qt::ShiftModifier; res.text = "T";}
    if (s[0] == 'U') {res.key = Qt::Key_U; res.modifiers = Qt::ShiftModifier; res.text = "U";}
    if (s[0] == 'V') {res.key = Qt::Key_V; res.modifiers = Qt::ShiftModifier; res.text = "V";}
    if (s[0] == 'W') {res.key = Qt::Key_W; res.modifiers = Qt::ShiftModifier; res.text = "W";}
    if (s[0] == 'X') {res.key = Qt::Key_X; res.modifiers = Qt::ShiftModifier; res.text = "X";}
    if (s[0] == 'Y') {res.key = Qt::Key_Y; res.modifiers = Qt::ShiftModifier; res.text = "Y";}
    if (s[0] == 'Z') {res.key = Qt::Key_Z; res.modifiers = Qt::ShiftModifier; res.text = "Z";}

    if (s[0] == '0') {res.key = Qt::Key_0; res.modifiers = Qt::NoModifier; res.text = "0";}
    if (s[0] == '1') {res.key = Qt::Key_1; res.modifiers = Qt::NoModifier; res.text = "1";}
    if (s[0] == '2') {res.key = Qt::Key_2; res.modifiers = Qt::NoModifier; res.text = "2";}
    if (s[0] == '3') {res.key = Qt::Key_3; res.modifiers = Qt::NoModifier; res.text = "3";}
    if (s[0] == '4') {res.key = Qt::Key_4; res.modifiers = Qt::NoModifier; res.text = "4";}
    if (s[0] == '5') {res.key = Qt::Key_5; res.modifiers = Qt::NoModifier; res.text = "5";}
    if (s[0] == '6') {res.key = Qt::Key_6; res.modifiers = Qt::NoModifier; res.text = "6";}
    if (s[0] == '7') {res.key = Qt::Key_7; res.modifiers = Qt::NoModifier; res.text = "7";}
    if (s[0] == '8') {res.key = Qt::Key_8; res.modifiers = Qt::NoModifier; res.text = "8";}
    if (s[0] == '9') {res.key = Qt::Key_9; res.modifiers = Qt::NoModifier; res.text = "9";}

    if (s[0] == '!') {res.key = Qt::Key_Exclam; res.modifiers = Qt::NoModifier; res.text = "!";}
    if (s[0] == '@') {res.key = Qt::Key_At; res.modifiers = Qt::NoModifier; res.text = "@";}
    if (s[0] == '#') {res.key = Qt::Key_ssharp; res.modifiers = Qt::NoModifier; res.text = "#";}
    if (s[0] == '$') {res.key = Qt::Key_Dollar; res.modifiers = Qt::NoModifier; res.text = "$";}
    if (s[0] == '%') {res.key = Qt::Key_Percent; res.modifiers = Qt::NoModifier; res.text = "%";}
    if (s[0] == ':') {res.key = Qt::Key_QuoteDbl; res.modifiers = Qt::NoModifier; res.text = ":";}
    if (s[0] == '&') {res.key = Qt::Key_Ampersand; res.modifiers = Qt::NoModifier; res.text = "&";}
    if (s[0] == '*') {res.key = Qt::Key_Asterisk; res.modifiers = Qt::NoModifier; res.text = "*";}
    if (s[0] == '.') {res.key = Qt::Key_Period; res.modifiers = Qt::NoModifier; res.text = ".";}
    if (s[0] == ',') {res.key = Qt::Key_Colon; res.modifiers = Qt::NoModifier; res.text = ",";}
    if (s[0] == ';') {res.key = Qt::Key_Semicolon; res.modifiers = Qt::NoModifier; res.text = ";";}
    if (s[0] == '?') {res.key = Qt::Key_Question; res.modifiers = Qt::NoModifier; res.text = "?";}
    if (s[0] == '/') {res.key = Qt::Key_Slash; res.modifiers = Qt::NoModifier; res.text = "/";}
    if (s[0] == '_') {res.key = Qt::Key_Underscore; res.modifiers = Qt::NoModifier; res.text = "_";}
    if (s[0] == '"') {res.key = Qt::Key_QuoteDbl; res.modifiers = Qt::NoModifier; res.text = "\"";}
    if (s[0] == '\'') {res.key = Qt::Key_Apostrophe; res.modifiers = Qt::NoModifier; res.text = "'";}
    if (s[0] == '(') {res.key = Qt::Key_ParenLeft; res.modifiers = Qt::NoModifier; res.text = "(";}
    if (s[0] == ')') {res.key = Qt::Key_ParenRight; res.modifiers = Qt::NoModifier; res.text = ")";}
    if (s[0] == '+') {res.key = Qt::Key_Plus; res.modifiers = Qt::NoModifier; res.text = "+";}
    if (s[0] == ',') {res.key = Qt::Key_Comma   ; res.modifiers = Qt::NoModifier; res.text = ",";}
    if (s[0] == '-') {res.key = Qt::Key_Minus; res.modifiers = Qt::NoModifier; res.text = "-";}
    if (s[0] == '{') {res.key = Qt::Key_BracketLeft; res.modifiers = Qt::NoModifier; res.text = "{";}
    if (s[0] == '}') {res.key = Qt::Key_BracketRight; res.modifiers = Qt::NoModifier; res.text = "}";}
    if (s[0] == '~') {res.key = Qt::Key_AsciiTilde; res.modifiers = Qt::NoModifier; res.text = "~";}
    if (s[0] == '[') {res.key = Qt::Key_BraceLeft; res.modifiers = Qt::NoModifier; res.text = "[";}
    if (s[0] == ']') {res.key = Qt::Key_BraceRight; res.modifiers = Qt::NoModifier; res.text = "]";}
    if (s[0] == '|') {res.key = Qt::Key_Bar; res.modifiers = Qt::NoModifier; res.text = "|";}
    if (s[0] == '\\') {res.key = Qt::Key_Backslash; res.modifiers = Qt::NoModifier; res.text = "\\";}
    if (s[0] == '<') {res.key = Qt::Key_Less; res.modifiers = Qt::NoModifier; res.text = "<";}
    if (s[0] == '>') {res.key = Qt::Key_Greater; res.modifiers = Qt::NoModifier; res.text = ">";}
    if (s[0] == '=') {res.key = Qt::Key_Equal; res.modifiers = Qt::NoModifier; res.text = "=";}
    if (s[0] == ' ') {res.key = Qt::Key_Space; res.modifiers = Qt::NoModifier; res.text = " ";}
    if (s[0] == '\n') {res.key = Qt::Key_Enter; res.modifiers = Qt::NoModifier; res.text = "\n";}
    if (s[0] == '\x7F') {res.key = Qt::Key_Backspace; res.modifiers = Qt::NoModifier; res.text = "\x7F";}
    if (s[0] == '\x1B') {res.key = Qt::Key_Escape; res.modifiers = Qt::NoModifier; res.text = "\x1B";}
    return res;
}

void KeyEmitter::emitKey(KeyEmitter::keyMesage k)
{
    QQuickItem* receiver = qobject_cast<QQuickItem*>(QGuiApplication::focusObject());
    if(!receiver) {
        std::cout << "Et merde" << std::endl;
        return;
    }

    QKeyEvent* pressEvent = new QKeyEvent(QEvent::KeyPress, k.key, k.modifiers, k.text);
    QKeyEvent* releaseEvent = new QKeyEvent(QEvent::KeyRelease, k.key, k.modifiers);
    QCoreApplication::sendEvent(receiver, pressEvent);
    QCoreApplication::sendEvent(receiver, releaseEvent);

    //TODO : memory leak or managed by qml ?
}

void KeyEmitter::emitKey(QString key)
{
    emitKey(stringToKey(key));
}

