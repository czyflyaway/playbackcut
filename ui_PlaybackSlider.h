/********************************************************************************
** Form generated from reading UI file 'PlaybackSlider.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PLAYBACKSLIDER_H
#define UI_PLAYBACKSLIDER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PlaybackSlider
{
public:

    void setupUi(QWidget *PlaybackSlider)
    {
        if (PlaybackSlider->objectName().isEmpty())
            PlaybackSlider->setObjectName(QString::fromUtf8("PlaybackSlider"));
        PlaybackSlider->resize(400, 300);

        retranslateUi(PlaybackSlider);

        QMetaObject::connectSlotsByName(PlaybackSlider);
    } // setupUi

    void retranslateUi(QWidget *PlaybackSlider)
    {
        PlaybackSlider->setWindowTitle(QCoreApplication::translate("PlaybackSlider", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PlaybackSlider: public Ui_PlaybackSlider {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PLAYBACKSLIDER_H
