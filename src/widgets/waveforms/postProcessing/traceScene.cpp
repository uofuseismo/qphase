#include <cmath>
#include <QColor>
#include <QFont>
#include <QString>
#include "qphase/widgets/waveforms/postProcessing/traceScene.hpp"

using namespace QPhase::Widgets::Waveforms::PostProcessing;

class TraceScene::TraceSceneImpl
{
public:
    TraceSceneImpl(const int traceWidth,
                   const int traceHeight) :
        mTraceWidth(std::max(200, traceWidth)),
        mTraceHeight(std::max(20, traceHeight))
    {
        mBackgroundFont.setStyleHint(QFont::Monospace);
        mBackgroundFont.setPointSize(22);
        mBackgroundFont.setWeight(QFont::Light);
        mBackgroundFont.setItalic(false);
    }

    /// Recomputes the trace height given the current plot size
    void recomputeTraceHeight()
    {
        auto availableHeight = static_cast<double> (mCurrentSize.height());
        int denominator = 1;
        //if (!mChannelList.empty())
        //{
        //    denominator = std::min(static_cast<int> (mChannelList.size()),
        //                           mMaxTracesPerScene);
        //}
        mTraceHeight
            = static_cast<int> (std::floor(availableHeight/denominator));
    }
///private:
    QSize mCurrentSize;
    QColor mBackgroundColor{Qt::white};
    QString mBackgroundName{tr("Trace Viewer")};
    QFont mBackgroundFont;
    int mTraceWidth{400};
    int mTraceHeight{150};
};


/// C'tor
TraceScene::TraceScene(const int traceWidth,
                       const int traceHeight,
                       QObject *parent) :
    QGraphicsScene(parent),
    pImpl(std::make_unique<TraceSceneImpl> (traceWidth, traceHeight))
{
    pImpl->mCurrentSize = QSize(width(), height());
    setBackgroundBrush(pImpl->mBackgroundColor);
    populateScene();
}

/// Destructor
TraceScene::~TraceScene() = default;

/// Resize event
void TraceScene::resize(const QSize &newSize)
{
    pImpl->mCurrentSize = newSize;
    pImpl->mTraceWidth = newSize.width();
    pImpl->recomputeTraceHeight();
    populateScene(); // replot
}

/// Populate scene 
void TraceScene::populateScene()
{
    bool drawBackgroundOnly = true;
    if (drawBackgroundOnly)
    {
        addSimpleText(pImpl->mBackgroundName, pImpl->mBackgroundFont);
    }
    else
    {

    }
}

