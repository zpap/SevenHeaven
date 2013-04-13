#ifndef SETTINGSMANAGER_HPP
#define SETTINGSMANAGER_HPP

#include <QtCore/QObject>
#include <QString>

class SettingsManager : public QObject
{
	Q_OBJECT

public:

	static const QString DATA_DIR;
	static const QString SHOW_DETAILS_DIR;

	static const QString FEED_URL;


	virtual ~SettingsManager();
	static SettingsManager& instance();

	// Reads information from application settings file (data/settings.conf)
	Q_INVOKABLE QString getSettingValue(const QString &strSettingName, const QString &strDefaultValue);

	// Updates information in application settings file (data/settings.conf) based
	// on given setting name and setting value parameters
	Q_INVOKABLE void updateSettingValue(const QString &strSettingName, const QString &value);

	// private methods
private:
		SettingsManager();

};

#endif // SETTINGSMANAGER_HPP
