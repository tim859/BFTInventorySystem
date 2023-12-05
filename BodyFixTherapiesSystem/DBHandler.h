#pragma once
#include <QtSql/qsqlquery.h>
#include <QtSql/qsqldatabase.h>
#include <qsettings.h>

class Herb;
class Formula;

/// <summary>
/// Singleton class responsible for all interaction with the database. Use DBHandler::GetInstance() to retrieve the sole instance of this class to be able to use its functions.
/// </summary>
class DBHandler
{
private:
	static DBHandler* instance;
	static std::string connName;
	QSqlDatabase db;
	QSettings settings;
	DBHandler();
	~DBHandler();
	std::string GetHerbsAndAmountsAsDBString(std::vector<Herb>* listOfHerbs, std::vector<int> listofHerbAmounts);

public:
	static DBHandler& GetInstance();
	void UpdateDatabase();

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