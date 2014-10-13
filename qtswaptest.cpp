#include <QtGui>

#include <QTextStream>

class Window : public QWindow
{
    Q_OBJECT

public:
    Window() :
        m_stream(stdout), m_glContext(0), m_totalTime(0), m_frameCount(0)
    {
        setSurfaceType(OpenGLSurface);
        QSurfaceFormat format;
        format.setSwapInterval(1);
        setFormat(format);
    }

    ~Window()
    {
        if(m_glContext) {
            delete m_glContext;
        }
    }

    void exposeEvent(QExposeEvent *) Q_DECL_OVERRIDE
    {
        if(isExposed()) {
            render();
        }
    }

    bool event(QEvent *e) Q_DECL_OVERRIDE
    {
        if(e->type() == QEvent::UpdateRequest) {
            render();
            return true;
        }
        return QWindow::event(e);
    }

    void render()
    {
        if(!m_glContext) {
            m_glContext = new QOpenGLContext();
            m_glContext->setFormat(format());
            m_glContext->create();

            m_timer.start();
        }

        m_glContext->makeCurrent(this);

        if(m_frameCount == 0) {
            m_gl = m_glContext->functions();

            m_stream << "Vendor/Renderer/Version: "
                     << (const char *) m_gl->glGetString(GL_VENDOR) << ", "
                     << (const char *) m_gl->glGetString(GL_RENDERER) << ", "
                     << (const char *) m_gl->glGetString(GL_VERSION) << '\n';
        }

        ++m_frameCount;
        int c = m_frameCount % 2;

        m_gl->glClearColor(c, 0, 1 - c, 1);
        m_gl->glClear(GL_COLOR_BUFFER_BIT);

        m_glContext->swapBuffers(this);

        qint64 frameTime = m_timer.nsecsElapsed();
        m_timer.start();
        m_totalTime += frameTime;

        m_stream << frameTime << '\n';

        if(m_frameCount > 600) {
            QGuiApplication::instance()->exit();
        } else {
            QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
        }
    }

private:
    QTextStream m_stream;

    QOpenGLFunctions *m_gl;
    QOpenGLContext *m_glContext;

    qint64 m_totalTime;

    QElapsedTimer m_timer;
    int m_frameCount;
};

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    Window w;
    w.showFullScreen();

    return app.exec();
}

#include "qtswaptest.moc"
