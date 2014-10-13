#include <QtGui>

#include <QVector>
#include <QTextStream>

class Window : public QWindow
{
    Q_OBJECT

public:
    Window() :
        m_glContext(0), m_totalTime(0), m_frameCount(0)
    {
        setSurfaceType(OpenGLSurface);
        QSurfaceFormat format;
        format.setSwapInterval(1);
        setFormat(format);

        m_frameTimes.reserve(650);
    }

    ~Window()
    {
        QTextStream stream(stdout);

        stream << (const char *) m_gl->glGetString(GL_VENDOR) << ", "
               << (const char *) m_gl->glGetString(GL_RENDERER) << ", "
               << (const char *) m_gl->glGetString(GL_VERSION) << '\n';

        for(int i = 0; i < m_frameCount; i++) {
            stream << m_frameTimes[i] << '\n';
        }

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
        }

        ++m_frameCount;
        int c = m_frameCount % 2;

        m_gl->glClearColor(c, 0, 1 - c, 1);
        m_gl->glClear(GL_COLOR_BUFFER_BIT);

        m_glContext->swapBuffers(this);

        qint64 frameTime = m_timer.nsecsElapsed();
        m_timer.start();
        m_frameTimes[m_frameCount] = frameTime;
        m_totalTime += frameTime;

        if(m_frameCount > 600) {
            QGuiApplication::instance()->exit();
        } else {
            QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
        }
    }

private:
    QVector<qint64> m_frameTimes;

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
