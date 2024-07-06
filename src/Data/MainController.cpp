#include "MainController.h"
#include <QDir>

MainController* MainController::m_pMainController = NULL;
Tables MainController::m_tables = Tables();
TreeModel* MainController::m_pModel = nullptr;

MainController::~MainController()
{
    delete m_pModel;
}

const MainController* MainController::GetMainController() 
{
    if ( m_pMainController != NULL )
        return m_pMainController;
    m_pMainController = new MainController();

    QString path = QDir::currentPath() + "/tablesFromDb.txt";
    if ( QFileInfo::exists( path ) && QFileInfo( path ).isFile() )
    {
        Parser parser( path.toStdString() ); // p.Print();
        m_tables = parser.ParseTables();
        m_pModel = new TreeModel( &m_tables );
    }
    else
        std::cerr << "\n file does not exist \n";

    return m_pMainController;
  
}

Tables& MainController::GetTables()
{
    return m_tables;
}

TreeModel* MainController::GetModel()
{
    return m_pModel;
}

void MainController::UpdateModel()
{
    // delete m_pModel;
    m_pModel = new TreeModel( &m_tables );
}

