#include "src/ui/FramelessWindow.h"
#include <QApplication>
#include <QPoint>
#include <QSize>

#ifdef Q_OS_WIN
#include <Windows.h>
#include <WinUser.h>
#include <windowsx.h>
#include <dwmapi.h>
#include <ObjIdl.h>
#include <gdiplus.h>
#include <gdipluscolor.h>
#pragma comment (lib, "Dwmapi.lib")
#pragma comment (lib, "user32.lib")

FramelessWindow::FramelessWindow(QWidget *parent) :
    QMainWindow(parent),
    m_titlebar(Q_NULLPTR)
{
}

void FramelessWindow::setupUi()
{
    Qt::WindowFlags winFlags = windowFlags();
    winFlags = winFlags | Qt::Window | Qt::FramelessWindowHint | Qt::WindowSystemMenuHint;
    winFlags = winFlags & ~Qt::WindowMaximizeButtonHint;
    setWindowFlags(winFlags);

    bool visible = isVisible();

    // The following code will restore the titlebar and Aero, which is what we want.
    // We will get rid of the titlebar again in nativeEvent() later.
    HWND hwnd = HWND(this->winId());
    DWORD style = DWORD(::GetWindowLong(hwnd, GWL_STYLE));
    style = style | WS_CAPTION;
    style = style & (ULONG(~WS_SYSMENU));
    ::SetWindowLong(hwnd, GWL_STYLE, LONG(style));

    // We better leave 1 pixel width of border untouched, so that Windows
    // can draw a nice shadow around it.
    const MARGINS shadow = { 1, 1, 1, 1 };
    DwmExtendFrameIntoClientArea(HWND(winId()), &shadow);

    setVisible(visible);
}

void FramelessWindow::setTitleBar(QWidget* titlebar)
{
    m_titlebar = titlebar;
    if (!titlebar)
    {
        return;
    }

    connect(titlebar, SIGNAL(destroyed(QObject*)), this, SLOT(onTitleBarDestroyed()));
}

void FramelessWindow::onTitleBarDestroyed()
{
    if (m_titlebar == QObject::sender())
    {
        m_titlebar = Q_NULLPTR;
    }
}

void FramelessWindow::addIgnoreWidget(QWidget* widget)
{
    if (!widget)
    {
        return;
    }

    if (m_whiteList.contains(widget))
    {
        return;
    }

    m_whiteList.append(widget);
}

bool FramelessWindow::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    MSG* msg = reinterpret_cast<MSG*>(message);
    switch (msg->message)
    {
        case WM_NCCALCSIZE:
        {
            NCCALCSIZE_PARAMS& params = *reinterpret_cast<NCCALCSIZE_PARAMS*>(msg->lParam);
            if (params.rgrc[0].top != 0)
            {
                params.rgrc[0].top -= 1;
            }

            // This kills the title bar that we added with WS_CAPTION.
            *result = WVR_REDRAW;
            return true;
        } // end case WM_NCCALCSIZE
        case WM_NCHITTEST:
        {
            // We're not handling resizing as we don't actually want
            // to allow window resizing at all.
            *result = 0;

            long x = GET_X_LPARAM(msg->lParam);
            long y = GET_Y_LPARAM(msg->lParam);

            // Assume that the cursor is either located outside
            // the frame area, or within the titlebar area.
            if (!m_titlebar)
            {
                return false;
            }

            // Support high DPI:
            double dpr = this->devicePixelRatioF();
            QPoint pos = m_titlebar->mapFromGlobal(QPoint(int(x / dpr), int(y / dpr)));

            if (!m_titlebar->rect().contains(pos))
            {
                return false;
            }

            QWidget* child = m_titlebar->childAt(pos);
            if (!child)
            {
                *result = HTCAPTION;
                return true;
            }
            else
            {
                if (m_whiteList.contains(child))
                {
                    *result = HTCAPTION;
                    return true;
                }
            }

            return false;
        } // end case WM_NCHITTEST
        case WM_GETMINMAXINFO:
        {
            // We don't want to allow window maximizing either,
            // so we also ignore this. This has no effect on
            // being able to minimize the window, which is
            // still possible.
            return false;
        } // end case WM_GETMINMAXINFO
        case WM_NCLBUTTONDBLCLK:
        {
            // Likewise, normally double clicking on the window
            // would attempt to maximize the window (or whatever
            // is considered to be a titlebar), so we want to
            // ignore that as well to prevent the window from
            // being maximized.
            *result = 0;
            return true;
        } // end case WM_NCLBUTTONDBLCLK
        default:
        {
            return QMainWindow::nativeEvent(eventType, message, result);
        } // end default
    }
}

void FramelessWindow::setContentsMargins(const QMargins &margins)
{
    QMainWindow::setContentsMargins(margins + m_frames);
    m_margins = margins;
}

void FramelessWindow::setContentsMargins(int left, int top, int right, int bottom)
{
    QMainWindow::setContentsMargins(left + m_frames.left(), \
                                    top + m_frames.top(), \
                                    right + m_frames.right(), \
                                    bottom + m_frames.bottom());

    m_margins.setLeft(left);
    m_margins.setTop(top);
    m_margins.setRight(right);
    m_margins.setBottom(bottom);
}

QMargins FramelessWindow::contentsMargins() const
{
    QMargins margins = QMainWindow::contentsMargins();
    margins -= m_frames;
    return margins;
}

void FramelessWindow::getContentsMargins(int *left, int *top, int *right, int *bottom) const
{
    QMainWindow::getContentsMargins(left, top, right, bottom);
    if (!(left && top && right && bottom))
    {
        return;
    }
}

QRect FramelessWindow::contentsRect() const
{
    QRect rect = QMainWindow::contentsRect();
    int width = rect.width();
    int height = rect.height();
    rect.setLeft(rect.left() - m_frames.left());
    rect.setTop(rect.top() - m_frames.top());
    rect.setWidth(width);
    rect.setHeight(height);
    return rect;
}

void FramelessWindow::showFullScreen()
{
    QMainWindow::showFullScreen();
}

#endif // Q_OS_WIN
