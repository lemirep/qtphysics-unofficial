#ifndef PhysicsASPECT_H
#define PhysicsASPECT_H

#include <qtphysicsunofficial_global.h>


QT_BEGIN_NAMESPACE

namespace Physics {

class PhysicsManager;

class QTPHYSICSUNOFFICIAL_EXPORT PhysicsAspect: public Qt3DCore::QAbstractAspect
{
public:
    explicit PhysicsAspect(QObject* parent=0);

    QVector<Qt3DCore::QAspectJobPtr> jobsToExecute(qint64 time) Q_DECL_OVERRIDE;
//    void sceneNodeAdded(Qt3DCore::QSceneChangePtr &e) Q_DECL_OVERRIDE;
//    void sceneNodeRemoved(Qt3DCore::QSceneChangePtr &e) Q_DECL_OVERRIDE;

private:
 //   void setRootEntity(Qt3DCore::QEntity *rootObject) Q_DECL_OVERRIDE;
    void onRootEntityChanged(Qt3DCore::QEntity *rootEntity) Q_DECL_OVERRIDE;
    #if (QT_VERSION == QT_VERSION_CHECK(5, 6, 0))
    void onInitialize(const QVariantMap &data) Q_DECL_OVERRIDE;
    #elif (QT_VERSION == QT_VERSION_CHECK(5, 7, 0))
    void onInitialize() Q_DECL_OVERRIDE;
    #endif
    void onCleanup() Q_DECL_OVERRIDE;
    void visitNode(Qt3DCore::QNode *node);

    PhysicsManager* m_manager;

signals:

public slots:
};
}
QT_END_NAMESPACE

#endif // PhysicsASPECT_H
