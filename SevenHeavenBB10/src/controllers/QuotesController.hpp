#ifndef QUOTESCONTROLLER_HPP_
#define QUOTESCONTROLLER_HPP_

#include <QObject>
#include <bb/cascades/ArrayDataModel>

class QuotesController : public bb::cascades::ArrayDataModel
{
    Q_OBJECT

public:
    explicit QuotesController(QObject* parent = 0);
    virtual ~QuotesController();

public Q_SLOTS:
	void load(const QString& file_name);
	QVariant getRandomQuote();

private:
    QVariantList m_data;
};
#endif /* QUOTESCONTROLLER_HPP_ */
