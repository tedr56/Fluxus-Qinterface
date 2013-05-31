#ifndef FLUXUSHEAD_H
#define FLUXUSHEAD_H

#include "heads.h"
#include <QFileInfo>

class FluxusHead : public Head
{
public:
    FluxusHead(QObject *parent);
    FluxusHead(bool interface, bool fluxus, QObject *parent);
    virtual QList<Media*> getMedia();
    virtual bool updateMedia(Media* updateMedia);

    bool setFluxusBrosDirectory(QString fbDir);
    int setFluxusMaterialsDirectory(QString fmDir);

    bool accessConfigFile();
    bool accessFile(QFileInfo* targetFile);
    bool accessMedia(Media* targetMedia);

private:
    bool m_interface;
    bool m_fluxus;
    QFileInfo configFile;
    QFileInfo m_fluxusbros_directory;
    QFileInfo m_fluxus_materials;
};

#endif // FLUXUSHEAD_H
