#include <backend/backendtypes/physicsgeometryrenderer.h>

#include <backend/physicsmanager.h>

namespace Physics {

PhysicsGeometryRenderer::PhysicsGeometryRenderer():
    Qt3DCore::QBackendNode(),
    m_objectName(),
    m_dirty(false),
    m_enabled(false),
    m_geometry(),
    m_geometry_functor(),
    m_manager(Q_NULLPTR),
    m_instanceCount(0)
    , m_primitiveCount(0)
    , m_baseVertex(0)
    , m_baseInstance(0)
    , m_restartIndex(-1)
    , m_primitiveRestart(false)
    , m_primitiveType(Qt3DRender::QGeometryRenderer::Triangles)
{

}

void PhysicsGeometryRenderer::setManager(PhysicsManager *manager){
        m_manager=manager;
}


PhysicsGeometryRenderer::~PhysicsGeometryRenderer(){
    m_manager->m_resources.remove(peerUuid());
}

void PhysicsGeometryRenderer::updateFromPeer(Qt3DCore::QNode *peer){
    Qt3DRender::QGeometryRenderer *geometry_renderer = static_cast<Qt3DRender::QGeometryRenderer*>(peer);
    m_instanceCount = geometry_renderer->instanceCount();
    m_primitiveCount = geometry_renderer->primitiveCount();
    m_baseVertex = geometry_renderer->baseVertex();
    m_baseInstance = geometry_renderer->baseInstance();
    m_restartIndex = geometry_renderer->restartIndex();
    m_primitiveRestart = geometry_renderer->primitiveRestart();
    m_primitiveType = geometry_renderer->primitiveType();

    m_objectName = peer->objectName();
    m_dirty=true;
    m_enabled=geometry_renderer->isEnabled();
    if (geometry_renderer->geometry() != Q_NULLPTR)
        setGeometry(geometry_renderer->geometry()->id());
    m_geometry_functor = geometry_renderer->geometryFunctor();
}

void PhysicsGeometryRenderer::setGeometry(Qt3DCore::QNodeId geometry)
{
    if (m_geometry != geometry) {
        m_geometry = geometry;
    }
}

void PhysicsGeometryRenderer::sceneChangeEvent(const Qt3DCore::QSceneChangePtr &e){
    switch (e->type()) {
        case Qt3DCore::NodeUpdated: {
            Qt3DCore::QScenePropertyChangePtr propertyChange = qSharedPointerCast<Qt3DCore::QScenePropertyChange>(e);
            QByteArray propertyName = propertyChange->propertyName();
            if (propertyName == QByteArrayLiteral("geometryFunctor")){ // Mesh with source
                m_geometry_functor= propertyChange->value().value<Qt3DRender::QGeometryFunctorPtr>();
                m_dirty=true;
            }
            else if (propertyName == QByteArrayLiteral("enabled")){
                m_enabled = propertyChange->value().value<bool>();
            }else if (propertyChange->propertyName() == QByteArrayLiteral("instanceCount")) {
                m_instanceCount = propertyChange->value().value<int>();
                m_dirty = true;
            } else if (propertyName == QByteArrayLiteral("primitiveCount")) {
                m_primitiveCount = propertyChange->value().value<int>();
                m_dirty = true;
            } else if (propertyName == QByteArrayLiteral("baseVertex")) {
                m_baseVertex = propertyChange->value().value<int>();
                m_dirty = true;
            } else if (propertyName == QByteArrayLiteral("baseInstance")) {
                m_baseInstance = propertyChange->value().value<int>();
                m_dirty = true;
            } else if (propertyName == QByteArrayLiteral("restartIndex")) {
                m_restartIndex = propertyChange->value().value<int>();
                m_dirty = true;
            } else if (propertyName == QByteArrayLiteral("primitiveRestart")) {
                m_primitiveRestart = propertyChange->value().value<bool>();
                m_dirty = true;
            }
            break;
    }
    case Qt3DCore::NodeAdded: {
        Qt3DCore::QScenePropertyChangePtr propertyChange = qSharedPointerCast<Qt3DCore::QScenePropertyChange>(e);
        if (propertyChange->propertyName() == QByteArrayLiteral("geometry")) {
            m_geometry = propertyChange->value().value<Qt3DCore::QNodeId>();
        }
        break;
    }
    case Qt3DCore::NodeRemoved: {
        Qt3DCore::QScenePropertyChangePtr propertyChange = qSharedPointerCast<Qt3DCore::QScenePropertyChange>(e);
        if (propertyChange->propertyName() == QByteArrayLiteral("geometry")) {
            m_geometry = Qt3DCore::QNodeId();
        }
        break;
    }
    default:
        break;
    }
}


PhysicsGeometryRendererFunctor::PhysicsGeometryRendererFunctor(PhysicsManager* manager)
{
    m_manager=manager;
}


Qt3DCore::QBackendNode *PhysicsGeometryRendererFunctor::create(Qt3DCore::QNode *frontend, const Qt3DCore::QBackendNodeFactory *factory)
const {
    PhysicsGeometryRenderer* geometry_renderer=new PhysicsGeometryRenderer();
    m_manager->m_resources.insert(frontend->id(),geometry_renderer);
    geometry_renderer->setFactory(factory);
    geometry_renderer->setManager(m_manager);
    geometry_renderer->setPeer(frontend);
    return geometry_renderer;
}
Qt3DCore::QBackendNode *PhysicsGeometryRendererFunctor::get(const Qt3DCore::QNodeId &id) const
{
    if(m_manager->m_resources.contains(id))
        return m_manager->m_resources.operator [](id);
    else
        return Q_NULLPTR;
}
void PhysicsGeometryRendererFunctor::destroy(const Qt3DCore::QNodeId &id) const
{
    if(m_manager->m_resources.contains(id))
        delete m_manager->m_resources.operator [](id);
}



}


