/*
    This file is part of Leela GTP.
    Copyright (C) 2018 Fierralin
    Copyright (C) 2017-2018 Marco Calignano

    Leela GTP and Leela Zero are free softwares: you can redistribute it
    and/or modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    Leela GTP is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Leela GTP.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "GTPConfig.h"
#include <QTime>
#include <QtGlobal>
#include <iostream>

GtpConfig::GtpConfig(QWidget *parent, GtpConfigElements *m_config) :
        QDialog (parent),
        main_config(m_config) {
    config.initial();

    this->setWindowTitle("Leela GTP further configurations");
    this->setFixedSize(600, 600);

    this->tmplabel1 = new QLabel("前几步更随机：", this);
    this->tmplabel1->setGeometry(60, 66, 150, 24);
    this->butt_random = new QSpinBox(this);
    this->butt_random->setGeometry(60, 90, 84, 24);
    this->butt_random->setRange(0, 800);
    this->butt_random->setValue(10);
    this->butt_random->setSingleStep(10);
    connect(butt_random, QOverload<int>::of(&QSpinBox::valueChanged),
            [=](int val) { config.random_num = val; });

    this->butt_enablenoise = new QCheckBox("是否允许策略网络随机", this);
    this->butt_enablenoise->setGeometry(60, 120, 150, 24);
    connect(butt_enablenoise, SIGNAL(toggled(bool)), this, SLOT(on_noise()));
    this->butt_enablenoise->setChecked(true);

    this->butt_noiseseed = new QPushButton("刷新随机数种子", this);
    this->butt_noiseseed->setGeometry(60, 150, 90, 24);
    connect(butt_noiseseed, SIGNAL(clicked(bool)), this, SLOT(on_noiseseed()));
    this->show_noiseseed = new QLabel(QString::number(config.random_seed), this);
    this->show_noiseseed->setGeometry(166, 150, 84, 24);
    this->show_noiseseed->setText(QString::number(config.random_seed));

    this->tmplabel2 = new QLabel("设置循环遍历步数", this);
    this->tmplabel2->setGeometry(60, 186, 90, 24);
    this->butt_loopvisits = new QSpinBox(this);
    this->butt_loopvisits->setGeometry(60, 210, 84, 24);
    this->butt_loopvisits->setRange(100, 100000);
    this->butt_loopvisits->setValue(3000);
    this->butt_loopvisits->setSingleStep(500);
    connect(butt_loopvisits, QOverload<int>::of(&QSpinBox::valueChanged),
            [=](int val) { config.loop_visits = val; });

    this->butt_validation = new QCheckBox("是否下权重对抗赛", this);
    this->butt_validation->setGeometry(60, 240, 120, 24);
    connect(butt_validation, SIGNAL(toggled(bool)), this, SLOT(on_validation()));
    this->butt_validation->setChecked(false);
    this->butt_compnetfile = new QPushButton("打开对抗权重文件", this);
    this->butt_compnetfile->setGeometry(60, 270, 100, 24);
    connect(butt_compnetfile, SIGNAL(clicked(bool)), this, SLOT(on_compnetfile()));
    this->show_compnetfile =
        new QLabel("默認文件：" + config.net_component_path + config.net_component_file, this);
    this->show_compnetfile->setGeometry(176, 270, 400, 24);
    on_validation();

    this->butt_heuristic = new QCheckBox("是否使用启发式智能弃权", this);
    this->butt_heuristic->setGeometry(60, 300, 150, 24);
    this->butt_heuristic->setChecked(true);
    connect(butt_heuristic, SIGNAL(toggled(bool)), this, SLOT(on_heuristic()));


    this->tmplabel3 = new QLabel("單GPU上同時下棋局數：", this);
    this->tmplabel3->setGeometry(60, 336, 150, 24);
    this->butt_gpugames = new QSpinBox(this);
    this->butt_gpugames->setGeometry(60, 360, 84, 24);
    this->butt_gpugames->setRange(1, 8);
    this->butt_gpugames->setValue(1);
    this->butt_gpugames->setSingleStep(1);
    //connect(butt_gamesNum, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
    //        [=] (int val) { int_gamesNum = val; });
    connect(butt_gpugames, QOverload<int>::of(&QSpinBox::valueChanged),
            [=](int val) { config.gpu_games = val; });


    this->butt_okay = new QPushButton("确定", this);
    this->butt_okay->setGeometry(60, 420, 84, 24);
    connect(butt_okay, SIGNAL(clicked(bool)), this, SLOT(on_okay()));
    this->butt_cancel = new QPushButton("取消", this);
    this->butt_cancel->setGeometry(150, 420, 84, 24);
    connect(butt_cancel, SIGNAL(clicked(bool)), this, SLOT(on_cancel()));

}

GtpConfig::~GtpConfig() {
    //std::cout << QTime::currentTime().toString("hh:mm:ss").toStdString() << "~GtpConfig\n";
    delete butt_random;
    delete butt_enablenoise;
    delete butt_noiseseed;
    delete butt_loopvisits;
    delete butt_validation;
    delete butt_compnetfile;
    delete show_compnetfile;
    delete butt_heuristic;
    delete butt_gpugames;
    delete butt_okay;
    delete butt_cancel;

    delete tmplabel1;
    delete tmplabel2;
    delete tmplabel3;
}

void GtpConfig::on_noise() {
    //std::cout << QTime::currentTime().toString("hh:mm:ss").toStdString() << "on noise\n";
    if (this->butt_enablenoise->isChecked()) {
        config.enable_noise = true;
    } else {
        config.enable_noise = false;
    }
}

void GtpConfig::on_noiseseed() {
    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
    config.random_seed = qrand();
    this->show_noiseseed->setText(QString::number(config.random_seed));
}

void GtpConfig::on_validation() {
    if (butt_validation->isChecked()) {
        config.validation = true;
        this->butt_compnetfile->setEnabled(true);
        this->show_compnetfile->setEnabled(true);
    } else {
        config.validation = false;
        this->butt_compnetfile->setEnabled(false);
        this->show_compnetfile->setEnabled(false);
    }
}

void GtpConfig::on_compnetfile() {
    QString filename = QFileDialog::getOpenFileName(this, tr("权重文件"));
    QFileInfo file(filename);
    config.net_component_file = file.fileName();
    config.net_component_path = file.filePath();
    this->show_compnetfile->setText(config.net_component_path + config.net_component_file);
}

void GtpConfig::on_heuristic() {
    if (this->butt_heuristic->isChecked()) {
        config.heuristic = true;
    } else {
        config.heuristic = false;
    }
}

void GtpConfig::on_okay() {
    main_config->copy(&config);
    this->close();
    delete this;
}

void GtpConfig::on_cancel() {
    this->close();
    delete this;
}
