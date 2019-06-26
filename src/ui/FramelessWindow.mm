#include "src/ui/FramelessWindow.h"

#ifdef Q_OS_MAC
#include <Cocoa/Cocoa.h>
#include <QOperatingSystemVersion>

FramelessWindow::FramelessWindow(QWidget *parent) :
    QMainWindow(parent),
    m_draggableHeight(0),
    m_bWinMoving(false),
    m_bMousePressed(false),
    m_bCloseBtnQuit(true),
    m_bNativeSystemBtn(false),
    m_bIsCloseBtnEnabled(true),
    m_bIsMinBtnEnabled(true),
    m_bIsZoomBtnEnabled(true),
    m_bIsCloseBtnHidden(false),
    m_bIsMinBtnHidden(false),
    m_bIsZoomBtnHidden(false),
    m_bTitleBarVisible(false)
{
    initUI();
}

// Objective-C class to override the action of the close & zoom buttons.
@interface ButtonPasser : NSObject
{
}

@property (atomic, readwrite) FramelessWindow* window;
+ (void)closeButtonAction:(id)sender;
- (void)zoomButtonAction:(id)sender;
@end

@implementation ButtonPasser
{
}

+ (void)closeButtonAction:(id)sender
{
    Q_UNUSED(sender);
    NSRunningApplication *currentApp = [NSRunningApplication currentApplication];
    [currentApp hide];
}

- (void)zoomButtonAction:(id)sender
{
    Q_UNUSED(sender);
    if (self.window == nullptr)
    {
        return;
    }

    if (self.window->isMaximized())
    {
        self.window->showNormal();
    }
    else
    {
        self.window->showMaximized();
    }
}
@end

void FramelessWindow::initUI()
{
    m_bNativeSystemBtn = false;

    QOperatingSystemVersion currentOS = QOperatingSystemVersion::current();
    if (currentOS.type() != QOperatingSystemVersion::MacOS &&
        currentOS.type() != QOperatingSystemVersion::IOS &&
        currentOS.type() != QOperatingSystemVersion::TvOS &&
        currentOS.type() != QOperatingSystemVersion::WatchOS)
    {
        setWindowFlags(Qt::FramelessWindowHint);
        return;
    }

    if (currentOS <= QOperatingSystemVersion::OSXMavericks)
    {
        setWindowFlags(Qt::FramelessWindowHint);
        return;
    }

    NSView* view = reinterpret_cast<NSView*>(winId());
    if (view == nullptr)
    {
        setWindowFlags(Qt::FramelessWindowHint);
        return;
    }

    NSWindow *window = view.window;
    if (window == nullptr)
    {
        setWindowFlags(Qt::FramelessWindowHint);
        return;
    }

    window.titleVisibility = NSWindowTitleHidden;
    window.titlebarAppearsTransparent = YES;
    window.movableByWindowBackground = YES;
    [window setMovable:YES];
    window.styleMask |= NSWindowStyleMaskFullSizeContentView;

    m_bNativeSystemBtn = true;

    ButtonPasser * passer = [[ButtonPasser alloc] init];
    passer.window = this;

    // Override the action of the fullscreen button:
    NSButton *zoomButton = [window standardWindowButton:NSWindowZoomButton];
    [zoomButton setTarget:passer];
    [zoomButton setAction:@selector(zoomButtonAction:)];
}

void FramelessWindow::setCloseBtnQuit(bool bQuit)
{
    if (bQuit || !m_bNativeSystemBtn)
    {
        return;
    }

    NSView* view = reinterpret_cast<NSView*>(winId());
    if (view == nullptr)
    {
        return;
    }

    NSWindow *window = view.window;
    if (window == nullptr)
    {
        return;
    }

    // Override the action of the close button:
    NSButton *closeButton = [window standardWindowButton:NSWindowCloseButton];
    [closeButton setTarget:[ButtonPasser class]];
    [closeButton setAction:@selector(closeButtonAction:)];
}

void FramelessWindow::setCloseBtnEnabled(bool bEnable)
{
    if (!m_bNativeSystemBtn)
    {
        return;
    }

    NSView* view = reinterpret_cast<NSView*>(winId());
    if (view == nullptr)
    {
        return;
    }

    NSWindow *window = view.window;
    if (window == nullptr)
    {
        return;
    }

    m_bIsCloseBtnEnabled = bEnable;
    if (bEnable)
    {
        [[window standardWindowButton:NSWindowCloseButton] setEnabled:YES];
    }
    else
    {
        [[window standardWindowButton:NSWindowCloseButton] setEnabled:NO];
    }
}

void FramelessWindow::setMinBtnEnabled(bool bEnable)
{
    if (!m_bNativeSystemBtn)
    {
        return;
    }

    NSView* view = reinterpret_cast<NSView*>(winId());
    if (view == nullptr)
    {
        return;
    }

    NSWindow *window = view.window;
    if (window == nullptr)
    {
        return;
    }

    m_bIsMinBtnEnabled = bEnable;
    if (bEnable)
    {
        [[window standardWindowButton:NSWindowMiniaturizeButton] setEnabled:YES];
    }
    else
    {
        [[window standardWindowButton:NSWindowMiniaturizeButton] setEnabled:NO];
    }
}

void FramelessWindow::setZoomBtnEnabled(bool bEnable)
{
    if (!m_bNativeSystemBtn)
    {
        return;
    }

    NSView* view = reinterpret_cast<NSView*>(winId());
    if (view == nullptr)
    {
        return;
    }

    NSWindow *window = view.window;
    if (window == nullptr)
    {
        return;
    }

    m_bIsZoomBtnEnabled = bEnable;
    if (bEnable)
    {
        [[window standardWindowButton:NSWindowZoomButton] setEnabled:YES];
    }
    else
    {
        [[window standardWindowButton:NSWindowZoomButton] setEnabled:NO];
    }
}

void FramelessWindow::setCloseBtnHidden(bool bHidden)
{
    if (!m_bNativeSystemBtn)
    {
        return;
    }

    NSView* view = reinterpret_cast<NSView*>(winId());
    if (view == nullptr)
    {
        return;
    }

    NSWindow *window = view.window;
    if (window == nullptr)
    {
        return;
    }

    m_bIsCloseBtnHidden = bHidden;
    if (bHidden)
    {
        [[window standardWindowButton:NSWindowCloseButton] setHidden:YES];
    }
    else
    {
        [[window standardWindowButton:NSWindowCloseButton] setHidden:NO];
    }
}

void FramelessWindow::setMinBtnHidden(bool bHidden)
{
    if (!m_bNativeSystemBtn)
    {
        return;
    }

    NSView* view = reinterpret_cast<NSView*>(winId());
    if (view == nullptr)
    {
        return;
    }

    NSWindow *window = view.window;
    if (window == nullptr)
    {
        return;
    }

    m_bIsMinBtnHidden = bHidden;
    if (bHidden)
    {
        [[window standardWindowButton:NSWindowMiniaturizeButton] setHidden:YES];
    }
    else
    {
        [[window standardWindowButton:NSWindowMiniaturizeButton] setHidden:NO];
    }
}

void FramelessWindow::setZoomBtnHidden(bool bHidden)
{
    if (!m_bNativeSystemBtn)
    {
        return;
    }

    NSView* view = reinterpret_cast<NSView*>(winId());
    if (view == nullptr)
    {
        return;
    }

    NSWindow *window = view.window;
    if (window == nullptr)
    {
        return;
    }

    m_bIsZoomBtnHidden = bHidden;
    if (bHidden)
    {
        [[window standardWindowButton:NSWindowZoomButton] setHidden:YES];
    }
    else
    {
        [[window standardWindowButton:NSWindowZoomButton] setHidden:NO];
    }
}

void FramelessWindow::setDraggableAreaHeight(int height)
{
    if (height < 0)
    {
        height = 0;
    }

    m_draggableHeight = height;
}

void FramelessWindow::mousePressEvent(QMouseEvent *event)
{
    if ((event->button() != Qt::LeftButton) || isMaximized())
    {
        return QMainWindow::mousePressEvent(event);
    }

    int height = size().height();
    if (m_draggableHeight > 0)
    {
        height = m_draggableHeight;
    }

    QRect rc;
    rc.setRect(0, 0, size().width(), height);
    if (rc.contains(this->mapFromGlobal(QCursor::pos())) == true)
    {
        m_WindowPos = this->pos();
        m_MousePos = event->globalPos();
        m_bMousePressed = true;
    }

    return QMainWindow::mousePressEvent(event);
}

void FramelessWindow::mouseReleaseEvent(QMouseEvent *event)
{
    m_bWinMoving = false;
    if ((event->button() == Qt::LeftButton))
    {
        m_bMousePressed = false;
    }

    return QMainWindow::mouseReleaseEvent(event);
}

void FramelessWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (!m_bMousePressed)
    {
        return QMainWindow::mouseMoveEvent(event);
    }

    m_bWinMoving = true;
    this->move(m_WindowPos + (event->globalPos() - m_MousePos));
    return QMainWindow::mouseMoveEvent(event);
}

void FramelessWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    // TODO
    //if (!isFullScreen())
    //{
    //    emit restoreFromFullScreen();
    //}
}

void FramelessWindow::onRestoreFromFullScreen()
{
    setTitlebarVisible(false);
}

void FramelessWindow::setTitlebarVisible(bool bTitlebarVisible)
{
    if (!m_bNativeSystemBtn)
    {
        return;
    }

    NSView* view = reinterpret_cast<NSView*>(winId());
    if (view == nullptr)
    {
        return;
    }

    NSWindow *window = view.window;
    if (window == nullptr)
    {
        return;
    }

    m_bTitleBarVisible = bTitlebarVisible;
    if (bTitlebarVisible)
    {
        window.styleMask ^= NSWindowStyleMaskFullSizeContentView;
    }
    else
    {
        window.styleMask |= NSWindowStyleMaskFullSizeContentView;
    }
}

#endif // Q_OS_MAC
