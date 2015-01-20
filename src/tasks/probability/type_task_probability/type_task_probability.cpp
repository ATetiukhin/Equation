/**
* @file   type_task_probability.cpp
* @Author ATetiukhin
* @date   January, 2015
* @brief  Implementation tasks probability.
*/

#include "type_task_probability.hpp" // you cannot change the position
#include "database_helper.hpp"
#include "probability_widget.hpp"

#include <QPluginLoader>
#include "plugin_probability.hpp"

#include "plot_dialog.hpp"

#include <QDebug>

TypeTaskProbability::TypeTaskProbability(ProbabilityWidget * ui)
    : dataBase_(DatabaseHelper::getInstance())
    , ui_(*ui)
{
    QStringList namesMethods(dataBase_.getNameSample());
    if (!namesMethods.isEmpty()) {
        ui_.updateComboBox(namesMethods);
    }
}

TypeTaskProbability::~TypeTaskProbability()
{
}

void TypeTaskProbability::answerButtonClicked()
{
    try {
        int idSample = 0;
        int left = 0;
        int right = 0;
        double r = 0;
        QVector<double> sample;

        ui_.getEquationParameters(idSample, left, right, r);
        dataBase_.sample(sample, idSample, left, right);
        qDebug() << sample;

        PluginProbability * plugin = nullptr;
        QPluginLoader loader(dataBase_.getPathToPlugin(1, 0));
		QObject * pluginObject = loader.instance();
        if (pluginObject) {
            plugin = qobject_cast<PluginProbability *>(pluginObject);
        } else {
            throw std::runtime_error(loader.errorString().toUtf8().constData());
        }

        if (plugin->isRandom(sample, r)) {
            ui_.updateResult("Good");
        } else {
            ui_.updateResult("Do Not Use");
        }

		QString s = QString::number(idSample) + "  " + QString::number(r);
		ui_.textEdit->append(s);
    } catch (std::exception const & e) {
        ui_.updateResult(e.what());
    }
}


/* End of 'type_task_probability.cpp' file */