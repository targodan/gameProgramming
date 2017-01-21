#include "Game.h"

#include <memory>

#include "engine/renderer/Mesh.h"
#include "engine/ECSCommon.h"
#include "OneShotForce.h"
#include "Actions.h"

using namespace engine;
using namespace engine::renderer;
using namespace engine::ECSCommon;
using namespace demo::IO;

namespace demoSimulation {
    void Game::initialize() {
        this->window.setClearColor(0.1f, 0.f, 0.1f);
        
        engine::renderer::TextRenderer::getInstance().setWindowDimensions(this->window.getWidth(), this->window.getHeight());
        LOG(INFO) << "Window dimensions: " << this->window.getWidth() << "x" << this->window.getHeight();

        Vertex frontBottom({0, 0, 0.5}, {0, 1, 0});
        Vertex backLeft({-0.5, 0, -0.5}, {0, 0, 1});
        Vertex backRight({0.5, 0, -0.5}, {1, 0, 0});
        Vertex up({0, 1, 0}, {1, 1, 0});
        Mesh tetrahedronMesh({frontBottom, backRight, backLeft, up}, {0, 1, 3, 2, 0, 3, 1, 2, 3, 1, 0, 2});
        tetrahedronMesh.loadMesh();
        
        std::shared_ptr<Material> material = std::make_shared<Material>(std::make_shared<ShaderProgram>("src/triangle_sh.vsh", 
                                                         "src/triangle_sh.fsh"), true);
        
        this->camera = this->entityManager.createEntity("Camera")
                .addComponent<PlacementComponent>(engine::util::vec3(0, 0.5, 2))
                .addComponent<CameraComponent>(engine::util::vec3(0, 0, -1), engine::util::vec3(0, 1, 0));
        auto& cc = this->camera.getComponent<CameraComponent>();
        cc.setProjectionMatrix(120, this->window.getAspectRatio(), 0.1, 10);
        
        
        this->tetrahedron = this->entityManager.createEntity("Tetrahedron")
                .addComponent<VisualComponent>(tetrahedronMesh, *material)
                .addComponent<PlacementComponent>(engine::util::vec3(0, 0, 0));
        
        auto& vc = this->tetrahedron.getComponent<VisualComponent>();
        
        float volume = 0.1;
        float area = 5;
        float density = 920; // kg / m³
        float mass = volume * density;
        
        auto properties = 
                engine::physics::ObjectProperties::uniformTetrahedronDistribution(
                            engine::physics::ObjectProperties::verticesToFlatVector(vc.getMesh().getVertices()), engine::util::vector<size_t>({0, 1, 2, 3})
                        )
                        .uniformDensity(volume, density)
                        .uniformAreaDistribution(area);
        
        auto defBody = std::make_shared<engine::physics::DeformableBody>(
                vc.getMesh(),
                properties,
                mass,
                0.1,
                0.05e9,
                0.4999,
                this->updatesPerSecond
            );
        
        auto defBodyEntity = this->entityManager.createEntity("DeformableBody")
                .addComponent<DeformableBodyComponent>(defBody);
        
        auto force = std::make_shared<OneShotForce>();
        this->entityManager.createEntity("Force")
                .addComponent<TimerComponent>(5)
                .addComponent<ForceComponent>(force);
        
        auto& fontfamiliy = FontRegistry::registerFontFamily(
            "DejaVuSans",
            "/usr/share/fonts/TTF/DejaVuSans.ttf",
            "/usr/share/fonts/TTF/DejaVuSans-Bold.ttf",
            "/usr/share/fonts/TTF/DejaVuSans-Oblique.ttf",
            "/usr/share/fonts/TTF/DejaVuSans-BoldOblique.ttf"
        );
        
        RichText testText(fontfamiliy, FontType::Regular, 80, Color::RED);
        testText << u8"Test öä§∑. "
                << RichText::fontType(FontType::Bold) << RichText::color(Color(1, 1, 0, 1)) << u8"Bold"
                << RichText::fontType(FontType::Italic) << RichText::color(Color::BLUE) << u8"Italic "
                << RichText::fontType(FontType::Italic) << RichText::color(Color::GREEN) << u8"Italic ";
        
        LOG(INFO) << testText.getPlainText_utf8();
        
        this->entityManager.createEntity("testtext")
                .addComponent<TextComponent>(testText, 50, 100);
        
        this->systemManager.enableSystem<PlacementSystem>();
        this->systemManager.enableSystem<CameraRenderSystem>();
        this->systemManager.enableSystem<RenderSystem>();
        this->systemManager.enableSystem<DeformableBodySystem>();
        this->systemManager.enableSystem<TimerSystem>();
        
        engine::Game::initialize();
    }
}
