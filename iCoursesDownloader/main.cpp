#include <QtWidgets/QApplication>

#include <windows.h>
#include <io.h>
#include <fcntl.h>
#include <iostream>

#include "mainwnd.h"

void initializeConsole() {
    AllocConsole();

    HANDLE hin = ::GetStdHandle(STD_INPUT_HANDLE);
    HANDLE hout = ::GetStdHandle(STD_OUTPUT_HANDLE);

    int hcin = _open_osfhandle((intptr_t)hin, _O_TEXT);
    FILE *fpin = _fdopen(hcin, "r");
    *stdin = *_fdopen(hcin, "r");

    int hcout = _open_osfhandle((intptr_t)hout, _O_TEXT);
    *stdout = *_fdopen(hcout, "wt");
    std::ios_base::sync_with_stdio();
}

int main(int argc, char *argv[]) {
    //initializeConsole();

    QApplication a(argc, argv);

    MainWnd mainWnd;
    mainWnd.show();
    mainWnd.hide();

    return a.exec();
}
