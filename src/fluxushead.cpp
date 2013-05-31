#include "fluxushead.h"
#include "qobjecthelper.h"


FluxusHead::FluxusHead(QObject *parent) :
    Head(parent)
{
}

FluxusHead::FluxusHead(bool interface, bool fluxus, QObject *parent) :
    Head(parent),
    m_interface(interface),
    m_fluxus(fluxus)
{
}

QList<Media *> FluxusHead::getMedia()
{

}

bool FluxusHead::updateMedia(Media *updateMedia)
{
}

bool FluxusHead::setFluxusBrosDirectory(QString fbDir)
{
    QFileInfo flxBrosInfo(fbDir);
    if (flxBrosInfo.exists()) {
        QDir flxBrosDir(fbDir);
        if (flxBrosDir.exists("config.json")) {
            QFileInfo flxBrosConfig(flxBrosDir.filePath("config.json"));
            if (flxBrosConfig.isReadable()) {
                configFile = flxBrosConfig;
                return true;
            }
        }
    }
    return false;
}

bool FluxusHead::accessConfigFile()
{
    return configFile.isWritable();
}

bool FluxusHead::accessFile(QFileInfo *targetFile)
{
    if (targetFile->exists()) {
        return targetFile->isReadable();
    } else {
        return false;
    }
}

bool FluxusHead::accessMedia(Media *targetMedia)
{
    targetMedia->getType();
}

