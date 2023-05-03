#include <iostream>
#include <array>
#include <QColor>
#include "qphase/widgets/colorMaps/parula.hpp"
#include "interpolate.hpp"
 
using namespace QPhase::Widgets::ColorMaps;

class Parula::ParulaImpl
{
public:
    std::array<std::pair<double, QColor>, 256> mColorMap
    {
        std::pair {0.0000000, QColor( 62,  38, 168, 255)},
        std::pair {0.0039216, QColor( 62,  39, 172, 255)},
        std::pair {0.0078431, QColor( 63,  40, 175, 255)},
        std::pair {0.0117647, QColor( 63,  41, 178, 255)},
        std::pair {0.0156863, QColor( 64,  42, 180, 255)},
        std::pair {0.0196078, QColor( 64,  43, 183, 255)},
        std::pair {0.0235294, QColor( 65,  44, 186, 255)},
        std::pair {0.0274510, QColor( 65,  45, 189, 255)},
        std::pair {0.0313725, QColor( 66,  46, 191, 255)},
        std::pair {0.0352941, QColor( 66,  47, 194, 255)},
        std::pair {0.0392157, QColor( 67,  48, 197, 255)},
        std::pair {0.0431373, QColor( 67,  49, 200, 255)},
        std::pair {0.0470588, QColor( 67,  50, 202, 255)},
        std::pair {0.0509804, QColor( 68,  51, 205, 255)},
        std::pair {0.0549020, QColor( 68,  52, 208, 255)},
        std::pair {0.0588235, QColor( 69,  53, 210, 255)},
        std::pair {0.0627451, QColor( 69,  55, 213, 255)},
        std::pair {0.0666667, QColor( 69,  56, 215, 255)},
        std::pair {0.0705882, QColor( 70,  57, 217, 255)},
        std::pair {0.0745098, QColor( 70,  58, 220, 255)},
        std::pair {0.0784314, QColor( 70,  59, 222, 255)},
        std::pair {0.0823529, QColor( 70,  61, 224, 255)},
        std::pair {0.0862745, QColor( 71,  62, 225, 255)},
        std::pair {0.0901961, QColor( 71,  63, 227, 255)},
        std::pair {0.0941176, QColor( 71,  65, 229, 255)},
        std::pair {0.0980392, QColor( 71,  66, 230, 255)},
        std::pair {0.1019608, QColor( 71,  68, 232, 255)},
        std::pair {0.1058824, QColor( 71,  69, 233, 255)},
        std::pair {0.1098039, QColor( 71,  70, 235, 255)},
        std::pair {0.1137255, QColor( 72,  72, 236, 255)},
        std::pair {0.1176471, QColor( 72,  73, 237, 255)},
        std::pair {0.1215686, QColor( 72,  75, 238, 255)},
        std::pair {0.1254902, QColor( 72,  76, 240, 255)},
        std::pair {0.1294118, QColor( 72,  78, 241, 255)},
        std::pair {0.1333333, QColor( 72,  79, 242, 255)},
        std::pair {0.1372549, QColor( 72,  80, 243, 255)},
        std::pair {0.1411765, QColor( 72,  82, 244, 255)},
        std::pair {0.1450980, QColor( 72,  83, 245, 255)},
        std::pair {0.1490196, QColor( 72,  84, 246, 255)},
        std::pair {0.1529412, QColor( 71,  86, 247, 255)},
        std::pair {0.1568627, QColor( 71,  87, 247, 255)},
        std::pair {0.1607843, QColor( 71,  89, 248, 255)},
        std::pair {0.1647059, QColor( 71,  90, 249, 255)},
        std::pair {0.1686275, QColor( 71,  91, 250, 255)},
        std::pair {0.1725490, QColor( 71,  93, 250, 255)},
        std::pair {0.1764706, QColor( 70,  94, 251, 255)},
        std::pair {0.1803922, QColor( 70,  96, 251, 255)},
        std::pair {0.1843137, QColor( 70,  97, 252, 255)},
        std::pair {0.1882353, QColor( 69,  98, 252, 255)},
        std::pair {0.1921569, QColor( 69, 100, 253, 255)},
        std::pair {0.1960784, QColor( 68, 101, 253, 255)},
        std::pair {0.2000000, QColor( 67, 103, 253, 255)},
        std::pair {0.2039216, QColor( 67, 104, 254, 255)},
        std::pair {0.2078431, QColor( 66, 106, 254, 255)},
        std::pair {0.2117647, QColor( 65, 107, 254, 255)},
        std::pair {0.2156863, QColor( 64, 109, 254, 255)},
        std::pair {0.2196078, QColor( 63, 110, 255, 255)},
        std::pair {0.2235294, QColor( 62, 112, 255, 255)},
        std::pair {0.2274510, QColor( 60, 113, 255, 255)},
        std::pair {0.2313725, QColor( 59, 115, 255, 255)},
        std::pair {0.2352941, QColor( 57, 116, 255, 255)},
        std::pair {0.2392157, QColor( 56, 118, 254, 255)},
        std::pair {0.2431373, QColor( 54, 119, 254, 255)},
        std::pair {0.2470588, QColor( 53, 121, 253, 255)},
        std::pair {0.2509804, QColor( 51, 122, 253, 255)},
        std::pair {0.2549020, QColor( 50, 124, 252, 255)},
        std::pair {0.2588235, QColor( 49, 125, 252, 255)},
        std::pair {0.2627451, QColor( 48, 127, 251, 255)},
        std::pair {0.2666667, QColor( 47, 128, 250, 255)},
        std::pair {0.2705882, QColor( 47, 130, 250, 255)},
        std::pair {0.2745098, QColor( 46, 131, 249, 255)},
        std::pair {0.2784314, QColor( 46, 132, 248, 255)},
        std::pair {0.2823529, QColor( 46, 134, 248, 255)},
        std::pair {0.2862745, QColor( 46, 135, 247, 255)},
        std::pair {0.2901961, QColor( 45, 136, 246, 255)},
        std::pair {0.2941176, QColor( 45, 138, 245, 255)},
        std::pair {0.2980392, QColor( 45, 139, 244, 255)},
        std::pair {0.3019608, QColor( 45, 140, 243, 255)},
        std::pair {0.3058824, QColor( 45, 142, 242, 255)},
        std::pair {0.3098039, QColor( 44, 143, 241, 255)},
        std::pair {0.3137255, QColor( 44, 144, 240, 255)},
        std::pair {0.3176471, QColor( 43, 145, 239, 255)},
        std::pair {0.3215686, QColor( 42, 147, 238, 255)},
        std::pair {0.3254902, QColor( 41, 148, 237, 255)},
        std::pair {0.3294118, QColor( 40, 149, 236, 255)},
        std::pair {0.3333333, QColor( 39, 151, 235, 255)},
        std::pair {0.3372549, QColor( 39, 152, 234, 255)},
        std::pair {0.3411765, QColor( 38, 153, 233, 255)},
        std::pair {0.3450980, QColor( 38, 154, 232, 255)},
        std::pair {0.3490196, QColor( 37, 155, 232, 255)},
        std::pair {0.3529412, QColor( 37, 156, 231, 255)},
        std::pair {0.3568627, QColor( 36, 158, 230, 255)},
        std::pair {0.3607843, QColor( 36, 159, 229, 255)},
        std::pair {0.3647059, QColor( 35, 160, 229, 255)},
        std::pair {0.3686275, QColor( 35, 161, 228, 255)},
        std::pair {0.3725490, QColor( 34, 162, 228, 255)},
        std::pair {0.3764706, QColor( 33, 163, 227, 255)},
        std::pair {0.3803922, QColor( 32, 165, 227, 255)},
        std::pair {0.3843137, QColor( 31, 166, 226, 255)},
        std::pair {0.3882353, QColor( 30, 167, 225, 255)},
        std::pair {0.3921569, QColor( 29, 168, 225, 255)},
        std::pair {0.3960784, QColor( 29, 169, 224, 255)},
        std::pair {0.4000000, QColor( 28, 170, 223, 255)},
        std::pair {0.4039216, QColor( 27, 171, 222, 255)},
        std::pair {0.4078431, QColor( 26, 172, 221, 255)},
        std::pair {0.4117647, QColor( 25, 173, 220, 255)},
        std::pair {0.4156863, QColor( 23, 174, 218, 255)},
        std::pair {0.4196078, QColor( 22, 175, 217, 255)},
        std::pair {0.4235294, QColor( 20, 176, 216, 255)},
        std::pair {0.4274510, QColor( 18, 177, 214, 255)},
        std::pair {0.4313725, QColor( 16, 178, 213, 255)},
        std::pair {0.4352941, QColor( 14, 179, 212, 255)},
        std::pair {0.4392157, QColor( 11, 179, 210, 255)},
        std::pair {0.4431373, QColor(  8, 180, 209, 255)},
        std::pair {0.4470588, QColor(  6, 181, 207, 255)},
        std::pair {0.4509804, QColor(  4, 182, 206, 255)},
        std::pair {0.4549020, QColor(  2, 183, 204, 255)},
        std::pair {0.4588235, QColor(  1, 183, 202, 255)},
        std::pair {0.4627451, QColor(  0, 184, 201, 255)},
        std::pair {0.4666667, QColor(  0, 185, 199, 255)},
        std::pair {0.4705882, QColor(  0, 186, 198, 255)},
        std::pair {0.4745098, QColor(  1, 186, 196, 255)},
        std::pair {0.4784314, QColor(  2, 187, 194, 255)},
        std::pair {0.4823529, QColor(  4, 187, 193, 255)},
        std::pair {0.4862745, QColor(  6, 188, 191, 255)},
        std::pair {0.4901961, QColor(  9, 189, 189, 255)},
        std::pair {0.4941176, QColor( 13, 189, 188, 255)},
        std::pair {0.4980392, QColor( 16, 190, 186, 255)},
        std::pair {0.5019608, QColor( 20, 190, 184, 255)},
        std::pair {0.5058824, QColor( 23, 191, 182, 255)},
        std::pair {0.5098039, QColor( 26, 192, 181, 255)},
        std::pair {0.5137255, QColor( 29, 192, 179, 255)},
        std::pair {0.5176471, QColor( 32, 193, 177, 255)},
        std::pair {0.5215686, QColor( 35, 193, 175, 255)},
        std::pair {0.5254902, QColor( 37, 194, 174, 255)},
        std::pair {0.5294118, QColor( 39, 194, 172, 255)},
        std::pair {0.5333333, QColor( 41, 195, 170, 255)},
        std::pair {0.5372549, QColor( 43, 195, 168, 255)},
        std::pair {0.5411765, QColor( 44, 196, 166, 255)},
        std::pair {0.5450980, QColor( 46, 196, 165, 255)},
        std::pair {0.5490196, QColor( 47, 197, 163, 255)},
        std::pair {0.5529412, QColor( 49, 197, 161, 255)},
        std::pair {0.5568627, QColor( 50, 198, 159, 255)},
        std::pair {0.5607843, QColor( 51, 199, 157, 255)},
        std::pair {0.5647059, QColor( 53, 199, 155, 255)},
        std::pair {0.5686275, QColor( 54, 200, 153, 255)},
        std::pair {0.5725490, QColor( 56, 200, 150, 255)},
        std::pair {0.5764706, QColor( 57, 201, 148, 255)},
        std::pair {0.5803922, QColor( 59, 201, 146, 255)},
        std::pair {0.5843137, QColor( 61, 202, 144, 255)},
        std::pair {0.5882353, QColor( 64, 202, 141, 255)},
        std::pair {0.5921569, QColor( 66, 202, 139, 255)},
        std::pair {0.5960784, QColor( 69, 203, 137, 255)},
        std::pair {0.6000000, QColor( 72, 203, 134, 255)},
        std::pair {0.6039216, QColor( 75, 203, 132, 255)},
        std::pair {0.6078431, QColor( 78, 204, 129, 255)},
        std::pair {0.6117647, QColor( 81, 204, 127, 255)},
        std::pair {0.6156863, QColor( 84, 204, 124, 255)},
        std::pair {0.6196078, QColor( 87, 204, 122, 255)},
        std::pair {0.6235294, QColor( 90, 204, 119, 255)},
        std::pair {0.6274510, QColor( 94, 205, 116, 255)},
        std::pair {0.6313725, QColor( 97, 205, 114, 255)},
        std::pair {0.6352941, QColor(100, 205, 111, 255)},
        std::pair {0.6392157, QColor(103, 205, 108, 255)},
        std::pair {0.6431373, QColor(107, 205, 105, 255)},
        std::pair {0.6470588, QColor(110, 205, 102, 255)},
        std::pair {0.6509804, QColor(114, 205, 100, 255)},
        std::pair {0.6549020, QColor(118, 204,  97, 255)},
        std::pair {0.6588235, QColor(121, 204,  94, 255)},
        std::pair {0.6627451, QColor(125, 204,  91, 255)},
        std::pair {0.6666667, QColor(129, 204,  89, 255)},
        std::pair {0.6705882, QColor(132, 204,  86, 255)},
        std::pair {0.6745098, QColor(136, 203,  83, 255)},
        std::pair {0.6784314, QColor(139, 203,  81, 255)},
        std::pair {0.6823529, QColor(143, 203,  78, 255)},
        std::pair {0.6862745, QColor(147, 202,  75, 255)},
        std::pair {0.6901961, QColor(150, 202,  72, 255)},
        std::pair {0.6941176, QColor(154, 201,  70, 255)},
        std::pair {0.6980392, QColor(157, 201,  67, 255)},
        std::pair {0.7019608, QColor(161, 200,  64, 255)},
        std::pair {0.7058824, QColor(164, 200,  62, 255)},
        std::pair {0.7098039, QColor(167, 199,  59, 255)},
        std::pair {0.7137255, QColor(171, 199,  57, 255)},
        std::pair {0.7176471, QColor(174, 198,  55, 255)},
        std::pair {0.7215686, QColor(178, 198,  53, 255)},
        std::pair {0.7254902, QColor(181, 197,  51, 255)},
        std::pair {0.7294118, QColor(184, 196,  49, 255)},
        std::pair {0.7333333, QColor(187, 196,  47, 255)},
        std::pair {0.7372549, QColor(190, 195,  45, 255)},
        std::pair {0.7411765, QColor(194, 195,  44, 255)},
        std::pair {0.7450980, QColor(197, 194,  42, 255)},
        std::pair {0.7490196, QColor(200, 193,  41, 255)},
        std::pair {0.7529412, QColor(203, 193,  40, 255)},
        std::pair {0.7568627, QColor(206, 192,  39, 255)},
        std::pair {0.7607843, QColor(208, 191,  39, 255)},
        std::pair {0.7647059, QColor(211, 191,  39, 255)},
        std::pair {0.7686275, QColor(214, 190,  39, 255)},
        std::pair {0.7725490, QColor(217, 190,  40, 255)},
        std::pair {0.7764706, QColor(219, 189,  40, 255)},
        std::pair {0.7803922, QColor(222, 188,  41, 255)},
        std::pair {0.7843137, QColor(225, 188,  42, 255)},
        std::pair {0.7882353, QColor(227, 188,  43, 255)},
        std::pair {0.7921569, QColor(230, 187,  45, 255)},
        std::pair {0.7960784, QColor(232, 187,  46, 255)},
        std::pair {0.8000000, QColor(234, 186,  48, 255)},
        std::pair {0.8039216, QColor(236, 186,  50, 255)},
        std::pair {0.8078431, QColor(239, 186,  53, 255)},
        std::pair {0.8117647, QColor(241, 186,  55, 255)},
        std::pair {0.8156863, QColor(243, 186,  57, 255)},
        std::pair {0.8196078, QColor(245, 186,  59, 255)},
        std::pair {0.8235294, QColor(247, 186,  61, 255)},
        std::pair {0.8274510, QColor(249, 186,  62, 255)},
        std::pair {0.8313725, QColor(251, 187,  62, 255)},
        std::pair {0.8352941, QColor(252, 188,  62, 255)},
        std::pair {0.8392157, QColor(254, 189,  61, 255)},
        std::pair {0.8431373, QColor(254, 190,  60, 255)},
        std::pair {0.8470588, QColor(254, 192,  59, 255)},
        std::pair {0.8509804, QColor(254, 193,  58, 255)},
        std::pair {0.8549020, QColor(254, 194,  57, 255)},
        std::pair {0.8588235, QColor(254, 196,  56, 255)},
        std::pair {0.8627451, QColor(254, 197,  55, 255)},
        std::pair {0.8666667, QColor(254, 199,  53, 255)},
        std::pair {0.8705882, QColor(254, 200,  52, 255)},
        std::pair {0.8745098, QColor(254, 202,  51, 255)},
        std::pair {0.8784314, QColor(253, 203,  50, 255)},
        std::pair {0.8823529, QColor(253, 205,  49, 255)},
        std::pair {0.8862745, QColor(253, 206,  49, 255)},
        std::pair {0.8901961, QColor(252, 208,  48, 255)},
        std::pair {0.8941176, QColor(251, 210,  47, 255)},
        std::pair {0.8980392, QColor(251, 211,  46, 255)},
        std::pair {0.9019608, QColor(250, 213,  46, 255)},
        std::pair {0.9058824, QColor(249, 214,  45, 255)},
        std::pair {0.9098039, QColor(249, 216,  44, 255)},
        std::pair {0.9137255, QColor(248, 217,  43, 255)},
        std::pair {0.9176471, QColor(247, 219,  42, 255)},
        std::pair {0.9215686, QColor(247, 221,  42, 255)},
        std::pair {0.9254902, QColor(246, 222,  41, 255)},
        std::pair {0.9294118, QColor(246, 224,  40, 255)},
        std::pair {0.9333333, QColor(245, 225,  40, 255)},
        std::pair {0.9372549, QColor(245, 227,  39, 255)},
        std::pair {0.9411765, QColor(245, 229,  38, 255)},
        std::pair {0.9450980, QColor(245, 230,  38, 255)},
        std::pair {0.9490196, QColor(245, 232,  37, 255)},
        std::pair {0.9529412, QColor(245, 233,  36, 255)},
        std::pair {0.9568627, QColor(245, 235,  35, 255)},
        std::pair {0.9607843, QColor(245, 236,  34, 255)},
        std::pair {0.9647059, QColor(245, 238,  33, 255)},
        std::pair {0.9686275, QColor(246, 239,  32, 255)},
        std::pair {0.9725490, QColor(246, 241,  31, 255)},
        std::pair {0.9764706, QColor(246, 242,  30, 255)},
        std::pair {0.9803922, QColor(247, 244,  28, 255)},
        std::pair {0.9843137, QColor(247, 245,  27, 255)},
        std::pair {0.9882353, QColor(248, 247,  26, 255)},
        std::pair {0.9921569, QColor(248, 248,  24, 255)},
        std::pair {0.9960784, QColor(249, 249,  22, 255)},
        std::pair {1.0000000, QColor(249, 251,  21, 255)}
    };
    std::pair<double, double> mRange{0, 1};
    InterpolationMethod mMethod{InterpolationMethod::Linear};
    bool mInitialized{false};
};

/// Constructor
Parula::Parula() :
    pImpl(std::make_unique<ParulaImpl> ())
{
}

/// Copy constructor
Parula::Parula(const Parula &colorMap)
{
    *this = colorMap;
}

/// Move constructor
Parula::Parula(Parula &&colorMap) noexcept
{
    *this = std::move(colorMap);
}

/// Copy assignment
Parula& Parula::operator=(const Parula &colorMap)
{
    if (&colorMap == this){return *this;}
    pImpl = std::make_unique<ParulaImpl> (*colorMap.pImpl);
    return *this;
}

/// Move assignment
Parula& Parula::operator=(Parula &&colorMap) noexcept
{
    if (&colorMap == this){return *this;}
    pImpl = std::move(colorMap.pImpl);
    return *this;
}

/// Destructor
Parula::~Parula() = default;

/// Initialized
bool Parula::isInitialized() const noexcept
{
    return pImpl->mInitialized;
} 

/// Initialize
void Parula::initialize(const std::pair<double, double> &range,
                        const InterpolationMethod method)
{
    if (range.first >= range.second)
    {
        throw std::invalid_argument(
            "range.first must be less than range.second");
    }
    pImpl->mRange = range;
    pImpl->mMethod = method;
    pImpl->mInitialized = true;
}

/// Interpolate
QColor Parula::evaluate(const double value) const
{
    if (!isInitialized()){throw std::runtime_error("Parula not initalized");}
    return ::interpolate(value, pImpl->mRange,
                         pImpl->mColorMap.size(), pImpl->mColorMap.data(),
                         pImpl->mMethod);
}
