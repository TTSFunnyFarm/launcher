#ifndef FRAMELESSWINDOW_H
#define FRAMELESSWINDOW_H

#include "qsystemdetection.h"
#include <QObject>
#include <QMainWindow>

#ifdef Q_OS_MAC
// Defines the macOS (Cocoa, Objective-C) implementation.
#include <QMouseEvent>
#include <QResizeEvent>
#include <QPoint>

class FramelessWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit FramelessWindow(QWidget *parent = nullptr);

private:
    void initUI();

public:
    // In a draggable area, the window can be moved by the mouse.
    // A height of 0 implies that the entire window is draggable.
    void setDraggableAreaHeight(int height = 0);

    // Native style (minimize/maximize/close buttons, rounded corners,
    // and drop shadow) is available only on OS X 10.10 or later.
    // After init, we should check whether NativeStyle is OK with this
    // function. If not, we should implement minimize/maximize/close
    // buttons ourself.
    bool isNativeStyleOK() {return m_bNativeSystemBtn;}

    // If setCloseBtnQuit is set to false, then the application will
    // hide itself rather than quit when the close button is clicked.
    // Keep in mind that once setCloseBtnQuit is set to false, it can
    // never be reset back to true. This function should be called
    // within the constructor of derived classes, and cannot be called
    // more than once. Only works with OS X 10.10 or later.
    void setCloseBtnQuit(bool bQuit = true);

    // Enables or disables the close button. Only works if isNativeStyleOK()
    // returns true.
    void setCloseBtnEnabled(bool bEnable = true);

    // Enables or disables the minimize button. Only works if isNativeStyleOK()
    // returns true.
    void setMinBtnEnabled(bool bEnable = true);

    // Enables or disables the zoom button. Only works if isNativeStyleOK()
    // returns true.
    void setZoomBtnEnabled(bool bEnable = true);

    bool isCloseBtnEnabled() {return m_bIsCloseBtnEnabled;}
    bool isMinBtnEnabled() {return m_bIsMinBtnEnabled;}
    bool isZoomBtnEnabled() {return m_bIsZoomBtnEnabled;}

    // Hides or shows the close button. Only works if isNativeStyleOK()
    // returns true.
    void setCloseBtnHidden(bool bHidden = false);

    // Hides or shows the minimize button. Only works if isNativeStyleOK()
    // returns true.
    void setMinBtnHidden(bool bHidden = false);

    // Hides or shows the zoom button. Only works if isNativeStyleOK()
    // returns true.
    void setZoomBtnHidden(bool bHidden = false);

    bool isCloseBtnHidden() {return m_bIsCloseBtnHidden;}
    bool isMinBtnHidden() {return m_bIsMinBtnHidden;}
    bool isZoomBtnHidden() {return m_bIsZoomBtnHidden;}

    // Sets resizable flag on the window.
    void setResizable(bool bResizable = false);

    bool isResizable() {return m_bIsResizable;}

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private:
    int m_draggableHeight;
    bool m_bWinMoving;
    bool m_bMousePressed;
    QPoint m_MousePos;
    QPoint m_WindowPos;
    bool m_bCloseBtnQuit;
    bool m_bNativeSystemBtn;
    bool m_bIsCloseBtnEnabled;
    bool m_bIsMinBtnEnabled;
    bool m_bIsZoomBtnEnabled;
    bool m_bIsCloseBtnHidden;
    bool m_bIsMinBtnHidden;
    bool m_bIsZoomBtnHidden;
    bool m_bIsResizable;

private:
    bool m_bTitleBarVisible;

    void setTitlebarVisible(bool bTitleBarVisible = false);
    bool isTitlebarVisible() {return m_bTitleBarVisible;}

private slots:
    void onRestoreFromFullScreen();

signals:
    void restoreFromFullScreen();

protected:
    void resizeEvent(QResizeEvent *event);
};
#endif // Q_OS_MAC

#ifdef Q_OS_WIN
// Defines the Windows (Win32, C++) implementation.
#include <QWidget>
#include <QList>
#include <QMargins>
#include <QRect>

class FramelessWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit FramelessWindow(QWidget *parent = nullptr);

    void setupUi();

protected:
    // Sets a widget that will be treated as the system titlebar.
    void setTitleBar(QWidget* titlebar);

    // When setting a widget as the titlebar, it will cover the titlebar
    // under it. As a result, we cannot drag and move the window with this
    // widget again. We can fix this by adding the widget to an ignore list.
    // This can be achieved by calling addIgnoreWidget and passing the widget
    // that should be added to the ignore list.
    void addIgnoreWidget(QWidget* widget);

    bool nativeEvent(const QByteArray &eventType, void *message, long *result);

private slots:
    void onTitleBarDestroyed();

public:
    void setContentsMargins(const QMargins &margins);
    void setContentsMargins(int left, int top, int right, int bottom);
    QMargins contentsMargins() const;
    QRect contentsRect() const;
    void getContentsMargins(int *left, int *top, int *right, int *bottom) const;

public slots:
    void showFullScreen();

private:
    QWidget* m_titlebar;
    QList<QWidget*> m_whiteList;

    QMargins m_margins;
    QMargins m_frames;
};

#endif // Q_OS_WIN

#endif // FRAMELESSWINDOW_H
