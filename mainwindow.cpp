#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    //初始化数据库
    m_database.createConnection();
    m_database.createTable();
    qDebug() << QCoreApplication::applicationDirPath();
    m_LocalMusic = new SongSheet(this);
    m_LocalMusic->name = "本地音乐";


    //UI设计
    //ui->setupUi(this);
    setMaximumSize(1300,800);
    setMinimumSize(1300,800);//固定软件界面的大小
    setWindowFlags(Qt::FramelessWindowHint|windowFlags());//实现无边框



    //下载进度条的初始化
    m_downloadProgressBar = new QProgressBar(this);
    m_downloadProgressBar->setGeometry(730, 28, 400, 15);
    m_downloadProgressBar->hide();
    auto downStyle = new QFile(":/downloadBarStyle.qss");
    QSSParser parse(downStyle);
    m_downloadProgressBar->setStyleSheet("QProgressBar{background:rgb(50, 50, 50);} QProgressBar::chunk{border-radius:5px;background:rgb(62,114,143)}");

    //边框初始化
    //下边框初始化
    m_downWidget = new QWidget(this);
    setDownWidget(m_downWidget);

    //左侧菜单初始化
    m_leftWidget = new QWidget(this);
    setLeftWidget();
    m_leftTable = new Left_Table(this);
    m_leftTable->setGeometry(0, 0, 300, 665);

    {
        QPushButton *button = new QPushButton("本地音乐");
        button->setStyleSheet("QPushButton{font-family:'Microsoft YaHei'; color:rgb(214, 214, 214);border:none;}");
        m_leftTable->m_LeftTableModel->setItem(0, 0, new QStandardItem(QObject::tr("")));
        m_leftTable->m_LeftTable->setIndexWidget(m_leftTable->m_LeftTableModel->index(0, 0), button);
        connect(button, SIGNAL(clicked()), this, SLOT(myMusicClicked()));
    }
    {
        QPushButton *button = new QPushButton("创建的歌单        +");
        button->setStyleSheet("QPushButton{font-family:'Microsoft YaHei';text-align:left;font-size:25px; color:rgb(214, 214, 214);border:none;}");

        m_leftTable->m_LeftTableModel->setItem(1, 0, new QStandardItem(QObject::tr("")));
        m_leftTable->m_LeftTable->setIndexWidget(m_leftTable->m_LeftTableModel->index(1, 0), button);
        connect(button, SIGNAL(clicked()), this, SLOT(creatSongSheetClicked()));
    }
    {
        SongSheet* nss = new SongSheet(this);
        nss->name = "我喜欢的音乐";
        m_SongSheetList.append(nss);

        QPushButton *button = new QPushButton("我喜欢的音乐");
        button->setStyleSheet("QPushButton{font-family:'Microsoft YaHei'; color:rgb(214, 214, 214);border:none;}");
        m_leftTable->m_LeftTableModel->setItem(2, 0, new QStandardItem(QObject::tr("")));
        m_leftTable->m_LeftTable->setIndexWidget(m_leftTable->m_LeftTableModel->index(2, 0), button);
        connect(button, SIGNAL(clicked()), this, SLOT(mySongSheetClicked()));
        m_SongSheetList.append(nss);
        connect(nss->m_SongSheet, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(LocalListClick()));
    }

    m_leftBtnFullScreen = new Left_MusicButton(m_leftWidget);
    m_leftBtnFullScreen->setGeometry(0,597,250,64);
    connect(m_leftBtnFullScreen,SIGNAL(clicked(bool)),this,SLOT(showMusicWidget()));

    m_downPlayWidget = new Down_PlayWidget(m_downWidget);
    m_downPlayWidget->setGeometry(0,0,1300,70);
    m_downVoiceWidget = new Down_VoiceWidget(m_downWidget);
    m_downVoiceWidget->setGeometry(940, 0, 180, 70);

    m_leftMusicShowWidget = new Left_musicShowWidget(this);
    m_leftMusicShowWidget->setGeometry(0,730,0,0);

    m_lyricwindow = new LyricWindow(m_leftMusicShowWidget);
    m_lyricwindow->setGeometry(650, 160, 800, 600);

    //上边框初始化
    m_topWidget = new QWidget(this);
    setTopWidget(m_topWidget);
    m_topSearchWidget = new Top_SearchWidget(m_topWidget);
    m_topSearchWidget->setGeometry(348, 20, 360, 48);

    m_lblLogo = new QLabel(m_topWidget);
    QPixmap pix(":/images/txy2.png");
    m_lblLogo->setPixmap(pix);
    m_lblLogo->setPixmap(m_lblLogo->pixmap()->scaled(QSize(255, 65), Qt::KeepAspectRatio));
    //qDebug() << pix.width() << pix.height();
    m_lblLogo->setGeometry(0,0,255,65);
    m_lblLogo->show();
    m_lblLogo->raise();
    //搜索栏初始化
    //起始位置: 315, 65
    //大小: 360, (500)?
    m_btnMinimum = new QPushButton(m_topWidget);
    m_btnMinimum->setStyleSheet("QPushButton{background:rgb(50,50,50);border:0px;}\
                                QPushButton{image:url(:/images/images/min_pressed.png)}\
                                QPushButton:hover{image:url(:/images/images/min_normal.png)}");
    m_btnMinimum->setGeometry(1225, 22, 25, 25);
    m_btnMinimum->setCursor(Qt::PointingHandCursor);
    m_btnMinimum->setToolTip(QString("最小化"));
    connect(m_btnMinimum, SIGNAL(clicked(bool)), this, SLOT(showMinWindow()) );


    m_btnClose = new QPushButton(m_topWidget);
    m_btnClose->setCursor(Qt::PointingHandCursor);
    m_btnClose->setToolTip(QString("关闭"));
    m_btnClose->setGeometry(1255, 22, 25, 25);
    m_btnClose->setStyleSheet("QPushButton{background:rgb(50,50,50);border:0px;}\
                                   QPushButton{image:url(:/images/images/close_normal.png)}\
                                   QPushButton:hover{image:url(:/images/images/close_pressed.png);}");
    connect(m_btnClose,SIGNAL(clicked(bool)),this,SLOT(closeMainwindow()));




    //middle
    m_middleMusicShow = new Middle_musicShow(this);
    m_middleMusicShow->lower();
    m_middleMusicShow->setGeometry(250,69,1050,661);

    createRedLine();
    //歌词窗体QLabel


    m_searchResult = new Middle_searchResult(this);
    m_searchResult->setGeometry(348, 65, 330, 500);
    m_searchResult->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置不可更改里面的内容
    m_searchResult->hide();             //默认不出现
    //播放器初始化
    m_mediaPlayer = new QMediaPlayer(this);
    m_downPlayWidget->setMediaPlayer(m_mediaPlayer);


    //播放列表可视化
    m_downBtnPlayList = new Down_PlayListButton(m_downWidget);
    m_downBtnPlayList->setGeometry(1220, 0, 180, 70);
    m_showPlayList = new Down_PlayList(this);



    //init of play list
    m_mediaPlayList = new QMediaPlaylist(this);
    m_mediaPlayList->setPlaybackMode(QMediaPlaylist::Loop);
    m_mediaPlayList->setCurrentIndex(0);

    //bind list with player
    m_mediaPlayer->setPlaylist(m_mediaPlayList);
    m_mediaPlayer->setVolume(30);
    m_unMutedVol = 30;
    m_playMode = 0;
    //播放进度条初始化

    //SongSheet * s = new SongSheet(this);
    //s->setGeometry(250,69,1050,661);
    //s->show();

    {
        QSqlDatabase db = QSqlDatabase::database("QSQLITE");
        QSqlQuery query(db);
        query.exec(QString("select songartist,songdir from songlist"));
        while(query.next())
        {
            m_showPlayList->addSong(query.value(0).toString());
            QPushButton *button = new QPushButton();
            button->setCursor(Qt::PointingHandCursor);
            button->setToolTip(QString("删除这首歌"));

            button->setStyleSheet("QPushButton{background:rgb(43,43,43);border:0px;}\
                                           QPushButton{image:url(:/images/images/close_normal.png)}\
                                           QPushButton:hover{image:url(:/images/images/close_pressed.png);}");
            connect(button, SIGNAL(clicked()), this, SLOT(removeBtnClicked()));
            int row=m_showPlayList->m_PlayListModel->rowCount() - 1;
            m_showPlayList->m_PlayList->setIndexWidget(m_showPlayList->m_PlayListModel->index(row, 1), button);
            m_mediaPlayList->addMedia(QUrl::fromLocalFile(query.value(1).toString()));
            Song s = m_database.querySongInfo(m_database.querySong(query.value(0).toString()));
            m_LocalMusic->addLocalSong(s);
            QPushButton *but = new QPushButton();
            but->setCursor(Qt::PointingHandCursor);
            but->setToolTip(QString("喜欢"));

            but->setStyleSheet("QPushButton{background:rgb(43,43,43);border:0px;}\
                                                                  QPushButton{image:url(:/images/love-grey.png)}\
                                                                  QPushButton:checked{image:url(:/images/love.png);}");
            but->setGeometry(0, 0, 25, 25);
            int ro=m_LocalMusic->m_SongSheetModel->rowCount() - 1;
            m_LocalMusic->m_SongSheet->setIndexWidget(m_LocalMusic->m_SongSheetModel->index(ro, 1), but);
            connect(but, SIGNAL(clicked()), this, SLOT(likeBtnClicked()));

            QPushButton *butto = new QPushButton();
            butto->setCursor(Qt::PointingHandCursor);
            butto->setToolTip(QString("添加这首歌到歌单"));

            butto->setStyleSheet("QPushButton{background:rgb(43,43,43);border:0px;}\
                                           QPushButton{image:url(:/images/images/add.png)}\
                                           QPushButton:hover{image:url(:/images/images/add.png);}");
            connect(butto, SIGNAL(clicked()), this, SLOT(addToSongSheetClicked()));
            m_LocalMusic->m_SongSheet->setIndexWidget(m_LocalMusic->m_SongSheetModel->index(ro, 5), butto);
        }
    }


    m_downProgressBar = new Down_PlayProgressBar(m_downWidget);
    m_downloadProgressBar->raise();
    m_downProgressBar->setGeometry(250, 0, 690, 70);
    //auto durTime = m_mediaPlayer->position();
    m_downProgressBar->update();
    //连接信号与槽
    m_middleWheelPic = new Middle_WheelPic(this);
    m_middleWheelPic->setGeometry(450, 130, 800, 800);

    m_middleWheelPic->lower();
    m_middleWheelPic->hide();
    m_middleMusicShow->lower();


    connect(m_downPlayWidget->m_btnPlay, SIGNAL(clicked(bool)), this, SLOT(updateMusicWidget()));
    connect(m_downPlayWidget->m_btnNextSong, SIGNAL(clicked(bool)), this, SLOT(playNextSong()));
    connect(m_downPlayWidget->m_btnPrevSong, SIGNAL(clicked(bool)), this, SLOT(playPrevSong()));

    connect(m_downPlayWidget->m_btnPlayMode, SIGNAL(clicked()), this, SLOT(playmodeChanged()));


    connect(m_downVoiceWidget->m_btnMute, SIGNAL(clicked(bool)), this, SLOT(mute()));
    connect(m_downVoiceWidget->m_sliderVol, SIGNAL(valueChanged(int)), this, SLOT(changeVolVal(int)));


    connect(m_mediaPlayer, SIGNAL(positionChanged(qint64)), this, SLOT(onPositionChanged(qint64)));
    connect(m_mediaPlayer, SIGNAL(durationChanged(qint64)), this, SLOT(onDurationChanged(qint64)));



    //connect(m_mediaPlayer, SIGNAL(currentMediaChanged(QMediaContent)), this, SLOT(updateMusicShowWidget(QMediaContent)));
    connect(m_downProgressBar->m_sliderPlayProgress, SIGNAL(valueChanged(int)), this, SLOT(changePlayProgress(int)));
    connect(m_downBtnPlayList->m_btnPlayList, SIGNAL(clicked(bool)), this, SLOT(showPlayList()));
    connect(m_showPlayList->m_PlayList, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(playListChange()));
    connect(m_LocalMusic->m_SongSheet, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(LocalListClick()));
    //connect(m_leftTable->m_LeftTable, SIGNAL(clicked(const QModelIndex &)), this, SLOT(LeftTableClick()));
    connect(m_topSearchWidget->m_btnSearch, SIGNAL(clicked()), this, SLOT(searchSong()));
    connect(m_topSearchWidget->m_lineSearch, SIGNAL(returnPressed()), this, SLOT(searchSong()));

    m_middleWheelPic->m_lblPrev->installEventFilter(this);
    m_lblLogo->installEventFilter(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::updateMusicShowWidget(QMediaContent curMedia) {
    if (!(m_mediaPlayList->previousIndex(2) == m_mediaPlayList->currentIndex())) {
        auto prePreUrl = m_mediaPlayList->media(m_mediaPlayList->previousIndex(2)).canonicalUrl().toString();
        QString ss;
        for(int i=8;i<prePreUrl.length();i++)
        {
            ss+=prePreUrl[i];
        }
        //qDebug() << ss;
        Song song = m_database.querySongInfo(ss);
        m_middleWheelPic->m_lblPrevPrev->setPixmap(QPixmap(song.getImageUrl()).scaled(400, 400, Qt::KeepAspectRatio));
    }
    else {
        m_middleWheelPic->m_lblPrevPrev->setPixmap(QPixmap());
    }

    if (!(m_mediaPlayList->previousIndex() == m_mediaPlayList->currentIndex())) {
        auto preUrl = m_mediaPlayList->media(m_mediaPlayList->previousIndex()).canonicalUrl().toString();
        QString ss;
        for(int i=8;i<preUrl.length();i++)
        {
            ss+=preUrl[i];
        }
        //qDebug() << ss;
        Song song = m_database.querySongInfo(ss);
        m_middleWheelPic->m_lblPrev->setPixmap(QPixmap(song.getImageUrl()).scaled(400, 400, Qt::KeepAspectRatio));
    }
    else {
        m_middleWheelPic->m_lblPrev->setPixmap(QPixmap());
    }
    QMediaContent tmp = m_mediaPlayer->currentMedia();
    if (!tmp.isNull())
    {
        QString s = tmp.canonicalUrl().toString();
        QString ss;
        for(int i=8;i<s.length();i++)
        {
            ss+=s[i];
        }
        //qDebug() << ss;
        Song song = m_database.querySongInfo(ss);
        m_middleWheelPic->m_lblCur->setPixmap(QPixmap(song.getImageUrl()).scaled(400, 400, Qt::KeepAspectRatio));
        m_leftMusicShowWidget->setSong(&song);
        m_leftBtnFullScreen->setSong(&song);
        m_leftMusicShowWidget->m_tmrUpdate->start();
        m_leftMusicShowWidget->m_tmrUpdateNew->start();
        m_lyricLoader.loadFromFile(song.getLyricUrl());
        auto ly = m_lyricLoader.getAllLine();
        m_lyricwindow->getLyric(ly);
    }

}




void MainWindow::updateMusicWidget() {
    if (m_downPlayWidget->m_btnPlay->isChecked()) {
        //播放列表空否？
        if (!m_mediaPlayList->isEmpty()) {
            m_mediaPlayer->play();
            m_middleMusicShow->m_lblImage->setPixmap(QPixmap(":/images/greybgc.png"));
            if (!m_middleWheelPic->isVisible()) {
                m_middleWheelPic->show();
                updateMusicShowWidget(m_mediaPlayList->currentMedia());
            }
            QMediaContent tmp = m_mediaPlayer->currentMedia();
            QString s = tmp.canonicalUrl().toString();
            QString ss;
            for(int i=8;i<s.length();i++)
            {
                ss+=s[i];
            }
            //qDebug() << ss;
            Song song = m_database.querySongInfo(ss);

            m_leftMusicShowWidget->setSong(&song);
            m_leftBtnFullScreen->setSong(&song);
            m_leftMusicShowWidget->m_tmrUpdate->start();
            m_leftMusicShowWidget->m_tmrUpdateNew->start();
            if(m_downProgressBar->pos().x() <= 2000)
            {
                m_lyricLoader.loadFromFile(song.getLyricUrl());
                auto ly = m_lyricLoader.getAllLine();
                m_lyricwindow->getLyric(ly);
            }
        }
    }

    else {
        m_mediaPlayer->pause();
        m_leftMusicShowWidget->m_tmrUpdate->stop();
        m_leftMusicShowWidget->updateStick();
        if(m_leftMusicShowWidget->j>0)//判断回复杆是否归零
        {
            m_leftMusicShowWidget->j=0;
        }
    }

}

void MainWindow::playNextSong() {
    if (m_mediaPlayList->isEmpty()) {
        return;
    }

    auto nextIndex = m_mediaPlayList->nextIndex();
    m_mediaPlayList->setCurrentIndex(nextIndex);
    updateMusicShowWidget(m_mediaPlayList->currentMedia());
//    auto nextIndex = (m_mediaPlayList->currentIndex() + 1) % m_mediaPlayList->mediaCount();
//    // calculate the index of next song by mod the max size of total media.
//    m_mediaPlayList->setCurrentIndex(nextIndex);

    if (! m_downPlayWidget->m_btnPlay->isChecked()) {
        m_downPlayWidget->m_btnPlay->setChecked(true);
        m_mediaPlayer->play();
    }
    QMediaContent tmp = m_mediaPlayer->currentMedia();
    QString s = tmp.canonicalUrl().toString();
    QString ss;
    for(int i=8;i<s.length();i++)
    {
        ss+=s[i];
    }
    //qDebug() << ss;
    Song song = m_database.querySongInfo(ss);
    m_leftMusicShowWidget->setSong(&song);
    m_leftBtnFullScreen->setSong(&song);
    m_leftMusicShowWidget->m_tmrUpdate->start();
    m_leftMusicShowWidget->m_tmrUpdateNew->start();
    m_lyricLoader.loadFromFile(song.getLyricUrl());
    auto ly = m_lyricLoader.getAllLine();
    m_lyricwindow->getLyric(ly);
}

void MainWindow::playPrevSong() {
    if (m_mediaPlayList->isEmpty()) {
        return;
    }
    auto nextIndex = m_mediaPlayList->previousIndex();
    // calculate the index of next song by mod the max size of total media.
    m_mediaPlayList->setCurrentIndex(nextIndex);
    updateMusicShowWidget(m_mediaPlayList->currentMedia());
    qDebug() << m_mediaPlayList->currentIndex();
    if (! m_downPlayWidget->m_btnPlay->isChecked()) {
        m_downPlayWidget->m_btnPlay->setChecked(true);
        m_mediaPlayer->play();

    }
    QMediaContent tmp = m_mediaPlayer->currentMedia();
    QString s = tmp.canonicalUrl().toString();
    QString ss;
    for(int i=8;i<s.length();i++)
    {
        ss+=s[i];
    }
    //qDebug() << ss;
    Song song = m_database.querySongInfo(ss);
    m_leftMusicShowWidget->setSong(&song);
    m_leftBtnFullScreen->setSong(&song);
    m_leftMusicShowWidget->m_tmrUpdate->start();
    m_leftMusicShowWidget->m_tmrUpdateNew->start();
    m_lyricLoader.loadFromFile(song.getLyricUrl());
    auto ly = m_lyricLoader.getAllLine();
    m_lyricwindow->getLyric(ly);
}

void MainWindow::mute() {
    if (! m_downVoiceWidget->m_btnMute->isChecked()) {
        //设置default音量
        m_mediaPlayer->setVolume(m_unMutedVol);
        m_downVoiceWidget->m_sliderVol->setValue(m_unMutedVol);
    }
    else {
        m_mediaPlayer->setVolume(0);
        m_downVoiceWidget->m_sliderVol->setValue(0);
    }

}

void MainWindow::changeVolVal(int val) {
    if (m_downVoiceWidget->m_btnMute->isChecked() && val != 0) {
        m_downVoiceWidget->m_btnMute->setChecked(false);
    }
    if (val != 0){  //否则unmutedvol永远是0
        m_unMutedVol = val;
    }
    m_mediaPlayer->setVolume(val);
}

void MainWindow::changePlayProgress(int val) {
    if (! m_mediaPlayList->mediaCount()) {
        return;
    }
    if (qAbs(m_mediaPlayer->position() - val)> 99) {
        m_mediaPlayer->setPosition(val);
    }


}

void MainWindow::onDurationChanged(qint64 duration){
    if (! m_mediaPlayList->mediaCount()) {
        return;
    }
    m_downProgressBar->m_sliderPlayProgress->setMaximum(duration);

    int sec = duration / 1000;      //秒数
    int min = sec / 60;
    sec %= 60;
    if (sec < 10) {
        m_tDuration = QString::asprintf("%d:0%d", min, sec);
    }
    else {
        m_tDuration = QString::asprintf("%d:%d", min, sec);
    }
    m_downProgressBar->m_lblRight->setText(m_tDuration);

}

void MainWindow::onPositionChanged(qint64 position) {
    if (m_downProgressBar->m_sliderPlayProgress->isSliderDown()) {
        return;
    }
    m_lyricwindow->update(position);
    m_downProgressBar->m_sliderPlayProgress->setSliderPosition(position);
    int sec = position / 1000;      //秒数
    int min = sec / 60;
    sec %= 60;
    if (sec < 10) {
        m_tPosition = QString::asprintf("%d:0%d", min, sec);
    }
    else {
        m_tPosition = QString::asprintf("%d:%d", min, sec);
    }
    m_downProgressBar->m_lblLeft->setText(m_tPosition);
    if (position == 0) {
        updateMusicShowWidget(m_mediaPlayList->currentMedia());

    }
}

void MainWindow::setDownWidget(QWidget *widget) {

    widget->setGeometry(0,730,1300,70);
    QPalette pal(widget->palette());
    pal.setColor(QPalette::Background,QColor(50,50,50));
    widget->setAutoFillBackground(true);
    widget->setPalette(pal);
    //widget->show();
}


void MainWindow::setTopWidget(QWidget *widget) {
    widget->setGeometry(0, 0, 1300, 65);
    QPalette pal(widget->palette());

    pal.setColor(QPalette::Background, QColor(50, 50, 50));
    widget->setAutoFillBackground(true);
    widget->setPalette(pal);
    widget->show();
}

void MainWindow::createRedLine()
{
    m_redWidget = new QWidget(this);
    m_redWidget->setGeometry(0,65,1300,4);
    QPalette pal(m_redWidget->palette());
    pal.setColor(QPalette::Background,QColor(177,34,34));
    m_redWidget->setAutoFillBackground(true);
    m_redWidget->setPalette(pal);
    m_redWidget->show();
}


void MainWindow::setLeftWidget() {
    m_leftWidget->setGeometry(0,69,250,661);
    QPalette pal(m_leftWidget->palette());
    pal.setColor(QPalette::Background,QColor(52,53,44));
    m_leftWidget->setAutoFillBackground(true);
    m_leftWidget->setPalette(pal);
    m_leftWidget->show();
}

void MainWindow::createFolder(QString folder) {
    QDir* newFolder = new QDir;
    bool exist = newFolder->exists(folder);

    if (exist) {
        //qDebug() << "This folder" << folder << "already exists" << endl;
    }
    else {
        newFolder->mkdir(folder);
        qDebug() << "folder making..";
    }
}

QString MainWindow::getSongId(QJsonObject obj) {
    auto doubleId = obj.value("id").toDouble();

    QString strId = QString::number(doubleId, 'f', 0);

    return strId;
}

QString MainWindow::getArtistName(QJsonObject obj) {
    auto artistArr = obj.value("artists");
    auto artistObj = artistArr[0];
    auto artistName = artistObj["name"];
    auto strArtistName = artistName.toString();
    return strArtistName;
}

void MainWindow::mousePressEvent(QMouseEvent *e) {
   // qDebug() << m_searchResult->pos();
    if (!isInRange(e->pos(), m_searchResult)
       &&!isInRange(e->pos(), m_topSearchWidget)) {
        m_searchResult->hide();
    }

    if (!isInRange(e->pos(), m_showPlayList)) {
        m_showPlayList->hide();
    }

    if(e->pos().y()<69) {
        e->ignore();
        if(e->button() == Qt::LeftButton) {
            m_pressed = true;
            m_point = e->pos();
        }
    }
}

bool MainWindow::isInRange(QPoint p, QWidget *widget) {
    if (p.x() <= widget->pos().x() + widget->width() && p.x() >= widget->pos().x()
      &&p.y() <= widget->pos().y() + widget->height() && p.y() >= widget->pos().y()) {
        return true;
    }
    return false;
}

void MainWindow::showPlayList() {

    if(!m_showPlayList->isVisible()) {
        m_showPlayList->show();
        m_showPlayList->setGeometry(800, 0, 500, 730);
    }
    else {
        m_showPlayList->hide();
    }
}

void MainWindow::searchSong() {
    auto strToSearch = m_topSearchWidget->m_lineSearch->text();
    qDebug() << "Hello" + strToSearch.toLatin1();
    m_searchResult->show();
    QUrl netEaseUrl = QUrl("http://music.163.com/api/search/get/web?csrf_token=hlpretag=&hlposttag=&s=" +strToSearch + "&type=1&offset=0&total=true&limit=10");
    QUrl url = QUrl("https://api.paugram.com/netease/?title=" + strToSearch);
    QNetworkAccessManager manager;
    QNetworkReply* searchReply = manager.get(QNetworkRequest(netEaseUrl));
    QEventLoop loop;
    connect (searchReply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();


    if (searchReply->error() == QNetworkReply::NoError) {
        QByteArray array = searchReply->readAll();
        QJsonParseError jsonErr;
        QJsonDocument json = QJsonDocument::fromJson(array, &jsonErr);
        QStringList strList;
        if (jsonErr.error == QJsonParseError::NoError) {
            QJsonObject obj = json.object();
            //处理搜索到的结果

            auto res = obj["result"].toObject();
            this->m_songArr = res["songs"].toArray();
            for (int i = 0; i < m_songArr.size(); i++) {
                auto objSong = m_songArr[i].toObject();
                auto id = getSongId(objSong);
                auto songName = objSong["name"].toString();
                auto artistName = getArtistName(objSong);
                strList.push_back(artistName + '-' + songName);
               // qDebug() << objSong["name"].toString() << artistName;
            }

            m_searchResult->setList(&strList);
            connect(m_searchResult, &QAbstractItemView::doubleClicked, this, &MainWindow::downloadSelectedSong, Qt::UniqueConnection);
            // ToDo: 下载完成后加入自动扫描到歌单的功能
        }
    }

}

void MainWindow::showMinWindow()
{
    showMinimized();
}

void MainWindow::showMusicWidget() {
    QPropertyAnimation *animation=new QPropertyAnimation(m_leftMusicShowWidget,"geometry");
    animation->setDuration(300);
    animation->setStartValue(QRect(0,730,1300,0));
    animation->setEndValue(QRect(0,69,1300,661));
    animation->start(QAbstractAnimation::DeleteWhenStopped);

}

void MainWindow::firstFinished() {

    connect(m_redirectedReply,&QNetworkReply::readyRead,this,&MainWindow::httpReadyRead);
    connect(m_redirectedReply,&QNetworkReply::downloadProgress,this,&MainWindow::updateDataReadProgress);
    connect(m_redirectedReply,&QNetworkReply::finished,this,&MainWindow::httpFinished);
}


void MainWindow::httpReadyRead() {
    if (m_curFile) {
        m_curFile->write(m_redirectedReply->readAll());
    }

}

void MainWindow::updateDataReadProgress(qint64 bytesRead, qint64 totalBytes) {
    //qDebug() << bytesRead << "/" << totalBytes;
    m_downloadProgressBar->setMaximum(totalBytes);

    m_downloadProgressBar->setValue(bytesRead);
}

void MainWindow::httpFinished() {
    m_downloadProgressBar->hide();
    //qDebug() << "Downloaded" <<  m_curFile->size() << "bytes" ;
    if (m_curFile->size() > 1000) {
        disconnect(m_redirectedReply,&QNetworkReply::readyRead,this,&MainWindow::httpReadyRead);
        disconnect(m_redirectedReply,&QNetworkReply::downloadProgress,this,&MainWindow::updateDataReadProgress);
        disconnect(m_redirectedReply,&QNetworkReply::finished,this,&MainWindow::httpFinished);
    }

    if (m_curFile) {
        m_curFile->close();
        delete m_curFile;
        m_curFile = nullptr;
    }

    if(m_redirectedReply){
        delete m_redirectedReply;
    }
    m_redirectedReply = nullptr;
}

void MainWindow::downloadSelectedSong(const QModelIndex &index) {
    //这坨代码写的太屎了。。
    auto objSong = m_songArr[index.row()].toObject();
    //qDebug() << objSong.value("img1v1Url").toString();
    //qDebug() << objSong;
    auto strId = getSongId(objSong);
    id = strId;
    songName = objSong["name"].toString();
    artistName = getArtistName(objSong);
    //qDebug() << strId;
    auto absPath = QApplication::applicationDirPath();
    auto dir = QApplication::applicationDirPath() + QString("/music/");
    createFolder(dir);
    m_curSongName = index.data().toString();
    filePath = dir + m_curSongName + ".mp3";
    //qDebug() << "FilePath:" << filePath;
    m_curFile = new QFile(filePath);
    m_curFile->open(QIODevice::WriteOnly);
    if (!m_curFile) {
        qDebug() << "文件打开失败";
        return;
    }
    m_accessManager = new QNetworkAccessManager(this);
    m_request = QNetworkRequest();

    auto link = QString("https://music.163.com/song/media/outer/url?id=" + strId);

    m_request.setUrl(link);
    m_reply = m_accessManager->get(m_request);
    m_downloadProgressBar->show();
    //m_downloadProgressBar->raise();
    m_searchResult->hide();

    //add music information to play list



    connect(m_reply,&QNetworkReply::finished,this,&MainWindow::urlRedirected);
    connect(m_reply,QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error),          //异常
                this,&MainWindow::doProcessError);


    //download lyrics.
    auto lyricLink = QString("http://music.163.com/api/song/media?id=" + strId);
    auto acMgr = new QNetworkAccessManager();
    auto req =  QNetworkRequest();
    req.setUrl(lyricLink);
    m_lyricReply = acMgr->get(req);
    auto lyricPath = QApplication::applicationDirPath() + QString("/lyric/") ;
    lyricFileName = lyricPath + m_curSongName + ".txt";
    m_lyricFile = new QFile(lyricFileName);
    createFolder(lyricPath);



    QEventLoop loop;
    connect (m_lyricReply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    QByteArray array = m_lyricReply->readAll();
    QJsonParseError jsonErr;
    QJsonDocument json = QJsonDocument::fromJson(array, &jsonErr);
    auto obj = json.object();
    if (m_lyricFile->open(QIODevice::WriteOnly|QIODevice::Text)) {
        //m_lyricFile->write(array);

        QString strLyric = obj["lyric"].toString();
        QTextStream ts(m_lyricFile);

        ts.setCodec("unicode");
        ts << strLyric << endl;
        m_lyricFile->close();
    }

    auto albumLink = QString("https://api.imjad.cn/cloudmusic/?type=detail&id=" + strId);
    req.setUrl(albumLink);
    m_albumReply = acMgr->get(req);
    QEventLoop albumLoop;
    connect (m_albumReply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    QByteArray albumArray = m_albumReply->readAll();

    QJsonDocument albumJson = QJsonDocument::fromJson(albumArray, &jsonErr);
    auto albumObj = albumJson.object();
    auto songArr = albumObj.value("songs").toArray();
    auto songObj = songArr.at(0).toObject();
    auto newSongObj = songObj.value("al").toObject();
    qDebug() << "newSongObj:" << newSongObj;
    auto abname = newSongObj.value("name").toString();
    qDebug() << "abName:" << abname;
    coverLink = newSongObj.value("picUrl").toString();
    qDebug() << "coverLink:" << coverLink;
    req.setUrl(coverLink);
    m_albumReply = acMgr->get(req);
    connect (m_albumReply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    auto albumPath = QApplication::applicationDirPath() + QString("/album/") ;
    albumName = albumPath + m_curSongName + ".jpg";
    m_albumFile = new QFile(albumName);
    createFolder(albumPath);
    if (m_albumFile->open(QIODevice::WriteOnly)) {
        m_albumFile->write( m_albumReply->readAll());
        m_albumFile->close();
        qDebug() << "successfully downloaded the album cover.";
        //auto abname = albumObj.value("album").toString();
            if(m_database.insert(id, songName, artistName, filePath, albumName, lyricFileName, abname))
            {
                QString name = songName + "-" + artistName;
                m_showPlayList->addSong(name);
                QPushButton *button = new QPushButton("");
                button->setCursor(Qt::PointingHandCursor);
                button->setToolTip(QString("删除这首歌"));

                button->setStyleSheet("QPushButton{background:rgb(43,43,43);border:0px;}\
                                               QPushButton{image:url(:/images/images/close_normal.png)}\
                                               QPushButton:hover{image:url(:/images/images/close_pressed.png);}");
                int row=m_showPlayList->m_PlayListModel->rowCount() - 1;
                m_showPlayList->m_PlayList->setIndexWidget(m_showPlayList->m_PlayListModel->index(row, 1), button);

                connect(button, SIGNAL(clicked()), this, SLOT(removeBtnClicked()));
                QString songdir = m_database.querySong(name);
                m_mediaPlayList->addMedia(QUrl::fromLocalFile(songdir));
                Song s = m_database.querySongInfo(filePath);
                m_LocalMusic->addLocalSong(s);
                QPushButton *but = new QPushButton();
                but->setCursor(Qt::PointingHandCursor);
                but->setToolTip(QString("喜欢"));

                but->setStyleSheet("QPushButton{background:rgb(43,43,43);border:0px;}\
                                                                      QPushButton{image:url(:/images/love-grey.png)}\
                                                                      QPushButton:checked{image:url(:/images/love.png);}");
                but->setGeometry(0, 0, 25, 25);
                int ro=m_LocalMusic->m_SongSheetModel->rowCount() - 1;
                m_LocalMusic->m_SongSheet->setIndexWidget(m_LocalMusic->m_SongSheetModel->index(ro, 1), but);
                connect(but, SIGNAL(clicked()), this, SLOT(likeBtnClicked()));
                QPushButton *butto = new QPushButton();
                butto->setCursor(Qt::PointingHandCursor);
                butto->setToolTip(QString("添加这首歌到歌单"));

                butto->setStyleSheet("QPushButton{background:rgb(43,43,43);border:0px;}\
                                               QPushButton{image:url(:/images/images/add.png)}\
                                               QPushButton:hover{image:url(:/images/images/add.png);}");
                connect(butto, SIGNAL(clicked()), this, SLOT(addToSongSheetClicked()));
                m_LocalMusic->m_SongSheet->setIndexWidget(m_LocalMusic->m_SongSheetModel->index(ro, 5), butto);

            }

    }
}

void MainWindow::urlRedirected() {
    auto url = m_reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toString();

    m_redirectedRequest = QNetworkRequest();
    //qDebug() << url;
    m_redirectedRequest.setUrl(url);
    m_redirAccMgr = new QNetworkAccessManager(this);
    m_redirectedReply = m_redirAccMgr->get(m_redirectedRequest);
    connect(m_redirectedReply,&QNetworkReply::readyRead,this,&MainWindow::httpReadyRead);
    connect(m_redirectedReply,&QNetworkReply::downloadProgress,this,&MainWindow::updateDataReadProgress);
    connect(m_redirectedReply,&QNetworkReply::finished,this,&MainWindow::httpFinished);
    connect(m_redirectedReply,QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error),          //异常
            this,&MainWindow::doProcessError);
    //connect(m_redirectedReply,&QNetworkReply::finished,this,&MainWindow::firstFinished);
}

void MainWindow::doProcessError(QNetworkReply::NetworkError code) {
    qDebug() << code;

}

void MainWindow::lyricRead() {
    m_lyricFile->write(m_lyricReply->readAll());
    qDebug() << "lyric loaded.";
}

void MainWindow::playmodeChanged() {
    m_playMode = (m_playMode + 1) % 3;
    qDebug() << m_playMode;
    if (m_playMode == 0) {
        m_mediaPlayList->setPlaybackMode(QMediaPlaylist::Loop);

        m_downPlayWidget->m_btnPlayMode->setToolTip("顺序播放");
        m_downPlayWidget->m_btnPlayMode->setStyleSheet("QPushButton{border-image:url(:/images/images/comboxitem2.png)}");
        m_downPlayWidget->m_btnPlayMode->setGeometry(1120, 25, 30, 20);

    }

    else if (m_playMode == 1) {
        m_mediaPlayList->setPlaybackMode(QMediaPlaylist::Random);
        m_downPlayWidget->m_btnPlayMode->setToolTip("随机播放");
        m_downPlayWidget->m_btnPlayMode->setStyleSheet("QPushButton{border-image:url(:/images/images/comboxitem1.png)}");
        m_downPlayWidget->m_btnPlayMode->setGeometry(1120, 25, 30, 20);
    }

    else if (m_playMode == 2) {
        m_mediaPlayList->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
        m_downPlayWidget->m_btnPlayMode->setToolTip("单曲循环");
        m_downPlayWidget->m_btnPlayMode->setStyleSheet("QPushButton{border-image:url(:/images/images/comboxitem3.png)}");
        m_downPlayWidget->m_btnPlayMode->setGeometry(1120, 20, 35, 35);
    }
}

void MainWindow::playListChange()
{
    int row = m_showPlayList->m_PlayList->currentIndex().row();
    int col = m_showPlayList->m_PlayList->currentIndex().column();
    if(col == 0)
    {
        QModelIndex index = m_showPlayList->m_PlayListModel->index(row,0);
        QString name = m_showPlayList->m_PlayListModel->data(index).toString();
        QString songdir = m_database.querySong(name);
        QString lyrdir = m_database.queryLyr(name);
        if(songdir == "")
        {
            qDebug() << "No url existed!";
            m_mediaPlayList->removeMedia(row);
        }
        qDebug() << m_lyricLoader.loadFromFile(lyrdir);
        auto ly = m_lyricLoader.getAllLine();
        m_lyricwindow->getLyric(ly);
        m_mediaPlayList->setCurrentIndex(row);
        m_mediaPlayer->play();
        m_downPlayWidget->m_btnPlay->setChecked(true);
        updateMusicWidget();
    }
    else if(col == 1)
    {
        QModelIndex index = m_showPlayList->m_PlayListModel->index(row, 0);
        QString name = m_showPlayList->m_PlayListModel->data(index).toString();
        QString songdir = m_database.querySong(name);
        QString url = "file:///" + songdir;
        QMediaContent tmp = m_mediaPlayer->currentMedia();
        QString s = tmp.canonicalUrl().toString();
        if(s == url)
        {
            playNextSong();
            m_mediaPlayer->stop();
            m_downPlayWidget->m_btnPlay->setChecked(false);
        }
        m_mediaPlayList->removeMedia(row);
        m_showPlayList->m_PlayListModel->removeRow(row);
        m_showPlayList->name = "";
    }
}
void MainWindow::removeBtnClicked()
{
    if(m_showPlayList->isVisible())
    {
        int row = m_showPlayList->m_PlayList->currentIndex().row();
        int col = m_showPlayList->m_PlayList->currentIndex().column();
        QModelIndex index = m_showPlayList->m_PlayListModel->index(row, 0);
        QString name = m_showPlayList->m_PlayListModel->data(index).toString();
        QString songdir = m_database.querySong(name);
        QString url = "file:///" + songdir;
        QMediaContent tmp = m_mediaPlayer->currentMedia();
        QString s = tmp.canonicalUrl().toString();
        if(s == url)
        {
            playNextSong();
            m_mediaPlayer->stop();
            m_downPlayWidget->m_btnPlay->setChecked(false);
        }
        m_mediaPlayList->removeMedia(row);
        m_showPlayList->m_PlayListModel->removeRow(row);
        m_showPlayList->name = "";
    }
    else
    {
        SongSheet* ss = new SongSheet();
        foreach(SongSheet* s, m_SongSheetList)
        {
            if(s->isVisible())
            {
                ss = s;
                break;
            }
        }
        int row = ss->m_SongSheet->currentIndex().row();
        ss->m_SongSheetModel->removeRow(row);
        if(m_showPlayList->name == ss->name) m_showPlayList->name = "";
    }
}


void MainWindow::myMusicClicked()
{
    foreach(SongSheet* ss, m_SongSheetList) ss->hide();
    if(m_LocalMusic->isVisible()) m_LocalMusic->hide();
    else m_LocalMusic->Show();
}
void MainWindow::mySongSheetClicked()
{
    int row = m_leftTable->m_LeftTable->currentIndex().row();
    m_LocalMusic->hide();
    qDebug() << row;
    if(m_SongSheetList.at(row - 1)->isVisible())
    {
        m_SongSheetList.at(row - 1)->hide();
    }
    else
    {
        foreach(SongSheet* ss, m_SongSheetList)
        {
            if(ss->isVisible())
                ss->hide();
        }
        SongSheet* ss = m_SongSheetList.at(row - 1);
        ss->Show();
    }
}
void MainWindow::creatSongSheetClicked()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("QInputDialog::getText()"),tr("请输入歌单名称:"), QLineEdit::Normal, "",&ok);
    if(ok)
    {
        foreach(SongSheet* ss, m_SongSheetList)
        {
            if(ss->name == text)
            {
                qDebug() << "same song sheet name";
                return;
            }
        }
        SongSheet* nss = new SongSheet(this);
        nss->name = text;
        QPushButton *button = new QPushButton(text);
        button->setStyleSheet("QPushButton{font-family:'Microsoft YaHei'; color:rgb(214, 214, 214);border:none;}");
        int row = m_leftTable->m_LeftTableModel->rowCount();
        m_leftTable->m_LeftTableModel->setItem(row, 0, new QStandardItem(QObject::tr("")));
        m_leftTable->m_LeftTable->setIndexWidget(m_leftTable->m_LeftTableModel->index(row, 0), button);
        m_SongSheetList.append(nss);
        connect(button, SIGNAL(clicked()), this, SLOT(mySongSheetClicked()));
        connect(nss->m_SongSheet, SIGNAL(clicked(const QModelIndex &)), this, SLOT(LocalListClick()));
    }
}
void MainWindow::LeftTableClick()
{
    int row = m_leftTable->m_LeftTable->currentIndex().row();
    if(row == 0)
    {
        foreach(SongSheet* ss, m_SongSheetList) ss->hide();
        if(m_LocalMusic->isVisible()) m_LocalMusic->hide();
        else m_LocalMusic->Show();
    }
    else if(row > 1)
    {
        m_LocalMusic->hide();
        if(m_SongSheetList.at(row - 2)->isVisible()) m_SongSheetList.at(row - 2)->hide();
        else
        {
            foreach(SongSheet* ss, m_SongSheetList)
            {
                ss->hide();
            }
            SongSheet* ss = m_SongSheetList.at(row - 2);
            ss->Show();
        }
    }
}


void MainWindow::LocalListClick()
{
    SongSheet* ss = new SongSheet();
    if(m_LocalMusic->isVisible()) ss = m_LocalMusic;
    else foreach(SongSheet* s, m_SongSheetList) if(s->isVisible()) {ss = s; break;}
    int row = ss->m_SongSheet->currentIndex().row();
    {
        if(m_showPlayList->name == ss->name)
        {
            QModelIndex index = ss->m_SongSheetModel->index(row,2);
            QString name = ss->m_SongSheetModel->data(index).toString();
            index = ss->m_SongSheetModel->index(row,3);
            QString artist = ss->m_SongSheetModel->data(index).toString();
            QString songdir = m_database.querySong(name + "-" + artist);
            QString lyrdir = m_database.queryLyr(name + "-" + artist);
            if(songdir == "")
            {
                qDebug() << "No url existed!";
                m_mediaPlayList->removeMedia(row);
            }
            qDebug() << m_lyricLoader.loadFromFile(lyrdir);
            auto ly = m_lyricLoader.getAllLine();
            m_lyricwindow->getLyric(ly);
            m_mediaPlayList->setCurrentIndex(row);
            //m_mediaPlayer->play();
            m_downPlayWidget->m_btnPlay->setChecked(true);
            updateMusicWidget();
            //m_mediaPlayer->play();
        }
        else
        {
            m_showPlayList->name = ss->name;
            int roww = ss->m_SongSheetModel->rowCount();
            int ro = m_showPlayList->m_PlayListModel->rowCount();
            m_showPlayList->m_PlayListModel->removeRows(0, ro);

            m_mediaPlayer->stop();
            m_mediaPlayList->clear();
            for(int i = 0; i < roww; i++)
            {
                QModelIndex index = ss->m_SongSheetModel->index(i,2);
                QString name = ss->m_SongSheetModel->data(index).toString();
                index = ss->m_SongSheetModel->index(i,3);
                QString artist = ss->m_SongSheetModel->data(index).toString();
                QString songdir = m_database.querySong(name + "-" + artist);
                m_showPlayList->addSong(name + "-" + artist);
                QPushButton *button = new QPushButton();
                int row=m_showPlayList->m_PlayListModel->rowCount() - 1;

                button->setCursor(Qt::PointingHandCursor);
                button->setToolTip(QString("删除这首歌"));

                button->setStyleSheet("QPushButton{background:rgb(50,50,50);border:0px;}\
                                               QPushButton{image:url(:/images/images/close_normal.png)}\
                                               QPushButton:hover{image:url(:/images/images/close_pressed.png);}");

                m_showPlayList->m_PlayList->setIndexWidget(m_showPlayList->m_PlayListModel->index(row, 1), button);
                connect(button, SIGNAL(clicked()), this, SLOT(removeBtnClicked()));
                m_mediaPlayList->addMedia(QUrl::fromLocalFile(songdir));
            }
            m_mediaPlayList->setCurrentIndex(row);
            //m_mediaPlayer->play();
            m_downPlayWidget->m_btnPlay->setChecked(true);
            updateMusicWidget();
            //m_mediaPlayer->play();
        }
    }
}

void MainWindow::closeMainwindow() {
    this->setMinimumSize(0,0);
    QPropertyAnimation *animation=new QPropertyAnimation(this,"geometry");
    animation->setDuration(500);
    animation->setStartValue(geometry());
    animation->setEndValue(QRect(geometry().x(), geometry().y(), 0, 0));
    connect(animation,SIGNAL(finished()),this,SLOT(close()));
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}


void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if(event->pos().y()<69)
    {
            if(m_pressed)
            {
                move(event->pos()-m_point+pos());
            }
    }
}
void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
     if(event->pos().y()<69)
     {
         m_pressed=false;
     }
}

void MainWindow::closeEvent(QCloseEvent *event) {
    Q_UNUSED(event);
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if(qobject_cast<QLabel*>(obj) == m_middleWheelPic->m_lblPrev &&event->type() == QEvent::MouseButtonRelease)
    {
        playPrevSong();
        return true;
    }

    if(qobject_cast<QLabel*>(obj) == m_lblLogo &&event->type() == QEvent::MouseButtonRelease)
    {
        qDebug() << "logo is pressed !";
        backToHome();

        return true;
    }

    return false;

}

void MainWindow::likeBtnClicked()
{
    SongSheet* ss = new SongSheet();
    if(m_LocalMusic->isVisible()) ss = m_LocalMusic;
    else
    {
        foreach(SongSheet* s, m_SongSheetList)
        {
            if(s->isVisible())
            {
                ss = s;
                break;
            }
        }
    }
    int row = ss->m_SongSheet->currentIndex().row();
    QModelIndex index = ss->m_SongSheetModel->index(row,2);
    QString name = ss->m_SongSheetModel->data(index).toString();
    index = ss->m_SongSheetModel->index(row, 3);
    QString artist = ss->m_SongSheetModel->data(index).toString();
    QString na = name + "-" + artist;
    QString url = m_database.querySong(na);
    int flag = 0;
    foreach(QString s, m_likes)
    {
        if(s == name)
        {
            flag = 1;
            m_likes.removeOne(name);
            break;
        }
    }
    if(flag == 0)
    {
        SongSheet* likess = m_SongSheetList.at(0);
        Song song = m_database.querySongInfo(url);
        likess->addSong(song);
        QPushButton *but = new QPushButton();
        but->setCursor(Qt::PointingHandCursor);
        but->setToolTip(QString("喜欢"));

        but->setStyleSheet("QPushButton{background:rgb(43,43,43);border:0px;}\
                                                              QPushButton{image:url(:/images/love-grey.png)}\
                                                              QPushButton:checked{image:url(:/images/love.png);}");
        but->setGeometry(0, 0, 25, 25);
        int ro=likess->m_SongSheetModel->rowCount() - 1;
        likess->m_SongSheet->setIndexWidget(likess->m_SongSheetModel->index(ro, 1), but);
        connect(but, SIGNAL(clicked()), this, SLOT(likeBtnClicked()));
        m_likes.append(name);
        return;
    }
    else
    {
        SongSheet* likess = m_SongSheetList.at(0);
        for(int i = 0; i < ss->m_SongSheetModel->rowCount(); i++)
        {
            index = likess->m_SongSheetModel->index(i, 2);
            if(likess->m_SongSheetModel->data(index).toString() == name)
            {
                likess->m_SongSheetModel->removeRow(i);
                return;
            }
        }
    }
}



void MainWindow::addToSongSheetClicked()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("QInputDialog::getText()"),tr("请输入要添加到的歌单名称:"), QLineEdit::Normal, "",&ok);
    int row = m_LocalMusic->m_SongSheet->currentIndex().row();
    QModelIndex index = m_LocalMusic->m_SongSheetModel->index(row,2);
    QString name = m_LocalMusic->m_SongSheetModel->data(index).toString();
    index = m_LocalMusic->m_SongSheetModel->index(row, 3);
    QString artist = m_LocalMusic->m_SongSheetModel->data(index).toString();
    QString na = name + "-" + artist;
    QString url = m_database.querySong(na);
    Song song = m_database.querySongInfo(url);

    if(ok)
    {
        foreach(SongSheet* ss, m_SongSheetList)
        {
            if(ss->name == text)
            {
                ss->addSong(song);
                QPushButton *but = new QPushButton();
                but->setCursor(Qt::PointingHandCursor);
                but->setToolTip(QString("喜欢"));

                but->setStyleSheet("QPushButton{background:rgb(43,43,43);border:0px;}\
                                                                      QPushButton{image:url(:/images/love-grey.png)}\
                                                                      QPushButton:checked{image:url(:/images/love.png);}");
                but->setGeometry(0, 0, 25, 25);
                int ro=ss->m_SongSheetModel->rowCount();
                ss->m_SongSheet->setIndexWidget(ss->m_SongSheetModel->index(ro - 1, 1), but);
                connect(but, SIGNAL(clicked()), this, SLOT(likeBtnClicked()));

                QPushButton *button = new QPushButton();
                button->setCursor(Qt::PointingHandCursor);
                button->setToolTip(QString("删除这首歌"));

                button->setStyleSheet("QPushButton{background:rgb(43,43,43);border:0px;}\
                                               QPushButton{image:url(:/images/images/close_normal.png)}\
                                               QPushButton:hover{image:url(:/images/images/close_pressed.png);}");
                ss->m_SongSheet->setIndexWidget(ss->m_SongSheetModel->index(ro - 1, 5), button);
                connect(button, SIGNAL(clicked()), this, SLOT(removeBtnClicked()));
                return;
            }
        }

        SongSheet* nss = new SongSheet(this);
        nss->name = text;
        QPushButton *button = new QPushButton(text);
        button->setStyleSheet("QPushButton{font-family:'Microsoft YaHei'; color:rgb(214, 214, 214);border:none;}");
        int row = m_leftTable->m_LeftTableModel->rowCount();
        m_leftTable->m_LeftTableModel->setItem(row, 0, new QStandardItem(QObject::tr("")));
        m_leftTable->m_LeftTable->setIndexWidget(m_leftTable->m_LeftTableModel->index(row, 0), button);
        connect(button, SIGNAL(clicked()), this, SLOT(mySongSheetClicked()));
        connect(nss->m_SongSheet, SIGNAL(clicked(const QModelIndex &)), this, SLOT(LocalListClick()));


        nss->addSong(song);
        QPushButton *but = new QPushButton();
        but->setCursor(Qt::PointingHandCursor);
        but->setToolTip(QString("喜欢"));

        but->setStyleSheet("QPushButton{background:rgb(43,43,43);border:0px;}\
                                                              QPushButton{image:url(:/images/love-grey.png)}\
                                                              QPushButton:checked{image:url(:/images/love.png);}");
        but->setGeometry(0, 0, 25, 25);
        int ro=nss->m_SongSheetModel->rowCount();
        nss->m_SongSheet->setIndexWidget(nss->m_SongSheetModel->index(ro - 1, 1), but);
        connect(but, SIGNAL(clicked()), this, SLOT(likeBtnClicked()));

        QPushButton *butto = new QPushButton();
        butto->setCursor(Qt::PointingHandCursor);
        butto->setToolTip(QString("删除这首歌"));

        butto->setStyleSheet("QPushButton{background:rgb(43,43,43);border:0px;}\
                                       QPushButton{image:url(:/images/images/close_normal.png)}\
                                       QPushButton:hover{image:url(:/images/images/close_pressed.png);}");
        nss->m_SongSheet->setIndexWidget(nss->m_SongSheetModel->index(ro - 1, 5), butto);
        connect(butto, SIGNAL(clicked()), this, SLOT(removeBtnClicked()));
        m_SongSheetList.append(nss);
    }
}


void MainWindow::backToHome()
{
    m_LocalMusic->hide();
    foreach(SongSheet* s, m_SongSheetList)
    {
        s->hide();
    }
    m_showPlayList->hide();
    m_middleWheelPic->hide();
    m_leftMusicShowWidget->minimumWidget();
    m_searchResult->hide();
    QPixmap pix;
    pix.load(":/images/TJnight.png");
    m_middleMusicShow->m_lblImage->setPixmap(pix.scaled(1050, 661, Qt::KeepAspectRatioByExpanding));
    m_middleMusicShow->m_lblImage->setGeometry(0,0,1050,661);
}
