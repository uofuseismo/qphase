#ifndef QPHASE_WIDGETS_MAP_MAIN_WINDOW_HPP
#define QPHASE_WIDGETS_MAP_MAIN_WINDOW_HPP
#include <memory>
#include <QMainWindow>
#include <qphase/widgets/map/map.hpp>
namespace QPhase::Widgets::Map
{
class Map;
}
namespace QPhase::Widgets::Map
{
/// @class MainWindow "mainWindow.hpp" "map/mainWindow.hpp"
/// @brief Creates the map viewer pop up window.
/// @copyright Ben Baker (University of Utah) distributed under the MIT license.
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    /// @name Constructors
    /// @{

    /// @brief Constructor.
    explicit MainWindow(QWidget *parent = nullptr);
    /// @}

    /// @result A pointer to the underlying map.
    [[nodiscard]] Map *getMapPointer();

    /// @brief Destructor.
    ~MainWindow() override;

    MainWindow(const MainWindow &) = delete;
    MainWindow(MainWindow &&) noexcept = delete;
    MainWindow &operator=(const MainWindow &) = delete;
    MainWindow &operator=(MainWindow &&) noexcept = delete;
private:
    void createMenus();
    Map *mMap{nullptr};
};
}
#endif
