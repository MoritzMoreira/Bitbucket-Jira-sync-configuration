#include <QWidget>
#include <qapplication.h>

#include "Data/MainController.h"
#include "Widgets/mainwindow.h"

int main( int argc, char* argv[] )
{
    QApplication app( argc, argv );
    const MainController* pMainController = MainController::GetMainController();
    pMainController->GetTables();
    MainWindow mainWindow( nullptr );
    mainWindow.show();

 /*std::ifstream stream(QDir::currentPath().toStdString()  + "/src/fields.txt");
    constexpr std::size_t readSize = 4096;
    std::string outString;
    auto buf = std::string(readSize, '\0');
    while (stream.read(&buf[0], readSize)) outString.append(buf, 0, stream.gcount());
    outString.append(buf, 0, stream.gcount());
    Parser ph(QDir::currentPath().toStdString() + "/src/fields.txt");
    ph.DeleteSpaces(outString);*/

 /*QModelIndex mi = w.GetUi()->treeView->selectionModel()->currentIndex();
    ConnectionWidget cw(w.GetTreeModel()->GetItem(mi, 0), &w);
    cw.show();*/

 /*Ui::FieldsForm ff;
    ff.setupUi(&w);*/
    return app.exec();
}

