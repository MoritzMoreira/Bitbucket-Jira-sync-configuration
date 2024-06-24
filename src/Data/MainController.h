#pragma once

#include "Parser.h"
#include "TreeModel.h"

class MainController
{
public:
    static const MainController* GetMainController();
    static Tables& GetTables();
    static TreeModel* GetModel();
    static void UpdateModel();
    MainController( const MainController& ) = delete;
    void operator=( const MainController& ) = delete;
    ~MainController();

private:
    MainController()
    {}

    static MainController* m_pMainController;
    static Tables m_tables;
    static TreeModel* m_pModel;
};
