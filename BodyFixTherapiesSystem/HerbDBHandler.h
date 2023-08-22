#pragma once

#include "DBHandler.h"
class Herb;

/*Class for managing and querying the herbs table in the BFT database*/
class HerbDBHandler : public DBHandler
{
public:
	QSqlQuery GetAllHerbsFromDB();
	QSqlQuery SearchDBForHerbs();
	bool AddHerbToDB(Herb newHerb);
	bool DeleteHerbFromDB();
	bool EditHerbInDB();
};

