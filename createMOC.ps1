$DEFINES  = "-DUNICODE -D_UNICODE -DWIN32 -DMINGW_HAS_SECURE_API=1 -DQT_DEPRECATED_WARNINGS -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_CORE_LIB -DQT_NEEDS_QMAIN"

$COMMAND = "C:\Qt\Qt5.14.2\5.14.2\mingw73_32\bin\moc.exe"
$ARG1 = "$DEFINES --include C:/Users/weiya/Downloads/SkySide3gby_Net/SkySide3gby_Net/debug/moc_predefs.h -IC:/Qt/Qt5.14.2/5.14.2/mingw73_32/mkspecs/win32-g++ -IC:/Users/weiya/Downloads/SkySide3gby_Net/SkySide3gby_Net -IC:/Qt/Qt5.14.2/5.14.2/mingw73_32/include -IC:/Qt/Qt5.14.2/5.14.2/mingw73_32/include/QtWidgets -IC:/Qt/Qt5.14.2/5.14.2/mingw73_32/include/QtGui -IC:/Qt/Qt5.14.2/5.14.2/mingw73_32/include/QtANGLE -IC:/Qt/Qt5.14.2/5.14.2/mingw73_32/include/QtNetwork -IC:/Qt/Qt5.14.2/5.14.2/mingw73_32/include/QtCore  -IC:/Qt/Qt5.14.2/Tools/mingw730_32/lib/gcc/i686-w64-mingw32/7.3.0/include/c++ -IC:/Qt/Qt5.14.2/Tools/mingw730_32/lib/gcc/i686-w64-mingw32/7.3.0/include/c++/i686-w64-mingw32 -IC:/Qt/Qt5.14.2/Tools/mingw730_32/7.3.0/include/c++/backward -IC:/Qt/Qt5.14.2/Tools/mingw730_32/7.3.0/include -IC:/Qt/Qt5.14.2/Tools/mingw730_32/7.3.0/include-fixed -IC:/Qt/Qt5.14.2/Tools/mingw730_32/i686-w64-mingw32/include"

#$ARG2 "mainwindow.h -o moc\moc_mainwindow.cpp"

# Use the call operator (&) to execute the command
# Command for windowinidialog.h
& $COMMAND $ARG1 "windowinidialog.h" -o "moc\moc_windowinidialog.cpp"

# Command for aboutdialog.h
& $COMMAND $ARG1 "aboutdialog.h" -o "moc\moc_aboutdialog.cpp"

# Command for keyinidialog.h
& $COMMAND $ARG1 "keyinidialog.h" -o "moc\moc_keyinidialog.cpp"

# Command for logindialog.h
& $COMMAND $ARG1 "logindialog.h" -o "moc\moc_logindialog.cpp"

# Command for mainwindow.h
& $COMMAND $ARG1 "mainwindow.h" -o "moc\moc_mainwindow.cpp"


