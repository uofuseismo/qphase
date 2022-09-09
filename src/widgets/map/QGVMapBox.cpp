#include <QString>
#include "private/haveMap.hpp"
#include "qphase/widgets/map/QGVMapBox.hpp"

namespace
{
const QMap<QGV::TilesType, QStringList> URLTemplates =
{
    {   
        QGV::TilesType::Satellite,
        {
             "https://api.mapbox.com/styles/v1/mapbox/outdoors-v9/tiles/${z}/${x}/${y}?access_token=${qk}"
        }
    },
};
}

/// C'tor
QGVLayerMapBox::QGVLayerMapBox(QGV::TilesType type,
                               const QLocale locale) :
    mType(type),
    mLocale(locale)
{
    // Try to get the token from the environment
    std::string token;
    if (std::getenv("QPHASE_MAPBOX_TOKEN") != nullptr)
    {
        token = std::string {std::getenv("QPHASE_MAPBOX_TOKEN")};
    }
    else
    {
        qDebug() << "QPHASE_MAPBOX_TOKEN environment variable not set.";
    }
    // Otherwise use the install token
    if (token.empty())
    {
        token = QPHASE_MAPBOX_TOKEN;
    }
    mAccessToken = QString::fromStdString(token);
    createName();
}

void QGVLayerMapBox::setType(QGV::TilesType type)
{
    mType = type;
    createName();
}

void QGVLayerMapBox::setLocale(const QLocale &locale)
{
    mLocale = locale;
    createName();
}

void QGVLayerMapBox::createName()
{
    // clang-format off
    const QMap<QGV::TilesType, QString> adapter = {
        { QGV::TilesType::Satellite, "QGV::Satellite-Streets" },
        //{ QGV::TilesType::Schema, "QGV::Schema" },
        //{ QGV::TilesType::Hybrid, "QGV::Hybrid" },
    };
    // clang-format on
    setName("MapBox Maps (" + adapter[mType] + " " + mLocale.name() + ")");
}

QString QGVLayerMapBox::getUrl() const
{
    return mUrl;
}

QLocale QGVLayerMapBox::getLocale() const
{
    return mLocale;
}

int QGVLayerMapBox::minZoomlevel() const
{
    return 0;
}

int QGVLayerMapBox::maxZoomlevel() const
{
    return 20;
}

QString QGVLayerMapBox::tilePosToUrl(const QGV::GeoTilePos &tilePos) const
{
    const QStringList &list = URLTemplates[mType];
    QString url = list.value(mServerNumber).toLower();
    url.replace("${z}", QString::number(tilePos.zoom()));
    url.replace("${x}", QString::number(tilePos.pos().x()));
    url.replace("${y}", QString::number(tilePos.pos().y()));
    url.replace("${qk}", mAccessToken);
    return url;
}
