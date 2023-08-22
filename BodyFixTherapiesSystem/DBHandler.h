#pragma once
#include <qsqldatabase.h>
#include <qsqlquery.h>

class DBHandler
{
public:
	QSqlDatabase GetBFTDB();
};

