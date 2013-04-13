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
	static const QString FILE_SHOWS;
	static const QString FILE_HISTORY;
	static const QString FILE_FUTURE;
	static const QString FILE_PONG;

	static const QString SICKBEARD_URL;
	static const QString SICKBEARD_API_KEY;
	static const QString SICKBEARD_PORT;


	virtual ~SettingsManager();
	static SettingsManager& instance();

	// Reads information from application settings file (data/settings.conf)
	Q_INVOKABLE QString getSettingValue(const QString &strSettingName, const QString &strDefaultValue);

	// Updates information in application settings file (data/settings.conf) based
	// on given setting name and setting value parameters
	Q_INVOKABLE void updateSettingValue(const QString &strSettingName, const QString &value);

	// Check if required settings are set
	bool validSettings();

	// private methods
private:
		SettingsManager();

};

#endif // SETTINGSMANAGER_HPP
