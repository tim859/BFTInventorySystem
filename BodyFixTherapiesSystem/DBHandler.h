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
	static QString ReadDBPathFromConfig();
	QSqlDatabase db;
	DBHandler(std::string connName);
	~DBHandler();
	std::string GetHerbsAndAmountsAsDBString(std::vector<Herb>* listOfHerbs, std::vector<int> listofHerbAmounts);

public:
	static DBHandler& GetInstance();

	// herb functions
	QSqlQuery GetAllHerbsFromDB();
	bool AddHerbToDB(Herb newHerb);
	bool EditHerbInDB(Herb editedHerb);
	bool DeleteHerbFromDB(int rowID);
	int GetRowIDOfLastRecordInHerbTable();

	// formula functions
	QSqlQuery GetAllFormulasFromDB();
	QSqlQuery GetHerbFromDB(int rowID);
	bool AddFormulaToDB(Formula newFormula);
	bool EditFormulaInDB(Formula editedFormula);
	bool DeleteFormulaFromDB(int rowID);
	int GetRowIDOfLastRecordInFormulaTable();
};