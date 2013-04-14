#include "QuotesController.hpp"
#include "RESTClient.hpp"
#include <bb/data/JsonDataAccess>

using namespace bb::data;

QuotesController::QuotesController(QObject* parent)
: bb::cascades::ArrayDataModel()
{
    setParent(parent);
    qDebug() << "Creating QuotesController object:" << this;
}

QuotesController::~QuotesController()
{
    qDebug() << "Destroying QuotesController object:" << this;
}

void QuotesController::load(const QString& file_name)
{
    // clear model
    clear();
    // clear not filtered data cache
    m_data.clear();
    {
        JsonDataAccess jda;
        QVariantMap root = jda.load(file_name).toMap();

        if (jda.hasError()) {
            DataAccessError error = jda.error();
            qDebug() << file_name << "JSON loading error: " << error.errorType() << ": " << error.errorMessage();
        }
        else {
            qDebug() << file_name << "JSON data loaded OK!";
            // when loaded, show all data (no filtering)
            m_data = root.value("data").toList();
            append(m_data);
        }
    }
}

QVariant QuotesController::getRandomQuote()
{
/*
	if (!m_data.isEmpty()) {
		return m_data.at(qrand() % m_data.size());
	}
	return 0;
*/
	QVariantMap map;
	map["quote"] = "\"This is a random quote!\"";
	map["author"] = "Rob Woods";
	return map;
}
