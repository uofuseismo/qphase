#ifndef UI_MAP_QGVMAPBOX_HPP
#define UI_MAP_QGVMAPBOX_HPP
#include <QGeoView/QGVLayerTilesOnline.h>
#include <QLocale>
#include <QString>
class QGV_LIB_DECL QGVLayerMapBox : public QGVLayerTilesOnline
{
    Q_OBJECT
public:
    explicit QGVLayerMapBox(QGV::TilesType type = QGV::TilesType::Satellite,
                            QLocale locale = QLocale());
    [[nodiscard]] QString getUrl() const;
    void setType(QGV::TilesType type);
    [[nodiscard]] QGV::TilesType getType() const;
    void setLocale(const QLocale &locale);
    [[nodiscard]] QLocale getLocale() const;
private:
    void createName();
    int minZoomlevel() const override;
    int maxZoomlevel() const override;
    [[nodiscard]] QString tilePosToUrl(const QGV::GeoTilePos& tilePos) const override;
private:
    QString mUrl;
    QGV::TilesType mType;
    QLocale mLocale;
    QString mAccessToken;
    const int mServerNumber = 0;
};
#endif
