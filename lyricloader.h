#ifndef LYRICLOADER_H
#define LYRICLOADER_H

#include <QObject>
#include <QPointer>
#include <QStringList>
#include "lyricline.h"
namespace QJson { class Parser; }

class QNetworkAccessManager;
class QNetworkReply;

class LyricLoader : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList lyric READ lyric NOTIFY lyricChanged)
    Q_PROPERTY(bool loading READ loading NOTIFY loadingChanged)
    Q_PROPERTY(bool hasTimer READ hasTimer NOTIFY lyricChanged)
public:
    //explicit LyricLoader(QObject *parent = 0);
    //~LyricLoader();

    //void classBegin();
    //void componentComplete();

    Q_INVOKABLE bool loadFromFile(const QString& fileName);
    //Q_INVOKABLE void loadFromMusicId(const QString& musicId);
    //Q_INVOKABLE void saveToFile(const QString& fileName);
    Q_INVOKABLE int getLineByPosition(const int& millisec, const int& startPos = 0) const;
    Q_INVOKABLE bool dataAvailable() const;

    QStringList lyric() const;
    bool hasTimer() const;
    bool loading() const;
public:
    QList<LyricLine*> getAllLine();

signals:
    void lyricChanged();
    void loadingChanged();

private:
    void reset();
    bool processContent(const QString& content);

private slots:
    //void requestFinished();

private:
    QJson::Parser* mParser;
    QNetworkAccessManager* mNetworkManager;
    QPointer<QNetworkReply> mReply;

    QString mRawData;
    QList<LyricLine*> mLines;
    bool mHasTimer;

    bool isComponentComplete;
};

#endif // LYRICLOADER_H
