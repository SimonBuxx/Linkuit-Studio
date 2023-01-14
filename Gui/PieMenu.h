/**
 * @file PieMenu.h
 * @author Simon Buchholz
 * @date 9 Jan 2023
 * @brief A simple pie menu QWidget implementation
 *
 * The pie menu is fully customizable for arbitrary amounts of buttons,
 * disabled buttons, button icons and appearance parameters.
 * All button clicks are published as a Qt signal containing the
 * clicked button's index.
 *
 * When the pin/unpin button is activated, the pie menu can be pinned
 * to keep it open until it is manually or programmatically closed.
 */

#ifndef PIEMENU_H
#define PIEMENU_H

#include <QWidget>
#include <QPainterPath>

/// \brief A simple pie menu widget for Qt
class PieMenu : public QWidget
{
    Q_OBJECT
public:
    /// \brief Constructor of the PieMenu widget
    /// \param parent: Pointer to the parent widget
    explicit PieMenu(QWidget *pParent = nullptr);

    /// \brief Hides the pie menu if it is not pinned
    void HideIfNotPinned(void);

    /// \brief Displays the pie menu at the current mouse position
    /// Note: the position is mapped to the parent coordinate system
    void Display(void);

    /// \brief Sets the default and disabled icons of the pie buttons
    /// \param pIndex: The index of the button
    /// \param pIconImage: Reference to the image for the icon
    void SetButtonIcon(uint8_t pIndex, const QImage& pIconImage);

    /// \brief Sets the icon of the close button
    /// \param pIcon: Reference to the icon
    void SetCloseButtonIcon(const QIcon& pIcon);

    /// \brief Sets the icon of the pin button
    /// \param pIcon: Reference to the icon
    void SetPinButtonIcon(const QIcon& pIcon);

    /// \brief Sets the enabled state of the button with the given index
    /// \param pIndex: The index of the button to be altered
    /// \param pEnable: Whether the button should be enabled or disabled
    void SetButtonEnabled(uint8_t pIndex, bool pEnable);

signals:
    /// \brief Emitted when one of the pie menu buttons is clicked
    /// \param pIndex: The index of the clicked button
    void ButtonClickedSignal(uint8_t pIndex);

protected:
    /// \brief Creates QPainterPath objects for the pie button shapes
    void InitPainterPaths(void);

    /// \brief Calculates the index of the button that the mouse is over
    /// The buttons are numbered from 0 to n, index n+1 is the close button
    /// and index n+2 is the pin/unpin button
    /// \return The button index or -1, if not on a button
    int8_t GetButtonUnderMouse(void) const;

    /// \brief Event handler to paint the widget
    /// \param Pointer to the paint event
    void paintEvent(QPaintEvent *pEvent) override;

    /// \brief Paints the custom-shaped pie menu buttons
    /// \param pPainter: Reference to the QPainter
    /// \param pMouseover: The index of the button that the mouse is over
    void paintPieButtons(QPainter& pPainter, int8_t pMouseover);

    /// \brief Paints the close button in the center of the pie menu
    /// \param pPainter: Reference to the QPainter
    /// \param pMouseover: Whether the mouse is over the button or not
    void paintCloseButton(QPainter& pPainter, bool pMouseover);

    /// \brief Paints the pin/unpin button in the top right corner
    /// \param pPainter: Reference to the QPainter
    /// \param pMouseover: Whether the mouse is over the button or not
    void paintPinButton(QPainter& pPainter, bool pMouseover);

    /// \brief Event handler to process mouse clicks
    /// \param pEvent: Pointer to the mouse event
    void mouseReleaseEvent(QMouseEvent *pEvent) override;

    /// \brief Event handler to update on mouse press
    /// \param pEvent: Pointer to the mouse event
    void mousePressEvent(QMouseEvent *pEvent) override;

    /// \brief Event handler to update on mouse move
    /// \param pEvent: Pointer to the mouse event
    void mouseMoveEvent(QMouseEvent *pEvent) override;

    /// \brief Event handler to update when the mouse leaves the widget
    /// \param pEvent: Pointer to the mouse event
    void leaveEvent(QEvent *pEvent) override;

protected:
    /// \brief The amount of pie buttons the pie menu will have
    uint8_t mButtonCount = 4;

    /// \brief Vector containing the default icons for the pie buttons
    std::vector<QIcon> mDefaultButtonIcons;

    /// \brief Vector containing the icons for the disabled pie buttons
    std::vector<QIcon> mDisabledButtonIcons;

    /// \brief Icon for the close icon in the center of the pie menu
    QIcon mCloseIcon;

    /// \brief Icon for the pin/unpin icon
    QIcon mPinIcon;

    /// \brief Vector containing the painter paths of the custom pie menu button shapes
    std::vector<QPainterPath> mPieButtonPaths;

    /// \brief Vector containing the button enable state of the pie menu buttons
    std::vector<bool> mButtonsEnabled;

    /// \brief Whether there should be a pin/unpin button
    bool mShowPinButton = true;

    /// \brief Whether the pie menu is currently pinned or not
    bool mIsPinned = false;

    /// \brief The stroke width of all buttons
    uint8_t mStrokeWidth = 1;

    /// \brief The radius of the pie menu
    uint32_t mPieRadius = 80;

    /// \brief The size of the widget excluding border strokes
    QSize mBaseSize;

    /// \brief The size of the widget including border strokes
    QSize mFullSize;

    /// \brief The angle of each pie button in degrees
    qreal mAnglePerButton;

    /// \brief The base angle (constant offset) of the pie buttons
    qreal mBaseAngle = 45;

    /// \brief The radius of the close button in pixels
    uint8_t mCloseButtonRadius = 25;

    /// \brief The button index of the close button
    uint8_t mCloseButtonIndex;

    /// \brief The radius of the pin/unpin button in pixels
    uint8_t mPinButtonRadius = 11;

    /// \brief The button index of the pin/unpin button
    uint8_t mPinButtonIndex;

    /// \brief Whether the button colors should alternate or not
    bool mAlternateColors = true;

    /// \brief The size of the pie menu button icons in pixels
    uint8_t mPieIconSize = 24;

    /// \brief The size of the close button icon in pixels
    uint8_t mCloseIconSize = 24;

    /// \brief The size of the pin/unpin button icon in pixels
    uint8_t mPinIconSize = 12;
};

#endif // PIEMENU_H
