#pragma once
#include <qsqldatabase.h>
#include <qsqlquery.h>

class Herb;
class Formula;

/// <summary>
/// Singleton class responsible for all interaction with the database. Use DBHandler::GetInstance() to retrieve the sole instance of this class to be able to use its functions.
/// </summary>
class DBHandler
{
private:
	static DBHandler* instance;
	QSqlDatabase db;
	DBHandler(std::string connName);
	~DBHandler();

public:
	static DBHandler& GetInstance();

	// herb functions
	QSqlQuery GetAllHerbsFromDB();
	bool AddHerbToDB(Herb newHerb);
	bool EditHerbInDB(Herb editedHerb);
	bool DeleteHerbFromDB(int rowID);
	int GetRowsInHerbTable();

	// formula functions
	QSqlQuery GetAllFormulasFromDB();
	QSqlQuery GetHerbFromDB(int rowID);
	bool AddFormulaToDB(Formula newFormula);
	/*bool EditFormulaInDB();
	bool DeleteFormulaFromDB();*/
	int GetRowsInFormulaTable();
};

