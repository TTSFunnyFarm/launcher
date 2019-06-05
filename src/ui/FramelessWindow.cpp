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
    m_titlebar(Q_NULLPTR),
    m_borderWidth(5),
    m_bJustMaximized(false),
    m_bResizable(true)
{
    setWindowFlags(windowFlags() | Qt::Window | Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);
    setResizable(m_bResizable);
}

void FramelessWindow::setResizable(bool resizable)
{
    bool visible = isVisible();
    m_bResizable = resizable;
    if (m_bResizable)
    {
        setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);

        // The following code will restore the titlebar and Aero, which is what we want.
        // We will get rid of the titlebar again in nativeEvent() later.
        HWND hwnd = HWND(this->winId());
        DWORD style = DWORD(::GetWindowLong(hwnd, GWL_STYLE));
        style = style | WS_CAPTION;
        style = style & (ULONG(~WS_SYSMENU));
        ::SetWindowLong(hwnd, GWL_STYLE, LONG(style));
    }
    else
    {
        setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);

        HWND hwnd = HWND(this->winId());
        DWORD style = DWORD(::GetWindowLong(hwnd, GWL_STYLE));
        ::SetWindowLong(hwnd, GWL_STYLE, LONG(style & ULONG(~WS_MAXIMIZEBOX & ~WS_CAPTION)));
    }

    // We better leave 1 pixel width of border untouched, so that Windows
    // can draw a nice shadow around it.
    const MARGINS shadow = { 1, 1, 1, 1 };
    DwmExtendFrameIntoClientArea(HWND(winId()), &shadow);

    setVisible(visible);
}

void FramelessWindow::setResizableAreaWidth(int width)
{
    if (width < 1)
    {
        width = 1;
    }

    m_borderWidth = width;
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
    // Workaround for known bug; see https://forum.qt.io/topic/93141/qtablewidget-itemselectionchanged/13
    #if (QT_VERSION == QT_VERSION_CHECK(5, 11, 1))
    MSG* msg = *reinterpret_cast<MSG**>(message);
    #else
    MSG* msg = reinterpret_cast<MSG*>(message);
    #endif

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
            *result = 0;

            const LONG border_width = m_borderWidth;
            RECT winrect;
            GetWindowRect(HWND(winId()), &winrect);

            long x = GET_X_LPARAM(msg->lParam);
            long y = GET_Y_LPARAM(msg->lParam);

            if (m_bResizable)
            {
                bool resizeWidth = minimumWidth() != maximumWidth();
                bool resizeHeight = minimumHeight() != maximumHeight();

                if (resizeWidth)
                {
                    // Left border
                    if (x >= winrect.left && x < winrect.left + border_width)
                    {
                        *result = HTLEFT;
                    }

                    // Right border
                    if (x < winrect.right && x >= winrect.right - border_width)
                    {
                        *result = HTRIGHT;
                    }
                }

                if (resizeHeight)
                {
                    // Bottom border
                    if (y < winrect.bottom && y >= winrect.bottom - border_width)
                    {
                        *result = HTBOTTOM;
                    }

                    // Top border
                    if (y >= winrect.top && y < winrect.top + border_width)
                    {
                        *result = HTTOP;
                    }
                }

                if (resizeWidth && resizeHeight)
                {
                    // Bottom left corner
                    if (x >= winrect.left && x < winrect.left + border_width &&
                        y < winrect.bottom && y >= winrect.bottom - border_width)
                    {
                        *result = HTBOTTOMLEFT;
                    }

                    // Bottom right corner
                    if (x < winrect.right && x >= winrect.right - border_width &&
                        y < winrect.bottom && y >= winrect.bottom - border_width)
                    {
                        *result = HTBOTTOMRIGHT;
                    }

                    // Top left corner
                    if (x >= winrect.left && x < winrect.left + border_width &&
                        y >= winrect.top && y < winrect.top + border_width)
                    {
                        *result = HTTOPLEFT;
                    }

                    // Top right corner
                    if (x < winrect.right && x >= winrect.right - border_width &&
                        y >= winrect.top && y < winrect.top + border_width)
                    {
                        *result = HTTOPRIGHT;
                    }
                }
            }

            if (*result != 0)
            {
                return true;
            }

            // *result still equaling 0 means that the cursor is
            // located outside the frame area, but it may also
            // be located within the titlebar area.
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
            if (::IsZoomed(msg->hwnd))
            {
                RECT frame = { 0, 0, 0, 0 };
                AdjustWindowRectEx(&frame, WS_OVERLAPPEDWINDOW, FALSE, 0);

                // Record frame area data:
                double dpr = this->devicePixelRatioF();

                m_frames.setLeft(int(abs(frame.left) / dpr + 0.5));
                m_frames.setTop(int(abs(frame.bottom) / dpr + 0.5));
                m_frames.setRight(int(abs(frame.right) / dpr + 0.5));
                m_frames.setBottom(int(abs(frame.bottom) / dpr + 0.5));

                QMainWindow::setContentsMargins(m_frames.left() + m_margins.left(), \
                                                m_frames.top() + m_margins.top(), \
                                                m_frames.right() + m_margins.right(), \
                                                m_frames.bottom() + m_margins.bottom());

                m_bJustMaximized = true;
            }
            else
            {
                if (m_bJustMaximized)
                {
                    QMainWindow::setContentsMargins(m_margins);
                    m_frames = QMargins();
                    m_bJustMaximized = false;
                }
            }

            return false;
        } // end case WM_GETMINMAXINFO
        case WM_NCLBUTTONDBLCLK:
        {
            *result = 0;
            return true;
        }
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

    if (isMaximized())
    {
        *left -= m_frames.left();
        *top -= m_frames.top();
        *right -= m_frames.right();
        *bottom -= m_frames.bottom();
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
    if (isMaximized())
    {
        QMainWindow::setContentsMargins(m_margins);
        m_frames = QMargins();
    }

    QMainWindow::showFullScreen();
}

#endif // Q_OS_WIN
